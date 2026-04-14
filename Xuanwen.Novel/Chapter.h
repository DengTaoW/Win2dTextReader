#pragma once
#include "Chapter.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct Chapter : ChapterT<Chapter>
    {
        Chapter() = default;

        Chapter(uint32_t index, hstring const& title, hstring const& text);
        uint32_t Index() const;
        void Index(uint32_t value);

        hstring Title() const;
        void Title(hstring const& value);

        hstring Text() const;
        void Text(hstring const& value);

    private:
        uint32_t m_index; 
        hstring m_title; 
        hstring m_text; 
    };
}

namespace winrt::Xuanwen::Novel::factory_implementation
{
    struct Chapter : ChapterT<Chapter, implementation::Chapter>
    {
    };
}
