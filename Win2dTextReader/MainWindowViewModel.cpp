#include "pch.h"
#include "MainWindowViewModel.h"
#include "MainWindowViewModel.g.cpp"


namespace winrt::Win2dTextReader::implementation
{
	winrt::hstring MainWindowViewModel::INT32_VALUES{ L"INT32_VALUES" }; 
	winrt::hstring MainWindowViewModel::UINT32_VALUES{ L"UINT32_VALUES" }; 
	winrt::hstring MainWindowViewModel::DOUBLE_VALUES{ L"DOUBLE_VALUES" }; 
	winrt::hstring MainWindowViewModel::STRING_VALUES{ L"STRING_VALUES" }; 


	MainWindowViewModel::MainWindowViewModel()
	{
		m_fontItems = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::FontItem>();
		m_lineHeightItems = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::DoubleItem>(); 
		m_fontSizeItems = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::DoubleItem>(); 
		m_themeItems = winrt::single_threaded_observable_vector<winrt::Win2dTextReader::ThemeItem>(); 

		this->InitializeCollections();

		auto appData = winrt::Microsoft::Windows::Storage::ApplicationData::GetDefault();
		m_localSettings = appData.LocalSettings().Values();

		if (m_localSettings.HasKey(INT32_VALUES)) {
			this->LoadData(); 
			this->SetAppTheme(m_themeIndex); 
		}
		else {
			this->SetDefaultValues(); 
		}
	}

	void MainWindowViewModel::LoadData()
	{
		if (m_localSettings.HasKey(INT32_VALUES)) {
			auto int32ValuesObject = 
				m_localSettings.Lookup(INT32_VALUES).as<winrt::Windows::Foundation::IReferenceArray<int32_t>>();
			winrt::com_array<int32_t> int32Values; 
			int32ValuesObject.GetInt32Array(int32Values); 

			m_windowPositionX = int32Values[0]; 
			m_windowPositionY = int32Values[1]; 
			m_windowSizeW = int32Values[2]; 
			m_windowSizeH = int32Values[3];
			m_chapterIndex = int32Values[4]; 
			m_lineHeightIndex = int32Values[5]; 
			m_fontSizeIndex = int32Values[6]; 
			m_fontFamilyIndex = int32Values[7]; 
			m_themeIndex = int32Values[8]; 
		}

		if (m_localSettings.HasKey(STRING_VALUES)) {
			auto stringValuesObject =
				m_localSettings.Lookup(STRING_VALUES).as < winrt::Windows::Foundation::IReferenceArray<winrt::hstring>>(); 
			winrt::com_array<winrt::hstring> stringValues; 
			stringValuesObject.GetStringArray(stringValues); 

			m_currentBook = winrt::Xuanwen::Novel::NovelBook(stringValues[0]); 
		}

		if (m_localSettings.HasKey(DOUBLE_VALUES)) {
			auto doubleValuesObject =
				m_localSettings.Lookup(DOUBLE_VALUES).as<winrt::Windows::Foundation::IReferenceArray<double>>(); 
			winrt::com_array<double> doubleValues; 
			doubleValuesObject.GetDoubleArray(doubleValues); 

			m_readerVerticalOffset = doubleValues[0]; 
		}
	}
	void MainWindowViewModel::SaveData()
	{
		std::vector<int32_t> int32Values = {
			m_windowPositionX,
			m_windowPositionY,
			m_windowSizeW, 
			m_windowSizeH, 
			m_chapterIndex,
			m_lineHeightIndex,
			m_fontSizeIndex,
			m_fontFamilyIndex,
			m_themeIndex
		};

		auto int32ValuesObject = winrt::Windows::Foundation::PropertyValue::CreateInt32Array(int32Values); 
		m_localSettings.Insert(INT32_VALUES, int32ValuesObject); 

		std::vector<hstring> stringValues = {
			m_currentBook.FilePath()
		}; 

		auto stringValuesObject = winrt::Windows::Foundation::PropertyValue::CreateStringArray(stringValues);
		m_localSettings.Insert(STRING_VALUES, stringValuesObject); 

		
		std::vector<double> doubleValues = {
			m_readerVerticalOffset
		}; 

		auto doubleValuesObject = winrt::Windows::Foundation::PropertyValue::CreateDoubleArray(doubleValues); 
		m_localSettings.Insert(DOUBLE_VALUES, doubleValuesObject);
	}

