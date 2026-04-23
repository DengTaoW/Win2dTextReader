#pragma once

#include "MainWindow.g.h"

namespace winrt::Win2dTextReader::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
	private:
		winrt::Win2dTextReader::BookContents m_bookContentsControl{ nullptr }; 
		winrt::Win2dTextReader::Settings m_settingsControl{ nullptr }; 
		winrt::Win2dTextReader::MainWindowViewModel m_viewModel{ nullptr }; 
		winrt::event< winrt::Win2dTextReader::ObjectAction> m_chaptersChanged; 
		uint64_t m_chaptersCounter{ 0 }; 

	public:
		MainWindow();
		void InitializeComponent();
		winrt::fire_and_forget LoadHistoryAsync(); 

		winrt::Win2dTextReader::MainWindowViewModel ViewModel(); 
		winrt::Windows::Foundation::Numerics::float4 GetPopupRegion(float wScale, float hScale); 
		void OnWindowPropertyChanged(winrt::Microsoft::UI::Windowing::AppWindow const& appwindow, winrt::Microsoft::UI::Windowing::AppWindowChangedEventArgs const args);

		winrt::fire_and_forget OnOpenButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnPreviousChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnNextChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		
		void ShowContents(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void ShowSettings(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		
		void ContentScrollView_ViewChanged(winrt::Microsoft::UI::Xaml::Controls::ScrollView const& sender, winrt::Windows::Foundation::IInspectable const& args);
		void OnReaderRegionScrollUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnReaderRegionScrollDown(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		
		void Window_Closed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);

		winrt::event_token ChaptersChanged(winrt::Win2dTextReader::ObjectAction const& handler);
		void ChaptersChanged(winrt::event_token const& token) noexcept;

		void OnViewModelPropertyChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);
	};
}

namespace winrt::Win2dTextReader::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
