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
		uint32_t m_chapterIndex; 

	public:
		MainWindow();
		void InitializeComponent();
		void SetWindowStyle(); 

		winrt::fire_and_forget SetCurrentChapter(winrt::Xuanwen::Novel::Chapter const& chapter); 
		winrt::fire_and_forget OnOpenButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnPreviousChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		void OnNextChapterButtonClicked(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget ShowContents(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
		winrt::fire_and_forget ShowNovelInfo(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
	};
}

namespace winrt::Win2dTextReader::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
