#include "pch.h"
#include "AppResource.h"


namespace winrt::Win2dTextReader
{
	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppFont> AppResource::Fonts()
	{
		auto result = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::AppFont>(); 

		auto resource = winrt::Microsoft::UI::Xaml::Application::Current().Resources(); 
		auto key = winrt::box_value(L"Fonts"); 

		if (resource.HasKey(key)) {
			auto object = resource.Lookup(key); 
			auto themes = object.as<winrt::Microsoft::UI::Xaml::DependencyObjectCollection>(); 

			for (uint32_t i = 0; i < themes.Size(); ++i) {
				auto item = themes.GetAt(i); 
				auto fontItem = item.as<winrt::Win2dTextReader::AppFont>(); 

				result.Append(fontItem); 
			}
		}
		
		return result; 
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppTheme> AppResource::Themes()
	{
		auto result = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::AppTheme>();

		auto resource = winrt::Microsoft::UI::Xaml::Application::Current().Resources();
		auto key = winrt::box_value(L"Themes");

		if (resource.HasKey(key)) {
			auto object = resource.Lookup(key);
			auto themes = object.as<winrt::Microsoft::UI::Xaml::DependencyObjectCollection>();

			for (uint32_t i = 0; i < themes.Size(); ++i) {
				auto item = themes.GetAt(i);
				auto fontItem = item.as<winrt::Win2dTextReader::AppTheme>();

				result.Append(fontItem);
			}
		}

		return result;
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> AppResource::FontSizeValues()
	{
		auto result = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::DoubleItem>(); 

		double fontSize = 10.0;
		for (size_t i = 0; i < 8; i++) {
			winrt::Win2dTextReader::DoubleItem item(fontSize, 0);
			result.Append(item);
			fontSize += 2.0;
		}

		return result; 
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> AppResource::LineHeightValues()
	{
		auto result = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::DoubleItem>(); 

		double lineHeight = 1.0;
		for (size_t i = 0; i < 8; i++) {
			winrt::Win2dTextReader::DoubleItem item(lineHeight, 1);
			result.Append(item);
			lineHeight += 0.2;
		}
		return result; 
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontWeightItem> AppResource::FontWeightValues()
	{
		auto result = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::FontWeightItem>(); 
		result.Append({ L"Light", winrt::Windows::UI::Text::FontWeights::Light() });
		result.Append({ L"SemiLight", winrt::Windows::UI::Text::FontWeights::SemiLight() });
		result.Append({ L"Normal", winrt::Windows::UI::Text::FontWeights::Normal() });
		result.Append({ L"Medium", winrt::Windows::UI::Text::FontWeights::Medium() });
		result.Append({ L"SemiBold", winrt::Windows::UI::Text::FontWeights::SemiBold() });
		result.Append({ L"Bold", winrt::Windows::UI::Text::FontWeights::Bold() });
		result.Append({ L"ExtraBold", winrt::Windows::UI::Text::FontWeights::ExtraBold() });

		return result; 
	}
}
