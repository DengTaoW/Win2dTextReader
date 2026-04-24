#pragma once
#include "DoubleItem.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct DoubleItem : DoubleItemT<DoubleItem>
    {
        DoubleItem() = default;

        DoubleItem(double x, int32_t digits);
        hstring DisplayText();
        double Value() const;

    private:
        double m_value; 
        hstring m_displayText; 
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct DoubleItem : DoubleItemT<DoubleItem, implementation::DoubleItem>
    {
    };
}
