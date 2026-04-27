#include "pch.h"
#include "Chapter.h"
#include "Chapter.g.cpp"

namespace winrt::Xuanwen::Novel::implementation
{
    Chapter::Chapter(hstring const& text)
        : m_text {text}
    {
    }

    hstring Chapter::Title()
    {
        if (!m_title.empty())
            return m_title; 

        if (m_text.empty())
            return L"内容为空的章"; 

        std::wstring_view textView{ m_text }; 
        
        size_t lineEnd = textView.find_first_of(L'\n', 0); 
        if (lineEnd == std::wstring_view::npos) {
            size_t length = textView.size() > 12ul ? 12ul : textView.size(); 
            return winrt::hstring{ textView.substr(0, length) };
        }
        else {
            std::wstring result{ textView.substr(0, lineEnd) };
            if (result.back() == L'\r')
                result.pop_back();

            m_title = winrt::hstring{ result }; 
        }

        return m_title; 
    }

    hstring Chapter::Text() const
    {
        return m_text; 
    }

}