	void MainWindowViewModel::OnChaptersChanged(winrt::Windows::Foundation::IInspectable const& obj)
	{
		bool isNewBook = winrt::unbox_value<bool>(obj); 
		if (isNewBook) {
			m_chapterIndex = 0; 
		}

		if (m_currentBook != nullptr) {
			this->NotifyPropertyChanged(L"CurrentChapter"); 
			this->NotifyPropertyChanged(L"CurrentBook"); 
		}
	}

	int32_t MainWindowViewModel::ChapterIndex() const
	{
		return m_chapterIndex;
	}
	void MainWindowViewModel::ChapterIndex(int32_t value)
	{
		int32_t totalChapters = m_currentBook.Chapters().Size(); 
		if (value >= totalChapters || value < 0)
			return; 

		if (value != m_chapterIndex) {
			m_chapterIndex = value;
			this->NotifyPropertyChanged(L"CurrentChapter");
		}
	}

	int32_t MainWindowViewModel::WindowPositionX() const
	{
		return m_windowPositionX;
	}
	void MainWindowViewModel::WindowPositionX(int32_t value)
	{
		if (value > 0) {
			m_windowPositionX = value;
		}
	}

	int32_t MainWindowViewModel::WindowPositionY() const
	{
		return m_windowPositionY;
	}
	void MainWindowViewModel::WindowPositionY(int32_t value)
	{
		if (value > 0) {
			m_windowPositionY = value;
		}
	}

	int32_t MainWindowViewModel::WindowSizeW() const
	{
		return m_windowSizeW;
	}
	void MainWindowViewModel::WindowSizeW(int32_t value)
	{
		if (value > 300) {
			m_windowSizeW = value;
		}
	}

	int32_t MainWindowViewModel::WindowSizeH() const
	{
		return m_windowSizeH;
	}
	void MainWindowViewModel::WindowSizeH(int32_t value)
	{
		if (value > 300) {
			m_windowSizeH = value;
		}
	}

	double MainWindowViewModel::ReaderVerticalOffset() const
	{
		return m_readerVerticalOffset;
	}
	void MainWindowViewModel::ReaderVerticalOffset(double value)
	{
		if (value >= 0) {
			m_readerVerticalOffset = value;
		}
	}

	winrt::Xuanwen::Novel::NovelBook MainWindowViewModel::CurrentBook()
	{
		return m_currentBook;
	}
	void MainWindowViewModel::CurrentBook(winrt::Xuanwen::Novel::NovelBook const& value)
	{
		if (value != m_currentBook) {
			m_currentBook = value; 
			this->NotifyPropertyChanged(L"CurrentBook"); 
		}
	}

	winrt::Xuanwen::Novel::Chapter MainWindowViewModel::CurrentChapter()
	{
		if (m_currentBook == nullptr) {
			return nullptr;
		}

		auto totalChapters = static_cast<int>(m_currentBook.Chapters().Size());
		if (totalChapters == 0) {
			return nullptr;
		}
		if (m_chapterIndex >= totalChapters) {
			m_chapterIndex = 0;
		}

		return m_currentBook.Chapters().GetAt((uint32_t)m_chapterIndex);
	}

