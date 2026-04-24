#pragma once
#include "ThemeItem.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct ThemeItem : ThemeItemT<ThemeItem>
    {
    private:
        hstring m_xamlName; 

    public:
        ThemeItem() = default;

        ThemeItem(hstring const& xamlName);
        hstring XamlName();
        winrt::Windows::Foundation::Uri Uri() const;
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct ThemeItem : ThemeItemT<ThemeItem, implementation::ThemeItem>
    {
    };
}
