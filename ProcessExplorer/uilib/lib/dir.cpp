#pragma comment(lib,"shlwapi.lib")

#include "..\\types.h"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "dir.h"

namespace uilib {

	char CDirectoryManager::dirname[MAX_PATH]("");

	CDirectoryManager::CDirectoryManager() :m_handle(NULL), m_bIsFirst(true)
	{
		memset(m_filter, 0, MAX_PATH);
	}

	CDirectoryManager::~CDirectoryManager()
	{
		Close();
	}

	bool CDirectoryManager::Open(const char *dirname, const char *filter)
	{
		unsigned int filter_size = strlen(dirname) + strlen(filter) + 1; //+1 for '\\'
		if (filter_size > MAX_PATH - 1) return false;
		sprintf(m_filter, "%s\\%s", dirname, filter);
		return true;
	}

	bool CDirectoryManager::Find(dir_entry *entry)
	{
		if (m_bIsFirst)
		{
			m_bIsFirst = false;
			m_handle = FindFirstFile(m_filter, &finddata);
			if (m_handle == INVALID_HANDLE_VALUE) return false;
		}
		else
		{
			if (m_handle && m_handle != INVALID_HANDLE_VALUE)
				if (!FindNextFile(m_handle, &finddata)) return false;
		}

		if (strncmp(finddata.cFileName, ".", 2) == 0 ||
			strncmp(finddata.cFileName, "..", 3) == 0)
		{
			return Find(entry);
		}

		if (entry)
		{
			strncpy(entry->filename, finddata.cFileName, MAX_PATH - 1);
			entry->filesize = finddata.nFileSizeLow | ((UINT64)finddata.nFileSizeHigh << 32);

			if (finddata.dwFileAttributes == 0xffffffff)
				entry->attr = ATTR_INVALID;
			else
				if (finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					entry->attr = ATTR_DIR;
				else
					entry->attr = ATTR_FILE;
		}

		return true;
	}

	void CDirectoryManager::Close()
	{
		if (m_handle && m_handle != INVALID_HANDLE_VALUE)
		{
			FindClose(m_handle);
			m_handle = 0;
		}
	}

	//static
	const char *CDirectoryManager::GetApplicationDirectory()
	{
		u32 max = MAX_PATH - 1;
		::GetModuleFileName(NULL, dirname, max);
		::PathRemoveFileSpec(dirname);
		return (const char *)CDirectoryManager::dirname;
	}

	//static
	const char *CDirectoryManager::GetCurrentDirectory()
	{
		u32 max = MAX_PATH - 1;
		::GetCurrentDirectory(max, (char*)dirname);
		dirname[max] = 0;
		return (const char*)CDirectoryManager::dirname;
	}

	const char *CDirectoryManager::GetCurrentApplicationFilename()
	{
		u32 max = MAX_PATH - 1;
		::GetModuleFileName(0, dirname, max);
		dirname[max] = 0;
		char *p = &dirname[strlen(dirname)];
		while (*--p != '\\');
		return (p + 1);
	}

	//static
	const char *CDirectoryManager::SetCurrentDirectory(const char *dirname, bool force)
	{
		const char *current = CDirectoryManager::GetCurrentDirectory();
		if (FALSE == ::SetCurrentDirectory(dirname)) {
			if (!force) return NULL;
			DWORD err = ::GetLastError();
			//if the directory doesn't exist
			if (err == ERROR_FILE_NOT_FOUND || err == ERROR_PATH_NOT_FOUND) {
				TCHAR *curr = (TCHAR*)dirname;
				while ((curr = _tcschr(curr, '\\')) != '\0') {
					//attempt to create the directory
					*curr = TEXT('\0');
					if (!::CreateDirectory(dirname, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
						*curr = TEXT('\\');
						return NULL;
					}
					*curr = TEXT('\\');
					curr++;
				}
				if (!::CreateDirectory(dirname, NULL))
					return NULL;
				::SetCurrentDirectory(dirname);
			}
			else
				return NULL;
		}
		return current;
	}

	//static
	const char *CDirectoryManager::PathFromFile(const char *filename)
	{
		int len = strlen(filename);
		if (len == 0) return NULL;

		strncpy(CDirectoryManager::dirname, filename, len);
		CDirectoryManager::dirname[len] = '\0';

		char *p = CDirectoryManager::dirname;
		char *p2 = &p[len - 1];

		while (p2 != p) {
			if (*p2 == '\\') {
				*p2 = '\0';
				break;
			}

			--p2;
		}
		return p;
	}

}