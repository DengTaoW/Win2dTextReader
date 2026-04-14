#pragma once
#include "NovelBook.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct NovelBook : NovelBookT<NovelBook>
    {
        NovelBook() = default;

        NovelBook(hstring const& filePath);
        winrt::Windows::Foundation::IAsyncAction InitializeAsync();
        hstring Name();
        winrt::Xuanwen::Novel::ChaptersCollection Chapters();

    private:
        static std::string DetectCharsetName(const char* data, size_t sampleSize); 
        static std::wstring DecodeBytesArray(const char* data, size_t dataLength); 
        static bool IsTitle(std::wstring_view line); 

        hstring m_filePath; 
        hstring m_bookName; 
        winrt::Xuanwen::Novel::ChaptersCollection m_chapters{ nullptr }; 
    };
}
namespace winrt::Xuanwen::Novel::factory_implementation
{
    struct NovelBook : NovelBookT<NovelBook, implementation::NovelBook>
    {
    };
}
