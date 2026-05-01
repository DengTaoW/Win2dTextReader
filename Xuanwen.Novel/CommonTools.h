#pragma once
#include <type_traits>
#include <concepts>

namespace winrt::Xuanwen
{
	template<typename T1, typename T2>
	auto minOf(const T1& t1, const T2& t2)  -> decltype(t1 + t2)
	{
		return t1 <= t2 ? t1 : t2; 
	}

	template<std::totally_ordered T>
	T minOf(std::initializer_list<T> values)
	{
		if (values.size() == 0) {
			throw std::invalid_argument("values is empty.");
		}

		return std::ranges::min(values.begin(), values.end()); 
	}

	template<typename T1, typename T2>
	auto maxOf(const T1& t1, const T2& t2) -> decltype(t1 + t2)
	{
		return t1 <= t2 ? t2 : t1; 
	}

	template<std::totally_ordered T>
	T maxOf(std::initializer_list<T> values)
	{
		if (values.size() == 0) {
			throw std::invalid_argument("values is empty."); 
		}

		return std::ranges::max(values.begin(), values.end()); 
	}


	class MappingFile
	{
	public: 
		MappingFile(const wchar_t* filePath); 
		//~MappingFile(); 

		size_t Length() const; 
		const char* Data() const; 
		std::wstring FileName(); 

	private:
		void* m_pAddr{ nullptr }; 
		long long m_Length{ 0 }; 
		std::wstring m_filePath; 
	};

}



