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
        winrt::Windows::Foundation::Collections::IVector<winrt::Xuanwen::Novel::Chapter> Chapters();

    private:
        static std::string DetectCharsetName(const char* data, size_t sampleSize); 
        static winrt::hstring DecodeBytesArray(const char* data, size_t dataLength); 
        static bool IsTitle(std::wstring_view line); 
        static bool IsTitle(std::wstring_view line, std::wstring& matchedText); 
        static bool IsEmpty(std::wstring_view text); 

        static bool IsUtf16LE(const char* data, size_t sampleSize); 
        static winrt::hstring DecodeFromUtf16(const char* data, size_t dataLength); 
        static winrt::hstring DecodeFromMultiBytes(const char* data, size_t dataLength, std::string charsetName); 

        void GenerateChaptersTypeB(std::wstring_view content); 
        void GenerateChaptersTypeA(std::wstring_view content);

        hstring m_filePath; 
        hstring m_bookName; 
        uint32_t m_totalChars; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Xuanwen::Novel::Chapter> m_chapters{ nullptr };
    };
}
namespace winrt::Xuanwen::Novel::factory_implementation
{
    struct NovelBook : NovelBookT<NovelBook, implementation::NovelBook>
    {
    };
}
