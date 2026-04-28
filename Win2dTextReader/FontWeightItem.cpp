#include "pch.h"
#include "FontWeightItem.h"
#include "FontWeightItem.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    FontWeightItem::FontWeightItem(hstring const& name, winrt::Windows::UI::Text::FontWeight const& fontWeight)
        : m_name {name}, m_fontWeight {fontWeight}
    {
    }

    hstring FontWeightItem::Name()
    {
        return m_name; 
    }

    winrt::Windows::UI::Text::FontWeight FontWeightItem::FontWeight()
    {
        return m_fontWeight; 
    }
}
