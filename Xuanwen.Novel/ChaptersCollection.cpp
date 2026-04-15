#include "pch.h"
#include "ChaptersCollection.h"
#include "ChaptersCollection.g.cpp"



namespace winrt::Xuanwen::Novel::implementation
{
    ChaptersCollection::ChaptersCollection()
    {
        m_chapters = winrt::single_threaded_vector<winrt::Xuanwen::Novel::Chapter>();
    }

    winrt::Windows::Foundation::Collections::IIterator<winrt::Xuanwen::Novel::Chapter> ChaptersCollection::First()
    {
        return m_chapters.First(); 
    }

    winrt::Xuanwen::Novel::Chapter ChaptersCollection::GetAt(uint32_t index)
    {
        return m_chapters.GetAt(index); 
    }

    uint32_t ChaptersCollection::Size()
    {
        return m_chapters.Size(); 
    }

    winrt::Windows::Foundation::Collections::IVectorView<winrt::Xuanwen::Novel::Chapter> ChaptersCollection::GetView()
    {
        return m_chapters.GetView(); 
    }

    bool ChaptersCollection::IndexOf(winrt::Xuanwen::Novel::Chapter const& value, uint32_t& index)
    {
        return m_chapters.IndexOf(value, index); 
    }

    void ChaptersCollection::SetAt(uint32_t index, winrt::Xuanwen::Novel::Chapter const& value)
    {
        m_chapters.SetAt(index, value); 
    }

    void ChaptersCollection::InsertAt(uint32_t index, winrt::Xuanwen::Novel::Chapter const& value)
    {
        m_chapters.InsertAt(index, value); 
    }

    void ChaptersCollection::RemoveAt(uint32_t index)
    {
        m_chapters.RemoveAt(index); 
    }

    void ChaptersCollection::Append(winrt::Xuanwen::Novel::Chapter const& value)
    {
        m_chapters.Append(value); 
    }

    void ChaptersCollection::RemoveAtEnd()
    {
        m_chapters.RemoveAtEnd(); 
    }

    void ChaptersCollection::Clear()
    {
        m_chapters.Clear(); 
    }

    uint32_t ChaptersCollection::GetMany(uint32_t startIndex, array_view<winrt::Xuanwen::Novel::Chapter> items)
    {
        return m_chapters.GetMany(startIndex, items); 
    }

    void ChaptersCollection::ReplaceAll(array_view<winrt::Xuanwen::Novel::Chapter const> items)
    {
        m_chapters.ReplaceAll(items); 
    }
}
