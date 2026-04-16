#include "pch.h"
#include "BookContents.xaml.h"
#include "BookContents.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    BookContents::BookContents()
    {
        m_chapters = winrt::single_threaded_observable_vector<winrt::Xuanwen::Novel::Chapter>(); 
    }

    void BookContents::InitializeComponent()
    {
        BookContentsT::InitializeComponent(); 
        this->ContentsListView().ItemsSource(m_chapters); 
    }

    void BookContents::SetChapters(winrt::Xuanwen::Novel::ChaptersCollection const& chapters)
    {
        m_totalChapters = chapters.Size(); 
        m_chapters.Clear(); 

        for (winrt::Xuanwen::Novel::Chapter const& chapter : chapters) {
            m_chapters.Append(chapter); 
        }
    }

    void BookContents::SetSelectedIndex(uint32_t index)
    {
        if (index >= m_totalChapters)
            return; 

        int32_t chapterIndex = static_cast<int32_t>(index); 
        if (this->ContentsListView().SelectedIndex() == chapterIndex)
            return; 

        this->ContentsListView().SelectedIndex(chapterIndex); 
    }

    winrt::event_token BookContents::SelectedChapterChanged(winrt::Win2dTextReader::ChapterAction const& handler)
    {
        return m_selectedChapterChanged.add(handler); 
    }

    void BookContents::SelectedChapterChanged(winrt::event_token const& token) noexcept
    {
        m_selectedChapterChanged.remove(token); 
    }


    void BookContents::ContentsListView_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return; 

        winrt::Windows::Foundation::IInspectable selectedItem = e.AddedItems().GetAt(0); 
        winrt::Xuanwen::Novel::Chapter chapter = selectedItem.as<winrt::Xuanwen::Novel::Chapter>(); 
        m_selectedChapterChanged(chapter); 
    }
}

