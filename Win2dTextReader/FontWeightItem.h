#pragma once
#include "FontWeightItem.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct FontWeightItem : FontWeightItemT<FontWeightItem>
    {
        FontWeightItem() = default;

        FontWeightItem(hstring const& name, winrt::Windows::UI::Text::FontWeight const& fontWeight);
        hstring Name();
        winrt::Windows::UI::Text::FontWeight FontWeight();

    private:
        hstring m_name; 
        winrt::Windows::UI::Text::FontWeight m_fontWeight; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct FontWeightItem : FontWeightItemT<FontWeightItem, implementation::FontWeightItem>
    {
    };
}
