#include "pch.h"
#include "NovelBook.h"
#include "NovelBook.g.cpp"
#include <uchardet/uchardet.h>
#include <regex>
#include <string>
#include <string_view>
#include <queue>

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
        m_chapters = winrt::single_threaded_observable_vector<winrt::Xuanwen::Novel::Chapter>(); 
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
        winrt::hstring content = DecodeBytesArray(data, dataLength); 
        if (content.empty()) {
            co_return; 
        }
        m_totalChars = static_cast<uint32_t>(content.size()); 

        // 4. 分章
        std::wstring_view contentView{ content }; 
        if (contentView.empty())
            co_return; 

        size_t firstLineEnd = contentView.find_first_of(L'\n', 0); 
        std::wstring_view firstLine = contentView.substr(0, firstLineEnd); 

        if (IsTitle(firstLine)) {
            this->GenerateChaptersTypeA(contentView); 
        }
        else {
            this->GenerateChaptersTypeB(contentView);
        }

        // 5. 判断第一章是否为空，如果是就移除
        if (m_chapters.Size() > 0) {
            winrt::Xuanwen::Novel::Chapter firstChapter = m_chapters.GetAt(0); 
            std::wstring textView{ firstChapter.Text().c_str()};

            if (NovelBook::IsEmpty(textView)) {
                m_chapters.RemoveAt(0); 
            }
        }
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

    winrt::Windows::Foundation::Collections::IVector<winrt::Xuanwen::Novel::Chapter> NovelBook::Chapters()
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

    winrt::hstring NovelBook::DecodeBytesArray(const char* data, size_t dataLength)
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

    bool NovelBook::IsEmpty(std::wstring_view text)
    {
        if (text.empty()) {
            return true; 
        }

        auto isWhiteChar = [](wchar_t c) { return c == L'\r' || c == L'\n' || c == L' '; };
        for (wchar_t c : text) {
            if (isWhiteChar(c) == false) {
                return false; 
            }
        }

        return true; 
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

    winrt::hstring NovelBook::DecodeFromUtf16(const char* data, size_t dataLength)
    {
        uint32_t wcharsCount = static_cast<uint32_t>(dataLength / 2);

        auto builder = winrt::impl::hstring_builder(wcharsCount); 
        wchar_t* outputData = builder.data(); 

        if (IsUtf16LE(data, dataLength)) {
            
            const wchar_t* wData = reinterpret_cast<const wchar_t*>(data); 
            std::copy(wData, wData + wcharsCount, outputData); 
        }
        else {
            const uint16_t* uData = reinterpret_cast<const uint16_t*>(data);

            for (size_t i = 0; i < wcharsCount; i++)
            {
                uint16_t val = uData[i];
                outputData[i] = static_cast<wchar_t>((val >> 8) | (val << 8));
            }
        }

        return builder.to_hstring(); 
    }

    winrt::hstring NovelBook::DecodeFromMultiBytes(const char* data, size_t dataLength, std::string charsetName)
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

        auto builder = winrt::impl::hstring_builder(static_cast<int>(sizeNeeded)); 

        MultiByteToWideChar(
            codePage, 0, data, static_cast<int>(dataLength), builder.data(), sizeNeeded);

        return builder.to_hstring(); 
    }

    void NovelBook::GenerateChaptersTypeB(std::wstring_view content)
    {
        size_t lineStart = 0;
        size_t lineEnd = 0; 
        size_t chapterStart = 0; 
        size_t chapterEnd = 0; 

        while (lineStart < content.length())
        {
            lineEnd = content.find_first_of(L'\n', lineStart + 1);
            if (lineEnd == std::wstring::npos)
                lineEnd = content.length();

            std::wstring_view line = content.substr(lineStart, lineEnd - lineStart);

            if (IsTitle(line)) {
                chapterEnd = lineStart; 
                std::wstring_view chapterText = content.substr(chapterStart, chapterEnd - chapterStart); 
                winrt::Xuanwen::Novel::Chapter chapter{ winrt::hstring{chapterText} };
                m_chapters.Append(chapter);
                chapterStart = chapterEnd + 1; 
            }
            lineStart = lineEnd;
        }

        std::wstring_view lastChapterText = content.substr(chapterStart);
        winrt::Xuanwen::Novel::Chapter lastChapter{  winrt::hstring {lastChapterText} }; 
        m_chapters.Append(lastChapter); 
    }

    void NovelBook::GenerateChaptersTypeA(std::wstring_view content)
    {
        size_t lineStart = 0;
        size_t lineEnd = 0;
        size_t chapterEnd = 0;

        std::queue<size_t> chapterStartQueue; 

        while (lineStart < content.length())
        {
            lineEnd = content.find_first_of(L'\n', lineStart);
            if (lineEnd == std::wstring::npos)
                lineEnd = content.length();

            std::wstring_view line = content.substr(lineStart, lineEnd - lineStart);

            if (IsTitle(line)) {
                chapterStartQueue.push(lineStart); 

                if (chapterStartQueue.size() < 2)
                    goto NEXT;

                chapterEnd = lineStart;
                size_t chapterStart = chapterStartQueue.front(); 
                chapterStartQueue.pop(); 

                std::wstring_view chapterText = content.substr(chapterStart, chapterEnd - chapterStart);
                winrt::Xuanwen::Novel::Chapter chapter{winrt::hstring{chapterText} };
                m_chapters.Append(chapter);
            }
            NEXT:
            lineStart = lineEnd + 1;
        }

        if (chapterStartQueue.empty())
            return; 

        std::wstring_view lastChapterText = content.substr(chapterStartQueue.front());
        winrt::Xuanwen::Novel::Chapter lastChapter{ winrt::hstring { lastChapterText} };
        m_chapters.Append(lastChapter);
    }
}
