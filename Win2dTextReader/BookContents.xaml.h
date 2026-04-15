#pragma once
#include "BookContents.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct BookContents : BookContentsT<BookContents>
    {
        BookContents();

        void InitializeComponent(); 

        void SetChapters(winrt::Xuanwen::Novel::ChaptersCollection const& chapters);
        void SetSelectedIndex(uint32_t index);

        winrt::event_token SelectedChapterChanged(winrt::Win2dTextReader::ChapterAction const& handler);
        void SelectedChapterChanged(winrt::event_token const& token) noexcept;
        void ContentsListView_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e);

    private: 
        uint32_t m_totalChapters{ 0 }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Xuanwen::Novel::Chapter> m_chapters{ nullptr }; 
        winrt::event<winrt::Win2dTextReader::ChapterAction> m_selectedChapterChanged; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct BookContents : BookContentsT<BookContents, implementation::BookContents>
    {
    };
}
