#include "pch.h"
#include "NovelInfoControl.xaml.h"
#if __has_include("NovelInfoControl.g.cpp")
#include "NovelInfoControl.g.cpp"
#endif


namespace winrt::Win2dTextReader::implementation
{
    void NovelInfoControl::SetNovelBook(winrt::Xuanwen::Novel::NovelBook const& book)
    {
        this->FileNameLabel().Text(book.Name()); 
        this->FilePathLabel().Text(book.FilePath()); 

        winrt::hstring totalCharsText = winrt::to_hstring(book.TotalChars()); 
        this->TotalCharsLabel().Text(totalCharsText); 

        winrt::hstring totalChaptersText = winrt::to_hstring(book.Chapters().Size()); 
        this->TotalChaptersLabel().Text(totalChaptersText); 
    }
}
