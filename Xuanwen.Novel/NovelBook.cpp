#include "pch.h"
#include "NovelBook.h"
#include "NovelBook.g.cpp"
#include <uchardet/uchardet.h>
#include <regex>
#include <string>
#include <string_view>

const std::wregex TITLE_REGEX{ LR"(第[\s\d零一二三四五六七八九十百千万]+章)" };

static constexpr size_t minOf(size_t v1, size_t v2)
{
    return (v1 < v2) ? v1 : v2; 
}


namespace winrt::Xuanwen::Novel::implementation
{
    NovelBook::NovelBook(hstring const& filePath)
        : m_filePath{ filePath }, m_totalChars{0}
    {
        m_chapters = winrt::Xuanwen::Novel::ChaptersCollection();
    }

    winrt::Windows::Foundation::IAsyncAction NovelBook::InitializeAsync()
    {
        // 1. 获取 Storage File
        winrt::Windows::Storage::StorageFile novelFile{ nullptr }; 

        if (m_filePath.starts_with(L"ms-appx")) {
            winrt::Windows::Foundation::Uri uri{ m_filePath }; 
            novelFile = co_await winrt::Windows::Storage::StorageFile::GetFileFromApplicationUriAsync(uri);
        }
        else {
            novelFile = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(m_filePath);
        }


        m_bookName = novelFile.DisplayName(); 

        // 2. 读取字节数组
        winrt::Windows::Storage::Streams::IBuffer buffer =
            co_await winrt::Windows::Storage::FileIO::ReadBufferAsync(novelFile);
        const char* data = reinterpret_cast<const char*>(buffer.data()); 
        size_t dataLength = buffer.Length(); 

        // 3. 解码字节数组
        std::wstring content = DecodeBytesArray(data, dataLength); 
        if (content.empty()) {
            co_return; 
        }
        m_totalChars = static_cast<uint32_t>(content.length()); 

        // 4. 分章
        this->GenerateChapters(content);
    }

    hstring NovelBook::Name() const
    {
        return m_bookName; 
    }

    hstring NovelBook::FilePath() const
    {
        return m_filePath; 
    }

    uint32_t NovelBook::TotalChars() const
    {
        return m_totalChars; 
    }

    winrt::Xuanwen::Novel::ChaptersCollection NovelBook::Chapters()
    {
        return m_chapters; 
    }

    std::string NovelBook::DetectCharsetName(const char* data, size_t sampleSize)
    {
        uchardet_t detector = uchardet_new();
        uchardet_handle_data(detector, data, sampleSize);
        uchardet_data_end(detector);

        std::string charsetName{ uchardet_get_charset(detector) };
        uchardet_delete(detector);

        if (charsetName.empty()) {
            return charsetName;
        }

        auto toLowerChar = [](char c) { return static_cast<char>(::tolower(c)); };
        std::transform(charsetName.begin(), charsetName.end(), charsetName.begin(), toLowerChar);
        return charsetName;
    }

    std::wstring NovelBook::DecodeBytesArray(const char* data, size_t dataLength)
    {
        // 1. 判断数据是否为空
        if (data == nullptr || dataLength == 0) {
            return {}; 
        }

        // 2. 检测编码名称
        const size_t sampleSize = minOf(10240ul, dataLength); 
        std::string charsetName = DetectCharsetName(data, sampleSize); 

        // 3. 解码 UTF-8, UTF-16BE, UTF-16LE, GB2312 编码的字符串，其他编码格式都会返回空字符串。
        if (charsetName.starts_with("utf-16")) {
            return DecodeFromUtf16(data, dataLength); 
        }
        else {
            return DecodeFromMultiBytes(data, dataLength, charsetName); 
        }
    }

    bool NovelBook::IsTitle(std::wstring_view line)
    {
        if (line.length() <= 2) {
            return false; 
        }
        else if (line.length() <= 12) {
            return std::regex_search(line.begin(), line.end(), TITLE_REGEX); 
        }
        else {
            return std::regex_search(line.begin(), line.begin() + 12, TITLE_REGEX); 
        }
    }

    bool NovelBook::IsUtf16LE(const char* data, size_t sampleSize)
    {
        int flags = IS_TEXT_UNICODE_SIGNATURE |
            IS_TEXT_UNICODE_REVERSE_SIGNATURE |
            IS_TEXT_UNICODE_STATISTICS |
            IS_TEXT_UNICODE_REVERSE_STATISTICS;

        if (IsTextUnicode(data, static_cast<int>(sampleSize), &flags)) {
            if (flags & IS_TEXT_UNICODE_SIGNATURE) {
                return true; 
            }

            if (flags & IS_TEXT_UNICODE_STATISTICS) {
                return true;
            }
        }
        return false;
    }

    std::wstring NovelBook::DecodeFromUtf16(const char* data, size_t dataLength)
    {
        if (IsUtf16LE(data, dataLength)) {
            size_t wcharsCount = dataLength / 2;
            return std::wstring(reinterpret_cast<const wchar_t*>(data), wcharsCount);
        }
        else {
            const uint16_t* pData = reinterpret_cast<const uint16_t*>(data);
            size_t wcharsCount = dataLength / 2;
            std::wstring result(wcharsCount, L'\0');

            for (size_t i = 0; i < wcharsCount; i++)
            {
                uint16_t val = pData[i];
                result[i] = static_cast<wchar_t>((val >> 8) | (val << 8));
            }
            return result;
        }
    }

    std::wstring NovelBook::DecodeFromMultiBytes(const char* data, size_t dataLength, std::string charsetName)
    {
        UINT codePage = 0;
        if (charsetName == "utf-8") {
            codePage = CP_UTF8;
        }
        else if (charsetName == "gb18030") {
            codePage = 54936;
        }
        else {
            return {};
        }

        int sizeNeeded = MultiByteToWideChar(
            codePage, 0, data, static_cast<int>(dataLength), nullptr, 0);

        if (sizeNeeded == 0) {
            return {};
        }

        std::wstring result(sizeNeeded, L'\0');
        MultiByteToWideChar(
            codePage, 0, data, static_cast<int>(dataLength), &result[0], sizeNeeded);

        return result;
    }

    void NovelBook::GenerateChapters(std::wstring_view content)
    {
        size_t lineStart = 0;
        size_t lineEnd = 0; 
        size_t chapterStart = 0; 
        size_t chapterEnd = 0; 

        uint32_t chapterIndex = 0;

        while (lineStart < content.length())
        {
            lineEnd = content.find_first_of(L'\n', lineStart + 1);
            if (lineEnd == std::wstring::npos)
                lineEnd = content.length();

            std::wstring_view line = content.substr(lineStart, lineEnd - lineStart);
            if (IsTitle(line)) {

                chapterEnd = lineStart; 
                std::wstring_view chapterText = content.substr(chapterStart, chapterEnd - chapterStart); 
                winrt::Xuanwen::Novel::Chapter chapter{ chapterIndex, winrt::hstring{chapterText} };
                m_chapters.Append(chapter);

                ++chapterIndex; 
                chapterStart = chapterEnd + 1; 
            }
            lineStart = lineEnd;
        }

        std::wstring_view lastChapterText = content.substr(chapterStart);
        winrt::Xuanwen::Novel::Chapter lastChapter{ chapterIndex, winrt::hstring {lastChapterText} }; 
        m_chapters.Append(lastChapter); 
    }
}
