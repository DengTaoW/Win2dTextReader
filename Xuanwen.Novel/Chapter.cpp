#include "pch.h"
#include "Chapter.h"
#include "Chapter.g.cpp"

namespace winrt::Xuanwen::Novel::implementation
{
    Chapter::Chapter(int32_t index, hstring const& title, hstring const& text)
        : m_index{ index }, m_title{ title }, m_text {text}
    {
    }

    int32_t Chapter::Index()
    {
        return m_index; 
    }

    void Chapter::Index(int32_t value)
    {
        m_index = value; 
    }

    hstring Chapter::Title()
    {
        return m_title; 
    }
    void Chapter::Title(hstring const& value)
    {
        m_title = value; 
    }

    hstring Chapter::Text()
    {
        return m_text; 
    }

    void Chapter::Text(hstring const& value)
    {
        m_text = value;
    }
}
