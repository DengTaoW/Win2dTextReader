#include "pch.h"
#include "Settings.xaml.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif



constexpr wchar_t SETTINGS_KEY[]{ L"ST" }; 
constexpr wchar_t FONT_SIZE_KEY[]{ L"ST:FONT_SIZE" }; 
constexpr wchar_t LINE_HEIGHT_KEY[]{ L"ST:LINE_HEIGHT" }; 
constexpr wchar_t FONT_FAMILY_KEY[]{ L"ST:FONT_FAMILY" };

namespace winrt::Win2dTextReader::implementation
{
    Settings::Settings()
	{
        auto appdata = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault(); 
        m_localSettings = appdata.LocalSettings().Values(); 

        // 初始化字体大小列表
        m_fontSizeValues = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>(); 
        double fontSize = 10.0; 
        for (size_t i = 0; i < 8; i++) {
            hstring str = Format(fontSize, 0); 
            m_fontSizeValues.Append(winrt::box_value(str)); 
            fontSize += 2.0; 
        }

        // 初始化字体高度列表
        m_lineHeightValues = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>(); 
        double lineHeight = 1.2; 
        for (size_t i = 0; i < 6; i++) {
            hstring str = Format(lineHeight, 1); 
            m_lineHeightValues.Append(winrt::box_value(str)); 
            lineHeight += 0.2; 
        }

        // 初始化字体列表
        m_fontDict = {
            {L"宋体", L"SimSun"},
            {L"楷体", L"KaiTi"}, 
            {L"等线", L"DengXian"}, 
            {L"中黑", L"SimHei"},
            {L"雅黑", L"Microsoft YaHei"}
        };

        m_fontFamilyValues = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>(); 
        for (auto& pair : m_fontDict) {
            m_fontFamilyValues.Append(winrt::box_value(pair.first)); 
        }
	}

	void Settings::InitializeComponent()
	{
        SettingsT::InitializeComponent();
        this->FontSizeComboBox().ItemsSource(m_fontSizeValues); 
        this->LineHeightComboBox().ItemsSource(m_lineHeightValues); 
        this->FontFamilyComboBox().ItemsSource(m_fontFamilyValues); 

        if (m_localSettings.HasKey(SETTINGS_KEY)) {
            m_fontSizeIndex = winrt::unbox_value<int32_t>(m_localSettings.Lookup(FONT_SIZE_KEY)); 
            m_lineHeightIndex = winrt::unbox_value<int32_t>(m_localSettings.Lookup(LINE_HEIGHT_KEY)); 
            m_fontFamilyIndex = winrt::unbox_value<int32_t>(m_localSettings.Lookup(FONT_FAMILY_KEY));
            
        }
        else {
            m_fontSizeIndex = 4; 
            m_lineHeightIndex = 3; 
            m_fontFamilyIndex = 0; 
        }


        this->FontSizeComboBox().SelectedIndex(m_fontSizeIndex);
        this->LineHeightComboBox().SelectedIndex(m_lineHeightIndex);
        this->FontFamilyComboBox().SelectedIndex(m_fontFamilyIndex);
	}

    void Settings::Save()
    {
        winrt::Windows::Foundation::IInspectable item{ nullptr }; 

        if (!m_localSettings.HasKey(SETTINGS_KEY)) {
            item = winrt::box_value<int32_t>(1); 
            m_localSettings.Insert(SETTINGS_KEY, item); 
        }

        item = winrt::box_value(m_fontSizeIndex); 
        m_localSettings.Insert(FONT_SIZE_KEY, item); 
        
        item = winrt::box_value(m_lineHeightIndex); 
        m_localSettings.Insert(LINE_HEIGHT_KEY, item); 

        item = winrt::box_value(m_fontFamilyIndex); 
        m_localSettings.Insert(FONT_FAMILY_KEY, item); 
    }

    double Settings::ReaderLineHeight() const
    {
        auto lineHeightScaleStr = winrt::unbox_value<hstring>(m_lineHeightValues.GetAt(m_lineHeightIndex)); 
        auto fontSizeStr = winrt::unbox_value<hstring>(m_fontSizeValues.GetAt(m_fontSizeIndex)); 

        double result = wcstod(lineHeightScaleStr.c_str(), nullptr) * wcstod(fontSizeStr.c_str(), nullptr); 
        return result; 
    }

    double Settings::ReaderFontSize() const
    {
        auto fontSizeStr = winrt::unbox_value<hstring>(m_fontSizeValues.GetAt(m_fontSizeIndex)); 
        
        double result = wcstod(fontSizeStr.c_str(), nullptr); 
        return result; 
    }

    winrt::Microsoft::UI::Xaml::Media::FontFamily Settings::ReaderFontFamily() const
    {
        winrt::hstring fontNameStr = winrt::unbox_value<hstring>(m_fontFamilyValues.GetAt(m_fontFamilyIndex)); 
        winrt::hstring fontSource = (*m_fontDict.find(fontNameStr)).second;
        winrt::Microsoft::UI::Xaml::Media::FontFamily result{ fontSource };
        return result; 
    }

    winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> Settings::FontSizeValues()
    {
        return m_fontSizeValues; 
    }

    winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> Settings::LineHeightValues()
    {
        return m_lineHeightValues; 
    }

    winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable> Settings::FontFamilyValues()
    {
        return m_fontFamilyValues; 
    }

    winrt::event_token Settings::PropertyChanged(
        winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        return m_propertyChanged.add(handler); 
    }

    void Settings::PropertyChanged(winrt::event_token const& token) noexcept
    {
        m_propertyChanged.remove(token); 
    }


    void Settings::FontSizeComboBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return; 
        m_fontSizeIndex = this->FontSizeComboBox().SelectedIndex(); 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontSize"));
    }

    void Settings::LineHeightComboBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return; 
        m_lineHeightIndex = this->LineHeightComboBox().SelectedIndex(); 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderLineHeight"));
    }

    void Settings::FontFamilyComboBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return;

        m_fontFamilyIndex = this->FontFamilyComboBox().SelectedIndex(); 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontFamily"));
    }

    winrt::hstring Settings::Format(double x, int n)
    {
        wchar_t buffer[30]{ L'\0' }; 
        StringCchPrintfW(buffer, _countof(buffer), L"%.*f", n, x);
        return winrt::hstring{buffer};
    }
}






