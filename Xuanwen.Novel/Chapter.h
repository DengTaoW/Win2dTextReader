#pragma once
#include "Chapter.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct Chapter : ChapterT<Chapter>
    {
        Chapter() = default;

        Chapter(uint32_t index, hstring const& text);

        uint32_t Index() const;
        hstring Title();
        hstring Text() const;

    private:
        uint32_t m_index{};
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
