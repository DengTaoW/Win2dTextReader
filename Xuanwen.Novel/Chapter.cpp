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

        constexpr wchar_t FIRST_TITLE[] { L"前言" }; 
        std::wstring_view textView{ m_text }; 
        
        size_t lineEnd = textView.find_first_of(L'\n', 0); 
        if (lineEnd == std::wstring_view::npos) {
            m_title = winrt::hstring{ FIRST_TITLE }; 
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
