#pragma once

#include "NovelInfoControl.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct NovelInfoControl : NovelInfoControlT<NovelInfoControl>
    {
        NovelInfoControl() = default;
        void SetNovelBook(winrt::Xuanwen::Novel::NovelBook const& book);
    };
}

namespace winrt::Win2dTextReader::factory_implementation
{
    struct NovelInfoControl : NovelInfoControlT<NovelInfoControl, implementation::NovelInfoControl>
    {
    };
}
