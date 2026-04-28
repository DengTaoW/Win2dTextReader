#pragma once

namespace winrt::Win2dTextReader
{
	class AppResource
	{
	public: 
		static winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppFont> Fonts(); 
		static winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::AppTheme> Themes(); 
		static winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> FontSizeValues(); 
		static winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> LineHeightValues();
		static winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontWeightItem> FontWeightValues();


	};
}


