#pragma once
#include "Chapter.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct Chapter : ChapterT<Chapter>
    {
        Chapter() = default;

        Chapter(int32_t index, hstring const& title, hstring const& text);
        int32_t Index();
        void Index(int32_t value);
        hstring Title();
        void Title(hstring const& value);
        hstring Text();
        void Text(hstring const& value);

    private:
        int32_t m_index; 
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
