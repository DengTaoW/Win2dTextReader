#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif


namespace winrt::Win2dTextReader::implementation
{
	MainWindow::MainWindow()
		: m_chapterIndex{ 0 }
	{
	}

	void MainWindow::InitializeComponent()
	{
		MainWindowT::InitializeComponent(); 
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

		m_novelBook = winrt::Xuanwen::Novel::NovelBook(result.Path()); 
		co_await m_novelBook.InitializeAsync();

		this->BookNameTextBlock().Text(m_novelBook.Name()); 
		
		wchar_t novelInfo[30]{ '\0' }; 
		StringCchPrintfW(novelInfo, _countof(novelInfo), L" \t共 %d 章", m_novelBook.Chapters().Size()); 
		this->NovelInfoTextBlock().Text(novelInfo); 

		m_chapterIndex = 0; 
		winrt::Xuanwen::Novel::Chapter firstChapter = m_novelBook.Chapters().GetAt(m_chapterIndex); 
		this->ChapterTitleTextBlock().Text(firstChapter.Title());
		this->FileContentTextBlock().Text(firstChapter.Text()); 
	}


	winrt::fire_and_forget MainWindow::OnPreviousChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			co_return;

		if (m_chapterIndex == 0)
			co_return; 

		winrt::apartment_context context;

		m_chapterIndex--; 
		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(m_chapterIndex);
		this->ChapterTitleTextBlock().Text(chapter.Title());
		this->FileContentTextBlock().Text(chapter.Text());

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 });
		co_await context;

		auto height = this->ContentScrollViewer().ScrollableHeight();
		this->ContentScrollViewer().ScrollToVerticalOffset(-height);
	}

	winrt::fire_and_forget MainWindow::OnNextChapterButtonClicked(
		winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		if (m_novelBook == nullptr)
			co_return;

		auto newChapterIndex = m_chapterIndex + 1; 
		if (newChapterIndex >= m_novelBook.Chapters().Size())
			co_return;

		winrt::apartment_context context;

		m_chapterIndex = newChapterIndex;
		winrt::Xuanwen::Novel::Chapter chapter = m_novelBook.Chapters().GetAt(m_chapterIndex);
		this->ChapterTitleTextBlock().Text(chapter.Title());
		this->FileContentTextBlock().Text(chapter.Text());

		co_await winrt::resume_after(std::chrono::milliseconds{ 100 });
		co_await context;
		

		auto height = this->ContentScrollViewer().ScrollableHeight();
		this->ContentScrollViewer().ScrollToVerticalOffset(-height);
	}
}



