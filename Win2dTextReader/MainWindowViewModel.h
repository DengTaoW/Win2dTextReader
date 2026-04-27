#pragma once
#include "MainWindowViewModel.g.h"


namespace winrt::Win2dTextReader::implementation
{
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel>
    {
        MainWindowViewModel();

        void LoadData();
        void SaveData();
        void OnChaptersChanged(winrt::Windows::Foundation::IInspectable const& obj);

        int32_t ChapterIndex() const;
        void ChapterIndex(int32_t value);

        int32_t WindowPositionX() const;
        void WindowPositionX(int32_t value);

        int32_t WindowPositionY() const;
        void WindowPositionY(int32_t value);

        int32_t WindowSizeW() const;
        void WindowSizeW(int32_t value);

        int32_t WindowSizeH() const;
        void WindowSizeH(int32_t value);

        double ReaderVerticalOffset() const;
        void ReaderVerticalOffset(double value);

        winrt::Xuanwen::Novel::NovelBook CurrentBook();
        void CurrentBook(winrt::Xuanwen::Novel::NovelBook const& value);

        winrt::Xuanwen::Novel::Chapter CurrentChapter();

        double FontSize() const;
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> FontSizeItems(); 
        int32_t FontSizeIndex() const; 
        void FontSizeIndex(int32_t value); 

        double LineHeight() const;
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> LineHeightItems(); 
        int32_t LineHeightIndex() const;
        void LineHeightIndex(int32_t value); 

        winrt::Microsoft::UI::Xaml::Media::FontFamily FontFamily();
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontItem> FontItems(); 
        int32_t FontFamilyIndex() const; 
        void FontFamilyIndex(int32_t value); 

        winrt::Win2dTextReader::AppTheme AppTheme() const; 
        int32_t ThemeIndex() const; 
        void ThemeIndex(int32_t value); 
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppTheme> ThemeItems();

        winrt::event_token PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private: 
        void NotifyPropertyChanged(std::wstring_view name); 
        void InitializeCollections(); 
        void SetDefaultValues();

        int32_t m_chapterIndex{ 0 }; 
        int32_t m_windowPositionX{ 0 }; 
        int32_t m_windowPositionY{ 0 }; 
        int32_t m_windowSizeW{ 0 }; 
        int32_t m_windowSizeH{ 0 }; 
        int32_t m_fontSizeIndex{ 0 };
        int32_t m_lineHeightIndex{ 0 };
        int32_t m_fontFamilyIndex{ 0 };
        int32_t m_themeIndex{ 0 }; 

        double m_readerVerticalOffset{ 0 }; 

        static winrt::hstring INT32_VALUES; 
        static winrt::hstring UINT32_VALUES; 
        static winrt::hstring DOUBLE_VALUES; 
        static winrt::hstring STRING_VALUES; 

        winrt::Xuanwen::Novel::NovelBook m_currentBook{ nullptr }; 
        winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontItem> m_fontItems{ nullptr }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> m_fontSizeItems{ nullptr }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> m_lineHeightItems{ nullptr }; 
        winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppTheme> m_themeItems{ nullptr };
        winrt::Windows::Foundation::Collections::IPropertySet m_localSettings{ nullptr }; 
        
    };
}
namespace winrt::Win2dTextReader::factory_implementation
{
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, implementation::MainWindowViewModel>
    {
    };
}