	double MainWindowViewModel::LineHeight() const
	{
		winrt::Win2dTextReader::DoubleItem item = m_lineHeightItems.GetAt((uint32_t)m_lineHeightIndex);
		double result = item.Value() * this->FontSize();
		return result; 
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> MainWindowViewModel::LineHeightItems()
	{
		return m_lineHeightItems;
	}

	int32_t MainWindowViewModel::LineHeightIndex() const
	{
		return m_lineHeightIndex;
	}

	void MainWindowViewModel::LineHeightIndex(int32_t value)
	{
		if (m_lineHeightIndex != value) {
			m_lineHeightIndex = value;
			this->NotifyPropertyChanged(L"LineHeight");
		}
	}

	double MainWindowViewModel::FontSize() const
	{
		winrt::Win2dTextReader::DoubleItem item = m_fontSizeItems.GetAt((uint32_t)m_fontSizeIndex);
		return item.Value();
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::DoubleItem> MainWindowViewModel::FontSizeItems()
	{
		return m_fontSizeItems;
	}

	int32_t MainWindowViewModel::FontSizeIndex() const
	{
		return m_fontSizeIndex;
	}

	void MainWindowViewModel::FontSizeIndex(int32_t value)
	{
		if (value != m_fontSizeIndex) {
			m_fontSizeIndex = value;
			this->NotifyPropertyChanged(L"FontSize");
		}
	}

	winrt::Microsoft::UI::Xaml::Media::FontFamily MainWindowViewModel::FontFamily()
	{
		winrt::Win2dTextReader::FontItem item = m_fontItems.GetAt((uint32_t)m_fontFamilyIndex);
		return item.FontFamily();
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::FontItem> MainWindowViewModel::FontItems()
	{
		return m_fontItems;
	}

	int32_t MainWindowViewModel::FontFamilyIndex() const
	{
		return m_fontFamilyIndex;
	}

	void MainWindowViewModel::FontFamilyIndex(int32_t value)
	{
		if (m_fontFamilyIndex != value) {
			m_fontFamilyIndex = value;
			this->NotifyPropertyChanged(L"FontFamily");
		}
	}

	int32_t MainWindowViewModel::ThemeIndex() const
	{
		return m_themeIndex; 
	}

	void MainWindowViewModel::ThemeIndex(int32_t value)
	{
		if (value == m_themeIndex) {
			return; 
		}

		m_themeIndex = value; 
		this->SetAppTheme(m_themeIndex); 
		this->NotifyPropertyChanged(L"ThemeIndex"); 
	}

	winrt::Windows::Foundation::Collections::IVector<winrt::Win2dTextReader::ThemeItem> MainWindowViewModel::ThemeItems()
	{
		return m_themeItems;
	}


	winrt::event_token MainWindowViewModel::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
	{
		return m_propertyChanged.add(handler);
	}
	void MainWindowViewModel::PropertyChanged(winrt::event_token const& token) noexcept
	{
		m_propertyChanged.remove(token);
	}

	void MainWindowViewModel::NotifyPropertyChanged(std::wstring_view name)
	{
		m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(name));
	}

	void MainWindowViewModel::InitializeCollections()
	{
		double fontSize = 10.0;
		for (size_t i = 0; i < 8; i++) {
			winrt::Win2dTextReader::DoubleItem item(fontSize, 0);
			m_fontSizeItems.Append(item);
			fontSize += 2.0; 
		}

		double lineHeight = 1.2;
		for (size_t i = 0; i < 7; i++) {
			winrt::Win2dTextReader::DoubleItem item(lineHeight, 1);
			m_lineHeightItems.Append(item);
			lineHeight += 0.2; 
		}

		std::map<std::wstring, std::wstring> fontNamesDict = {
			{L"宋体", L"SimSun"},
			{L"楷体", L"KaiTi"},
			{L"等线", L"DengXian"},
			{L"中黑", L"SimHei"},
			{L"雅黑", L"Microsoft YaHei"}
		};

		for (const auto& [displayName, fontName] : fontNamesDict) {
			winrt::Win2dTextReader::FontItem item(fontName, displayName); 
			m_fontItems.Append(item); 
		}

		std::vector<hstring> themeXamlFiles = {
			L"Dark.xaml", 
			L"Foreset.xaml",
			L"Ocean.xaml", 
			L"Sakura.xaml",
			L"SistersSoft.xaml"
		};

		for (const auto& file : themeXamlFiles) {
			winrt::Win2dTextReader::ThemeItem item{ file };
			m_themeItems.Append(item); 
		}
	}

	void MainWindowViewModel::SetDefaultValues()
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN); 
		int screenHeight = GetSystemMetrics(SM_CYSCREEN); 

		m_windowSizeW = static_cast<int>(screenWidth * 0.5); 
		m_windowSizeH = static_cast<int>(screenHeight * 0.7);

		m_windowPositionX = (screenWidth - m_windowSizeW) / 2; 
		m_windowPositionY = (screenHeight - m_windowSizeH) / 2; 

		m_chapterIndex = 0; 
		m_fontSizeIndex = 4; 
		m_lineHeightIndex = 3;
		m_readerVerticalOffset = 0; 

		m_fontFamilyIndex = 0; 
		m_themeIndex = 0;
		m_currentBook = winrt::Xuanwen::Novel::NovelBook(L"ms-appx:///Assets/使用说明.txt");
	}

	void MainWindowViewModel::SetAppTheme(int32_t themeIndex)
	{
		winrt::Win2dTextReader::ThemeItem item = m_themeItems.GetAt(themeIndex);

		auto resources = winrt::Microsoft::UI::Xaml::Application::Current().Resources();
		auto mergedDicts = resources.MergedDictionaries();

		winrt::Microsoft::UI::Xaml::ResourceDictionary newThemeDict;
		newThemeDict.Source(item.Uri());

		if (mergedDicts.Size() == 2) {
			mergedDicts.SetAt(1, newThemeDict); 
		}
	}
}
