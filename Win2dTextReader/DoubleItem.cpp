#include "pch.h"
#include "DoubleItem.h"
#include "DoubleItem.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
    DoubleItem::DoubleItem(double x, int32_t digits)
        : m_value {x}
    {
        wchar_t buffer[20]{ L'\0' }; 
        StringCchPrintfW(buffer, _countof(buffer), L"%.*f", digits, x); 
        m_displayText = winrt::hstring{ buffer }; 
    }

    hstring DoubleItem::DisplayText()
    {
        return m_displayText;
    }

    double DoubleItem::Value()
    {
        return m_value; 
    }
}
