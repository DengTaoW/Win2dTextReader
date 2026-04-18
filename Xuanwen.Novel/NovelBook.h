#pragma once
#include "NovelBook.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct NovelBook : NovelBookT<NovelBook>
    {
        NovelBook() = default;

        NovelBook(hstring const& filePath);
        winrt::Windows::Foundation::IAsyncAction InitializeAsync();
        hstring Name() const;
        hstring FilePath() const; 
        uint32_t TotalChars() const; 
        winrt::Xuanwen::Novel::ChaptersCollection Chapters();

    private:
        static std::string DetectCharsetName(const char* data, size_t sampleSize); 
        static std::wstring DecodeBytesArray(const char* data, size_t dataLength); 
        static bool IsTitle(std::wstring_view line); 

        static bool IsUtf16LE(const char* data, size_t sampleSize); 
        static std::wstring DecodeFromUtf16(const char* data, size_t dataLength); 
        static std::wstring DecodeFromMultiBytes(const char* data, size_t dataLength, std::string charsetName); 

        void GenerateChapters(std::wstring_view content); 

        hstring m_filePath; 
        hstring m_bookName; 
        uint32_t m_totalChars; 
        winrt::Xuanwen::Novel::ChaptersCollection m_chapters{ nullptr }; 
    };
}
namespace winrt::Xuanwen::Novel::factory_implementation
{
    struct NovelBook : NovelBookT<NovelBook, implementation::NovelBook>
    {
    };
}
