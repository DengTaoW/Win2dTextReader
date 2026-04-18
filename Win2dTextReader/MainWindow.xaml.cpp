#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

constexpr float POPUP_BOTTOM_PADDING = 20.0F; 

namespace winrt::Win2dTextReader::implementation
{
	constexpr uint32_t INVALID_CHAPTER_INDEX{ UINT32_MAX }; 

	MainWindow::MainWindow()
	{
		m_readingHistory = winrt::Win2dTextReader::ReadingHistory(); 
		m_isReadingHistoryValid = m_readingHistory.Load(); 
		if (!m_isReadingHistoryValid) {
			m_readingHistory.ChapterIndex(INVALID_CHAPTER_INDEX); 
		}
	}

	void MainWindow::InitializeComponent()
	{
		MainWindowT::InitializeComponent(); 

		m_bookContents = winrt::Win2dTextReader::BookContents(); 
		m_bookContents.SelectedChapterChanged({this, &MainWindow::SetCurrentChapter});
		
		m_novelInfoControl = winrt::Win2dTextReader::NovelInfoControl(); 
		
		m_settings = winrt::Win2dTextReader::Settings(); 
		m_settings.PropertyChanged({this, &MainWindow::SettingsPropertyChanged});

		this->ChapterContentTextBlock().FontSize(m_settings.ReaderFontSize());
		this->ChapterContentTextBlock().LineHeight(m_settings.ActualLineHeight());
		this->ChapterContentTextBlock().FontFamily(m_settings.ReaderFontFamily());
		
		// Window
		this->ExtendsContentIntoTitleBar(true);
		this->AppWindow().Changed({this, &MainWindow::OnWindowPropertyChanged});
		this->RestoreRedingHistoryAsync(); 

		// Popup
		this->ContentsPopup().Child(m_bookContents);
		this->NovelFileInfoPopup().Child(m_novelInfoControl); 
		this->SettingsPopup().Child(m_settings);
	}

	winrt::fire_and_forget MainWindow::RestoreRedingHistoryAsync()
	{
		winrt::Microsoft::UI::Windowing::OverlappedPresenter presenter =
			winrt::Microsoft::UI::Windowing::OverlappedPresenter::Create();
		
		presenter.SetBorderAndTitleBar(true, true); 
		presenter.IsMinimizable(false); 
		presenter.IsMaximizable(false); 
		presenter.IsResizable(true); 

		this->AppWindow().SetPresenter(presenter);

		if (!m_isReadingHistoryValid) {
			winrt::Xuanwen::Novel::NovelBook book = m_readingHistory.LoadUaseges(); 
			co_await this->ReadBookAsync(book, true); 
		}
		else {
			winrt::Windows::Graphics::RectInt32 rect{
				m_readingHistory.WindowPositionX(),
				m_readingHistory.WindowPositionY(),
				m_readingHistory.WindowSizeW(),
				m_readingHistory.WindowSizeH()
			};
			this->AppWindow().MoveAndResize(rect); 
			winrt::Xuanwen::Novel::NovelBook book{ m_readingHistory.FilePath() }; 
			co_await this->ReadBookAsync(book, false);
		}
	}

	void MainWindow::OnWindowPropertyChanged(
		winrt::Microsoft::UI::Windowing::AppWindow const& appwindow, 
		winrt::Microsoft::UI::Windowing::AppWindowChangedEventArgs const args)
	{
		if (args.DidPositionChange()) {
			auto newPosition = appwindow.Position();
			m_readingHistory.WindowPositionX(newPosition.X); 
			m_readingHistory.WindowPositionY(newPosition.Y); 
		}

		if (args.DidSizeChange()) {
			auto newSize = appwindow.Size(); 
			m_readingHistory.WindowSizeW(newSize.Width); 
			m_readingHistory.WindowSizeH(newSize.Height);
		}
	}

	winrt::Windows::Foundation::IAsyncAction MainWindow::ReadBookAsync(winrt::Xuanwen::Novel::NovelBook const& book, bool isNewBook)
	{
		if (m_novelBook != nullptr) {
			m_novelBook.Chapters().Clear();
		}

		m_novelBook = book;
		co_await m_novelBook.InitializeAsync();

		m_readingHistory.FilePath(m_novelBook.FilePath()); 
		this->BookNameTextBlock().Text(m_novelBook.Name());

		if (m_novelBook.Chapters().Size() > 0) {
			uint32_t chapterIndex = 0u; 
			if (!isNewBook && m_isReadingHistoryValid) {
				chapterIndex = m_readingHistory.ChapterIndex(); 
			}

			m_bookContents.SetChapters(m_novelBook.Chapters()); 
			this->SetCurrentChapter(m_novelBook.Chapters().GetAt(chapterIndex));
		}
	}

	void MainWindow::SettingsPropertyChanged(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e)
	{
		winrt::hstring propertyName = e.PropertyName(); 
		if (propertyName == L"ReaderFontSize") 
		{
			this->ChapterContentTextBlock().FontSize(m_settings.ReaderFontSize());
			this->ChapterContentTextBlock().LineHeight(m_settings.ActualLineHeight());
		}
		else if (propertyName == L"ReaderLineHeight") 
		{
			this->ChapterContentTextBlock().LineHeight(m_settings.ActualLineHeight());
		}
		else if (propertyName == L"ReaderFontFamily") {
			this->ChapterContentTextBlock().FontFamily(m_settings.ReaderFontFamily()); 
		}
	}

