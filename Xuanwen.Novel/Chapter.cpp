#include "pch.h"
#include "Chapter.h"
#include "Chapter.g.cpp"

namespace winrt::Xuanwen::Novel::implementation
{
    Chapter::Chapter(uint32_t index, hstring const& title, hstring const& text)
        : m_index{ index }, m_title{ title }, m_text {text}
    {
    }

    uint32_t Chapter::Index() const
    {
        return m_index; 
    }

    void Chapter::Index(uint32_t value)
    {
        m_index = value; 
    }

    hstring Chapter::Title() const
    {
        return m_title; 
    }
    void Chapter::Title(hstring const& value)
    {
        m_title = value; 
    }

    hstring Chapter::Text() const
    {
        return m_text; 
    }

    void Chapter::Text(hstring const& value)
    {
        m_text = value;
    }
}
