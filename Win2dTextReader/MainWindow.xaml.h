#pragma once

#include "MainWindow.g.h"

namespace winrt::Win2dTextReader::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
	private:
		winrt::Xuanwen::Novel::NovelBook m_novelBook{ nullptr };
		uint32_t m_chapterIndex; 

	public:
		MainWindow();
		void InitializeComponent();

		winrt::fire_and_forget OnOpenButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget OnPreviousChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget OnNextChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
	};
}

namespace winrt::Win2dTextReader::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
