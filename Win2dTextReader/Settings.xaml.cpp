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
        : m_lineHeight {1.6}
        , m_fontSize {18.0}
	{
        auto appdata = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault(); 
        m_localSettings = appdata.LocalSettings().Values(); 
	}

	void Settings::InitializeComponent()
	{
		SettingsT::InitializeComponent(); 
        this->SetFontSizeValues(); 
        this->SetLineHeightValues(); 
        m_fontFamily = winrt::Microsoft::UI::Xaml::Media::FontFamily(L"SimSun"); 
	}

    void Settings::SetFontSizeValues()
    {
        m_fontsizeValues = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>();

        for (size_t i = 10; i < 21; i++)
        {
            winrt::hstring str = winrt::to_hstring(i); 
            m_fontsizeValues.Append(winrt::box_value(str));
        }
        this->FontSizeComboBox().ItemsSource(m_fontsizeValues); 
    }

    void Settings::SetLineHeightValues()
    {
        m_lineHeightValues = winrt::single_threaded_observable_vector<winrt::Windows::Foundation::IInspectable>(); 

        double value = 1.2; 
        for (size_t i = 0; i < 6; i++) {
            winrt::hstring str = Settings::Format(value, 1); 
            m_lineHeightValues.Append(winrt::box_value(str)); 
            value += 0.2; 
        }
        this->LineHeightComboBox().ItemsSource(m_lineHeightValues); 
    }

    bool Settings::Load()
    {
        if (!m_localSettings.HasKey(SETTINGS_KEY)) {
            return false; 
        }

        m_fontSize = this->GetValue<double>(FONT_SIZE_KEY); 
        m_lineHeight = this->GetValue<double>(LINE_HEIGHT_KEY); 
        winrt::hstring fontName = this->GetValue<winrt::hstring>(FONT_FAMILY_KEY);
        m_fontFamily = winrt::Microsoft::UI::Xaml::Media::FontFamily(fontName);

        return true; 
    }

    void Settings::Save()
    {
        if (!m_localSettings.HasKey(SETTINGS_KEY)) {
            this->SaveValue<int32_t>(1, SETTINGS_KEY); 
        }

        this->SaveValue<double>(m_fontSize, FONT_SIZE_KEY); 
        this->SaveValue<double>(m_lineHeight, LINE_HEIGHT_KEY); 
        
        if (m_fontFamily != nullptr) {
            this->SaveValue<hstring>(m_fontFamily.Source(), FONT_FAMILY_KEY); 
        }
    }

    void Settings::UpdateUI()
    {
        // FontSize
        winrt::hstring targetStr = Settings::Format(m_fontSize, 0); 
        for (uint32_t i = 0; i < m_fontsizeValues.Size(); i++)
        {
            winrt::Windows::Foundation::IInspectable item = m_fontsizeValues.GetAt(i); 
            winrt::hstring str = winrt::unbox_value<winrt::hstring>(item); 

            if (str == targetStr) {
                this->FontSizeComboBox().SelectedIndex(i); 
                break; 
            }
        }

        // LineHeight
        targetStr = Settings::Format(m_lineHeight, 1); 
        for (uint32_t i = 0; i < m_lineHeightValues.Size(); i++) 
        {
            winrt::Windows::Foundation::IInspectable item = m_lineHeightValues.GetAt(i);
            winrt::hstring str = winrt::unbox_value<winrt::hstring>(item);

            if (str == targetStr) {
                this->LineHeightComboBox().SelectedIndex(i);
                break;
            }
        }

        // FontFamily 
        targetStr = m_fontFamily.Source(); 

        for (uint32_t i = 0; i < this->FontFamilyComboBox().Items().Size(); ++i) {
            winrt::Windows::Foundation::IInspectable item = this->FontFamilyComboBox().Items().GetAt(i);
            auto textBlock = item.as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>();
            if (textBlock.FontFamily().Source() == targetStr) {
                this->FontFamilyComboBox().SelectedIndex(i); 
                break; 
            }
        }
    }

    double Settings::ReaderLineHeight() const
    {
        return m_lineHeight; 
    }
    void Settings::ReaderLineHeight(double value)
    {
        if (m_lineHeight == value)
            return; 

        m_lineHeight = value; 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderLineHeight")); 
    }

    double Settings::ReaderFontSize() const
    {
        return m_fontSize; 
    }
    void Settings::ReaderFontSize(double value)
    {
        if (m_fontSize == value)
            return; 

        m_fontSize = value; 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontSize"));
    }

    winrt::Microsoft::UI::Xaml::Media::FontFamily Settings::ReaderFontFamily() const
    {
        return m_fontFamily; 
    }
    void Settings::ReaderFontFamily(winrt::Microsoft::UI::Xaml::Media::FontFamily const& value)
    {
        if (m_fontFamily == nullptr)
            goto NEXT;

        if (m_fontFamily.Source() == value.Source())
            return; 

        NEXT:
        m_fontFamily = winrt::Microsoft::UI::Xaml::Media::FontFamily(value); 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderFontFamily"));
    }

    winrt::Windows::UI::Color Settings::ReaderForeground() const
    {
        return m_foreground; 
        
    }
    void Settings::ReaderForeground(winrt::Windows::UI::Color const& value)
    {
        if (m_foreground == value)
            return; 
        m_foreground = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderForeground"));
    }

    winrt::Microsoft::UI::Xaml::Media::Brush Settings::ReaderBackground() const
    {
        return m_background; 
    }
    void Settings::ReaderBackground(winrt::Microsoft::UI::Xaml::Media::Brush const& value)
    {
        if (m_background == value)
            return; 
        m_background = value; 
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"ReaderBackground"));

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
        winrt::Windows::Foundation::IInspectable item = e.AddedItems().GetAt(0);
        winrt::hstring str = winrt::unbox_value<winrt::hstring>(item); 

        double value = wcstod(str.c_str(), nullptr); 
        this->ReaderFontSize(value); 
    }

    void Settings::LineHeightComboBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return; 

        winrt::Windows::Foundation::IInspectable item = e.AddedItems().GetAt(0);
        winrt::hstring str = winrt::unbox_value<winrt::hstring>(item);

        double value = wcstod(str.c_str(), nullptr);
        this->ReaderLineHeight(value);
    }

    winrt::hstring Settings::Format(double value, int digits)
    {
        wchar_t buffer[100]{ L'\0' };
        StringCchPrintfW(buffer, _countof(buffer), L"%.*f", digits, value);
        return winrt::hstring{ buffer };
    }

    void Settings::FontFamilyComboBox_SelectionChanged(
        winrt::Windows::Foundation::IInspectable const&, 
        winrt::Microsoft::UI::Xaml::Controls::SelectionChangedEventArgs const& e)
    {
        if (e.AddedItems().Size() == 0)
            return;

        winrt::Windows::Foundation::IInspectable item = e.AddedItems().GetAt(0); 
        auto textBlock = item.as<winrt::Microsoft::UI::Xaml::Controls::TextBlock>(); 
        this->ReaderFontFamily(textBlock.FontFamily());
    }
}






