#pragma once
#include "ReaderHistoryItem.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct ReaderHistoryItem : ReaderHistoryItemT<ReaderHistoryItem>
    {
        ReaderHistoryItem() = default;

    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct ReaderHistoryItem : ReaderHistoryItemT<ReaderHistoryItem, implementation::ReaderHistoryItem>
    {
    };
}
