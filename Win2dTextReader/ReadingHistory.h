#pragma once
#include "ReadingHistory.g.h"

namespace winrt::Win2dTextReader::implementation
{
    struct ReadingHistory : ReadingHistoryT<ReadingHistory>
    {
    private:
        hstring m_filePath; 
        int32_t m_windowPositionX; 
        int32_t m_windowPositionY; 
        int32_t m_windowSizeW; 
        int32_t m_windowSizeH; 
        uint32_t m_chapterIndex; 
        double m_verticalOffset; 
        winrt::Windows::Foundation::Collections::IPropertySet m_localSettings{ nullptr }; 

        template<typename T>
        T GetValue(winrt::hstring key)
        {
            winrt::Windows::Foundation::IInspectable object = m_localSettings.Lookup(key); 
            return winrt::unbox_value<T>(object); 
        }

        template<typename T>
        void SaveValue(const T& value, winrt::hstring key)
        {
            winrt::Windows::Foundation::IInspectable object = winrt::box_value(value); 
            m_localSettings.Insert(key, object); 
        }

    public:
        ReadingHistory();

        hstring FilePath() const;
        void FilePath(hstring const& value);

        uint32_t ChapterIndex() const;
        void ChapterIndex(uint32_t value);

        int32_t WindowPositionX() const;
        void WindowPositionX(int32_t value);

        int32_t WindowPositionY() const;
        void WindowPositionY(int32_t value);

        int32_t WindowSizeW() const;
        void WindowSizeW(int32_t value);

        int32_t WindowSizeH() const;
        void WindowSizeH(int32_t value);

        double VerticalOffset() const;
        void VerticalOffset(double value);

        bool Load(); 
        void Save();

        winrt::Xuanwen::Novel::NovelBook LoadUaseges();
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct ReadingHistory : ReadingHistoryT<ReadingHistory, implementation::ReadingHistory>
    {
    };
}
