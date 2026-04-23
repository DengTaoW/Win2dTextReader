#pragma once
#include "Chapter.g.h"


namespace winrt::Xuanwen::Novel::implementation
{
    struct Chapter : ChapterT<Chapter>
    {
        Chapter() = default;

        Chapter(hstring const& text);

        hstring Title();
        hstring Text() const;

    private:
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
