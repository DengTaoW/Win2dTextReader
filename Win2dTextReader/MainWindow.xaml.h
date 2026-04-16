#pragma once

#include "MainWindow.g.h"

namespace winrt::Win2dTextReader::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
	private:
		winrt::Xuanwen::Novel::NovelBook m_novelBook{ nullptr };
		winrt::Win2dTextReader::BookContents m_bookContents{ nullptr }; 
		winrt::Win2dTextReader::NovelInfoControl m_novelInfoControl{ nullptr }; 
		winrt::Win2dTextReader::ReadingHistory m_readingHistory{ nullptr }; 
		bool m_isReadingHistoryValid; 

	public:
		MainWindow();
		void InitializeComponent();
		void SetWindowStyle(); 
		void OnWindowPropertyChanged(winrt::Microsoft::UI::Windowing::AppWindow const& appwindow, winrt::Microsoft::UI::Windowing::AppWindowChangedEventArgs const args);
		winrt::Windows::Foundation::IAsyncAction ReadBookAsync(winrt::hstring filePath, bool isNewBook); 

		winrt::fire_and_forget SetCurrentChapter(winrt::Xuanwen::Novel::Chapter const& chapter); 
		winrt::fire_and_forget OnOpenButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnPreviousChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnNextChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget ShowContents(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget ShowNovelInfo(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void Window_Closed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowEventArgs const& args);
		void Window_Activated(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::WindowActivatedEventArgs const& args);
	};
}

namespace winrt::Win2dTextReader::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
