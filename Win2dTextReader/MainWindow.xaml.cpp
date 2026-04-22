#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif


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
		
		// Window
		this->ExtendsContentIntoTitleBar(true);
		this->AppWindow().Changed({this, &MainWindow::OnWindowPropertyChanged});
		this->RestoreRedingHistoryAsync(); 

		// Popup
		this->ContentsPopup().Child(m_bookContents);
		this->NovelFileInfoPopup().Child(m_novelInfoControl); 
		this->SettingsPopup().Child(m_settings);
	}

	winrt::Win2dTextReader::Settings MainWindow::ReaderSettings()
	{
		return m_settings; 
	}

	winrt::Windows::Foundation::Numerics::float4 MainWindow::GetPopupRegion(float wScale, float hScale)
	{
		winrt::Windows::Foundation::Numerics::float2 readerSize = this->ReaderRegion().ActualSize(); 
		float width = readerSize.x * wScale; 
		float height = readerSize.y * hScale; 

		if (width < 300.0f) {
			width = readerSize.x - 40.0f; 
		}
		
		if (width > 800.0f) {
			width = 800.0f; 
		}

		float left = (readerSize.x - width) / 2.0f; 
		float top = readerSize.y - height - 20.0f;;

		return { left, top, width, height };
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
			if(newSize.Width > 300)
				m_readingHistory.WindowSizeW(newSize.Width); 

			if (newSize.Height > 300)
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

		auto region = this->GetPopupRegion(0.5f, 0.9f); 

		m_bookContents.Height(region.w); 
		m_bookContents.Width(region.z); 

		this->ContentsPopup().HorizontalOffset(region.x); 
		this->ContentsPopup().VerticalOffset(region.y); 
		this->ContentsPopup().IsOpen(true); 

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 }); 
		co_await context; 
		m_bookContents.SetSelectedIndex(m_readingHistory.ChapterIndex()); 
	}

	// 显示小说信息列表
	void MainWindow::ShowNovelInfo(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto region = this->GetPopupRegion(0.5f, 0.5f);

		m_novelInfoControl.Height(region.w);
		m_novelInfoControl.Width(region.z);

		this->NovelFileInfoPopup().HorizontalOffset(region.x);
		this->NovelFileInfoPopup().VerticalOffset(region.y);

		m_novelInfoControl.SetNovelBook(m_novelBook);
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
		auto viewportHeight = this->ContentScrollView().ViewportHeight(); 
		auto lineHeight = m_settings.ReaderLineHeight();
		auto scrollAmount = std::floor(viewportHeight / lineHeight) * lineHeight; 

		auto verticalOffset = this->ContentScrollView().VerticalOffset(); 
		if (verticalOffset < scrollAmount) {
			scrollAmount = verticalOffset; 
		}

		this->ContentScrollView().ScrollBy(0.0, -scrollAmount);
	}

	// 向下滚动
	void MainWindow::OnReaderRegionScrollDown(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		auto viewportHeight = this->ContentScrollView().ViewportHeight();
		auto lineHeight = m_settings.ReaderLineHeight();
		auto scrollAmount = std::floor(viewportHeight / lineHeight) * lineHeight;

		auto restAmount = this->ContentScrollView().ScrollableHeight() - this->ContentScrollView().VerticalOffset();
		if (restAmount < 0) {
			return;
		}

		scrollAmount = restAmount > scrollAmount ? scrollAmount : restAmount;
		this->ContentScrollView().ScrollBy(0.0, scrollAmount);
	}

	// 显示设置
	void MainWindow::ShowSettings(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::apartment_context context;

		auto region = this->GetPopupRegion(0.6f, 0.6f); 

		m_settings.Height(region.w);
		m_settings.Width(region.z);

		this->SettingsPopup().HorizontalOffset(region.x);
		this->SettingsPopup().VerticalOffset(region.y);
		this->SettingsPopup().IsOpen(true);
	}
}









