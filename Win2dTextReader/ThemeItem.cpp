#include "pch.h"
#include "ThemeItem.h"
#include "ThemeItem.g.cpp"

namespace winrt::Win2dTextReader::implementation
{
    ThemeItem::ThemeItem(hstring const& xamlName)
        : m_xamlName {xamlName}
    {
    }

    hstring ThemeItem::XamlName()
    {
        return m_xamlName;
    }

    winrt::Windows::Foundation::Uri ThemeItem::Uri() const
    {
        hstring str = L"ms-appx:///Themes/" + m_xamlName; 
        return winrt::Windows::Foundation::Uri(str); 
    }
}
