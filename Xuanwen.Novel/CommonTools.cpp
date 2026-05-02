#include "pch.h"
#include "CommonTools.h"

namespace winrt::Xuanwen
{
	MappingFile::MappingFile(const wchar_t* filePath)
		: m_pAddr {nullptr}, m_Length {0}, m_filePath {filePath}
	{
		// 1. 打开文件
		HANDLE hFile = ::CreateFileW(
			filePath,
			GENERIC_READ,
			FILE_SHARE_READ,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
		);

		if (hFile == INVALID_HANDLE_VALUE)
			return; 

		// 2. 获取文件大小
		LARGE_INTEGER fileSize;
		if (! ::GetFileSizeEx(hFile, &fileSize)) {
			::CloseHandle(hFile);
			return;
		}

		m_Length = fileSize.QuadPart;

		// 3. 创建文件映射
		HANDLE hMapping = ::CreateFileMappingW(
			hFile,
			nullptr,
			PAGE_READONLY,
			0, 0,
			nullptr
		);

		if (hMapping == nullptr) {
			::CloseHandle(hFile);
			return;
		}

		// 4. 将文件映射到进程的虚拟地址空间
		m_pAddr = ::MapViewOfFile( hMapping, FILE_MAP_READ, 0, 0, 0 );

		::CloseHandle(hMapping);
		::CloseHandle(hFile);
	}

	size_t MappingFile::Length() const
	{
		return static_cast<size_t>(m_Length); 
	}

	const char* MappingFile::Data() const
	{
		return reinterpret_cast<const char*>(m_pAddr);
	}

	std::wstring Xuanwen::MappingFile::FileName() 
	{
		size_t start = m_filePath.find_last_of(L'\\'); 
		if (start == std::wstring::npos) {
			start = 0; 
		}


		size_t end = m_filePath.find_last_of(L'.'); 
		if (end == std::wstring::npos) {
			end = m_filePath.size();
		}
		
		if (end > start) {
			start++; 
			return m_filePath.substr(start, end - start); 
		}

		return m_filePath; 
	}
}

