#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif


namespace winrt::Win2dTextReader::implementation
{
	MainWindow::MainWindow()
		: m_chapterIndex{ UINT32_MAX }
	{
	}

	void MainWindow::InitializeComponent()
	{
		MainWindowT::InitializeComponent(); 
		m_bookContents = winrt::Win2dTextReader::BookContents(); 
		m_bookContents.SelectedChapterChanged({this, &MainWindow::SetCurrentChapter});

		m_novelInfoControl = winrt::Win2dTextReader::NovelInfoControl(); 
		this->SetWindowStyle(); 

		// 设置 popup 控件
		this->ContentsPopup().Child(m_bookContents);
		this->NovelFileInfoPopup().Child(m_novelInfoControl); 
	}

	void MainWindow::SetWindowStyle()
	{
		this->ExtendsContentIntoTitleBar(true); 

		winrt::Microsoft::UI::Windowing::OverlappedPresenter presenter =
			winrt::Microsoft::UI::Windowing::OverlappedPresenter::Create();
		
		presenter.SetBorderAndTitleBar(true, true); 
		presenter.IsMinimizable(false); 
		presenter.IsMaximizable(false); 
		presenter.IsResizable(true); 

		this->AppWindow().SetPresenter(presenter);
	}

	winrt::fire_and_forget MainWindow::SetCurrentChapter(winrt::Xuanwen::Novel::Chapter const& chapter)
	{
		if (m_chapterIndex == chapter.Index())
			co_return; 

		winrt::apartment_context context; 

		m_chapterIndex = chapter.Index(); 
		this->ChapterTitleTextBlock().Text(chapter.Title());
		this->ChapterContentTextBlock().Text(chapter.Text());
		this->ContentsPopup().IsOpen(false); 

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 }); 
		co_await context; 

		auto height = this->ContentScrollViewer().ScrollableHeight();
		this->ContentScrollViewer().ScrollToVerticalOffset(-height);
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

		if (m_novelBook != nullptr) {
			m_novelBook.Chapters().Clear(); 
		}

		m_novelBook = winrt::Xuanwen::Novel::NovelBook(result.Path()); 
		co_await m_novelBook.InitializeAsync();

		this->BookNameTextBlock().Text(m_novelBook.Name()); 
		
		if (m_novelBook.Chapters().Size() > 0) {
			m_chapterIndex = UINT32_MAX; 
			this->SetCurrentChapter(m_novelBook.Chapters().GetAt(0));
			m_bookContents.SetSelectedIndex(0);
		}
	}


	void MainWindow::OnPreviousChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			return;

		if (m_chapterIndex == 0)
			return; 

		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(m_chapterIndex - 1);
		this->SetCurrentChapter(chapter); 
	}

	void MainWindow::OnNextChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			return;

		auto newChapterIndex = m_chapterIndex + 1; 
		if (newChapterIndex >= m_novelBook.Chapters().Size())
			return;

		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(newChapterIndex);
		this->SetCurrentChapter(chapter); 
	}


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
		float top = (containerSize.y - height) / 2.0f; 

		m_bookContents.Width(width); 
		m_bookContents.Height(height); 
		m_bookContents.SetChapters(m_novelBook.Chapters());
		m_bookContents.SetSelectedIndex(m_chapterIndex); 

		this->ContentsPopup().HorizontalOffset(left); 
		this->ContentsPopup().VerticalOffset(top); 

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 }); 
		co_await context; 
		this->ContentsPopup().IsOpen(true); 
	}

	winrt::fire_and_forget MainWindow::ShowNovelInfo(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			co_return; 

		winrt::apartment_context context; 

		// 计算大小和位置
		winrt::Windows::Foundation::Numerics::float2 containerSize = this->ReaderRegion().ActualSize();
		float width = containerSize.x * 0.5f;
		float height = containerSize.y * 0.4f;
		float left = (containerSize.x - width) / 2.0f;
		float top = (containerSize.y - height) / 2.0f;

		m_novelInfoControl.Width(width);
		m_novelInfoControl.Height(height);
		m_novelInfoControl.SetNovelBook(m_novelBook);

		this->NovelFileInfoPopup().HorizontalOffset(left);
		this->NovelFileInfoPopup().VerticalOffset(top);

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 });
		co_await context;
		this->NovelFileInfoPopup().IsOpen(true);
	}

}




