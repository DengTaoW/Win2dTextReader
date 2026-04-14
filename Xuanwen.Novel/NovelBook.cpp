#include "pch.h"
#include "NovelBook.h"
#include "NovelBook.g.cpp"
#include <uchardet/uchardet.h>
#include <regex>
#include <string>
#include <string_view>

const std::wregex TITLE_REGEX{ LR"(第[\s\d零一二三四五六七八九十百千万]+章)" };

namespace winrt::Xuanwen::Novel::implementation
{
    NovelBook::NovelBook(hstring const& filePath)
        : m_filePath{ filePath }
    {
        m_chapters = winrt::Xuanwen::Novel::ChaptersCollection();
    }

    winrt::Windows::Foundation::IAsyncAction NovelBook::InitializeAsync()
    {
        // 1. 获取 Storage File
        winrt::Windows::Storage::StorageFile novelFile =
            co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(m_filePath);

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

        // 4. 分章
        std::wstring_view contentView{ content }; 
        size_t lineStart = 0; 
        size_t currentPos = 0; 
        int counter = 0; 
        winrt::Xuanwen::Novel::Chapter currentChapter{}; 
        m_chapters.Append(currentChapter); 

        currentChapter.Index(counter); 
        currentChapter.Title(L"前言");


        while (lineStart < content.length())
        {
            size_t lineEnd = content.find_first_of(L'\n', lineStart); 
            if (lineEnd == std::wstring::npos)
                lineEnd = content.length(); 

            std::wstring_view line = contentView.substr(lineStart, lineEnd - lineStart); 
            if (IsTitle(line)) {
                std::wstring_view text = contentView.substr(currentPos, lineStart - currentPos); 
                currentChapter.Text(winrt::hstring(text)); 

                ++counter; 
                currentChapter = winrt::Xuanwen::Novel::Chapter(); 
                m_chapters.Append(currentChapter); 
                currentChapter.Index(counter); 
                currentChapter.Title(winrt::hstring(line)); 

                currentPos = lineStart; 
            }
            lineStart = lineEnd + 1; 
        }

        std::wstring_view text = contentView.substr(currentPos); 
        currentChapter.Text(winrt::hstring(text)); 
    }

    hstring NovelBook::Name()
    {
        return m_bookName; 
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
        size_t sampleSize = (dataLength > 8192ul) ? 8192ul : dataLength; 
        std::string charsetName = DetectCharsetName(data, sampleSize); 

        // 3. 处理 UTF-16LE 字节数组
        if (charsetName == "utf-16le") {
            size_t wcharsCount = dataLength / 2; 
            return std::wstring(reinterpret_cast<const wchar_t*>(data), wcharsCount); 
        }

        // 3. 处理 UTF-16BE 字节数组
        if (charsetName == "utf-16be") {
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

        // 4. 处理边长多字节数组
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

    bool NovelBook::IsTitle(std::wstring_view line)
    {

        if (line.length() <= 12) {
            return std::regex_search(line.begin(), line.end(), TITLE_REGEX); 
        }
        else {
            return std::regex_search(line.begin(), line.begin() + 12, TITLE_REGEX); 
        }
    }
}
