#pragma once
#include "ChaptersCollection.g.h"



namespace winrt::Xuanwen::Novel::implementation
{
    struct ChaptersCollection : ChaptersCollectionT<ChaptersCollection>
    {
        ChaptersCollection();

        winrt::Windows::Foundation::Collections::IIterator<winrt::Xuanwen::Novel::Chapter> First();
        winrt::Xuanwen::Novel::Chapter GetAt(uint32_t index);
        uint32_t Size();
        winrt::Windows::Foundation::Collections::IVectorView<winrt::Xuanwen::Novel::Chapter> GetView();
        bool IndexOf(winrt::Xuanwen::Novel::Chapter const& value, uint32_t& index);
        void SetAt(uint32_t index, winrt::Xuanwen::Novel::Chapter const& value);
        void InsertAt(uint32_t index, winrt::Xuanwen::Novel::Chapter const& value);
        void RemoveAt(uint32_t index);
        void Append(winrt::Xuanwen::Novel::Chapter const& value);
        void RemoveAtEnd();
        void Clear();
        uint32_t GetMany(uint32_t startIndex, array_view<winrt::Xuanwen::Novel::Chapter> items);
        void ReplaceAll(array_view<winrt::Xuanwen::Novel::Chapter const> items);

    private:
        winrt::Windows::Foundation::Collections::IVector<winrt::Xuanwen::Novel::Chapter> m_chapters{ nullptr }; 
    };
}
namespace winrt::Xuanwen::Novel::factory_implementation
{
    struct ChaptersCollection : ChaptersCollectionT<ChaptersCollection, implementation::ChaptersCollection>
    {
    };
}
