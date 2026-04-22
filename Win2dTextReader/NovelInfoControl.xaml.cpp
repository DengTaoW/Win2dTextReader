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

        winrt::hstring totalCharsText = ConvertReadableText(book.TotalChars()); 
        this->TotalCharsLabel().Text(totalCharsText); 

        winrt::hstring totalChaptersText = winrt::to_hstring(book.Chapters().Size()); 
        this->TotalChaptersLabel().Text(totalChaptersText); 
    }

    winrt::hstring NovelInfoControl::ConvertReadableText(uint32_t count)
    {
        double x = count; 
        wchar_t unit = L'\0'; 

        if (x >= 1e4) {
            x /= 1e4; 
            unit = L'万';
        }
        else if (x >= 1e3) {
            x /= 1e3; 
            unit = L'千';
        }

        wchar_t buffer[20]{ L'\0' }; 
        StringCchPrintf(buffer, _countof(buffer), L"%.2f ", x); 

        winrt::hstring result{ buffer };
        result = result + winrt::hstring({ unit });
        return result; 
    }
}