	winrt::fire_and_forget MainWindow::SetCurrentChapter(winrt::Xuanwen::Novel::Chapter const& chapter)
	{
		winrt::apartment_context context; 

		m_readingHistory.ChapterIndex(chapter.Index());
		this->ChapterTitleTextBlock().Text(chapter.Title());
		this->ChapterContentTextBlock().Text(chapter.Text());
		this->ContentsPopup().IsOpen(false); 

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 }); 
		co_await context; 

		double verticalOffset = -1.0 * this->ContentScrollView().VerticalOffset();

		if (m_chaptersCounter == 0 && m_isReadingHistoryValid) {
			verticalOffset = m_readingHistory.VerticalOffset(); 
		}
		++m_chaptersCounter; 

		this->ContentScrollView().ScrollBy(0.0, verticalOffset);
	}

	winrt::fire_and_forget MainWindow::OnOpenButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::Microsoft::Windows::Storage::Pickers::FileOpenPicker picker{ this->AppWindow().Id() }; 
		picker.FileTypeFilter().Append(L".txt"); 
		picker.SuggestedStartLocation(winrt::Microsoft::Windows::Storage::Pickers::PickerLocationId::Downloads); 

		winrt::Microsoft::Windows::Storage::Pickers::PickFileResult result =
			co_await picker.PickSingleFileAsync(); 

		if (result == nullptr)
			co_return; 
		winrt::Xuanwen::Novel::NovelBook book{ result.Path() }; 
		co_await this->ReadBookAsync(book, true);
	}


	void MainWindow::OnPreviousChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			return;

		if (m_readingHistory.ChapterIndex() == 0)
			return; 

		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(m_readingHistory.ChapterIndex() - 1);
		this->SetCurrentChapter(chapter); 
	}

	void MainWindow::OnNextChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			return;

		auto newChapterIndex = m_readingHistory.ChapterIndex() + 1; 
		if (newChapterIndex >= m_novelBook.Chapters().Size())
			return;

		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(newChapterIndex);
		this->SetCurrentChapter(chapter); 
	}

	// 显示目录列表
	winrt::fire_and_forget MainWindow::ShowContents(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr || m_novelBook.Chapters().Size() == 0)
			co_return; 

		winrt::apartment_context context; 
		
		winrt::Windows::Foundation::Numerics::float2 containerSize = this->ReaderRegion().ActualSize();
		float width = containerSize.x * 0.5f; 
		float height = containerSize.y * 0.9f; 
		float left = (containerSize.x - width) / 2.0f; 
		float top = containerSize.y - height - POPUP_BOTTOM_PADDING; 

		m_bookContents.Width(width); 
		m_bookContents.Height(height); 

		this->ContentsPopup().HorizontalOffset(left); 
		this->ContentsPopup().VerticalOffset(top); 
		this->ContentsPopup().IsOpen(true); 

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 }); 
		co_await context; 
		m_bookContents.SetSelectedIndex(m_readingHistory.ChapterIndex()); 
	}

	// 显示小说信息列表
	void MainWindow::ShowNovelInfo(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{

		// 计算大小和位置
		winrt::Windows::Foundation::Numerics::float2 containerSize = this->ReaderRegion().ActualSize();
		float width = containerSize.x * 0.5f;
		float height = containerSize.y * 0.5f;
		float left = (containerSize.x - width) / 2.0f;
		float top = containerSize.y - height - POPUP_BOTTOM_PADDING;

		m_novelInfoControl.Width(width);
		m_novelInfoControl.Height(height);
		m_novelInfoControl.SetNovelBook(m_novelBook);

		this->NovelFileInfoPopup().HorizontalOffset(left);
		this->NovelFileInfoPopup().VerticalOffset(top);

		this->NovelFileInfoPopup().IsOpen(true);
	}


	void MainWindow::Window_Closed(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::WindowEventArgs const&)
	{
		m_readingHistory.Save(); 
		m_settings.Save(); 
	}

	// 记录滚动位置
	void MainWindow::ContentScrollView_ViewChanged(
		winrt::Microsoft::UI::Xaml::Controls::ScrollView const&, winrt::Windows::Foundation::IInspectable const&)
	{
		double verticalOffset = this->ContentScrollView().VerticalOffset(); 
		m_readingHistory.VerticalOffset(verticalOffset);
	}

	// 向上滚动
	void MainWindow::OnReaderRegionScrollUp(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto height = this->ContentScrollView().ActualHeight(); 
		this->ContentScrollView().ScrollBy(0.0, -0.5 * height);
	}

	// 向下滚动
	void MainWindow::OnReaderRegionScrollDown(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto height = this->ContentScrollView().ActualHeight();
		this->ContentScrollView().ScrollBy(0.0, 0.5*height); 
	}

	// 设置 Popup
	winrt::fire_and_forget MainWindow::ShowSettings(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::apartment_context context;

		// 计算大小和位置
		winrt::Windows::Foundation::Numerics::float2 containerSize = this->ReaderRegion().ActualSize();
		float width = containerSize.x * 0.6f;
		float height = containerSize.y * 0.6f;
		float left = (containerSize.x - width) / 2.0f;
		float top = containerSize.y - height - POPUP_BOTTOM_PADDING;

		m_settings.Width(width);
		m_settings.Height(height);

		this->SettingsPopup().HorizontalOffset(left);
		this->SettingsPopup().VerticalOffset(top);

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 });
		co_await context;
		this->SettingsPopup().IsOpen(true);
		m_settings.UpdateUI();
	}
}









