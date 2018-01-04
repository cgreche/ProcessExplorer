#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "osdfile.h"

#include <time.h>

namespace uilib {

#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#define INVALID_SET_FILE_POINTER ((DWORD)-1)

	FILEERR OSDFile::CreatePath(const TCHAR *path)
	{
		TCHAR *sep = (TCHAR*)path;

		while (sep = (TCHAR *)_tcschr(sep, TEXT('\\')))
		{
			*sep = TEXT('\0');

			bool success = true;

			// if the path already exists, we're done
			if (GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES)
			{
				// create the path
				success = CreateDirectory(path, NULL) != 0;
			}

			*sep = TEXT('\\');

			if (!success)
				return OSDFile::SystemErrorToFileError(GetLastError());

			sep++;
		}
		return FILEERR_NONE;
	}


	//static
	FILEERR OSDFile::SystemErrorToFileError(DWORD error)
	{
		switch (error)
		{
		case ERROR_SUCCESS:
			return FILEERR_NONE;

		case ERROR_OUTOFMEMORY:
			return FILEERR_OUT_OF_MEMORY;

		case ERROR_FILE_NOT_FOUND:
		case ERROR_PATH_NOT_FOUND:
			return FILEERR_FILE_NOT_FOUND;

		case ERROR_ACCESS_DENIED:
			return FILEERR_ACCESS_DENIED;

		case ERROR_SHARING_VIOLATION:
			return FILEERR_ALREADY_OPEN;

		}

		return FILEERR_UNKNOWN;
	}

	void OSDFile::timet_to_filetime(time_t t, LPFILETIME pft)
	{
		LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
		pft->dwLowDateTime = (DWORD)ll;
		pft->dwHighDateTime = (DWORD)(ll >> 32);
	}

	time_t OSDFile::filetime_to_timet(FILETIME & ft)
	{
		//	ULONGLONG ull = reinterpret_cast<const ULONGLONG&>(ft);
		//	ull -= 116444736000000000;
		//	ull /= 10000000;
		//	return static_cast<time_t>(ull);

		ULARGE_INTEGER ull;
		ull.LowPart = ft.dwLowDateTime;
		ull.HighPart = ft.dwHighDateTime;
		return (time_t)((ull.QuadPart - 116444736000000000) / 10000000);
	}

	OSDFile::OSDFile() :m_handle(0), is_directory(false) { }
	OSDFile::~OSDFile() {  }

	//static
	OSDFile *OSDFile::Open(const char *filename, u32 flags, FILEERR *pErr)
	{
		FILEERR err;
		DWORD access, sharemode, disposition;
		DWORD res;

		access = sharemode = disposition = 0;

		OSDFile *file;
		file = new OSDFile;
		if (!file) {
			if (pErr)
				*pErr = FILEERR_OUT_OF_MEMORY;
			return NULL;
		}

		//simple reading
		if (flags & OPENFLAG_READ) {
			access = GENERIC_READ;
			disposition = OPEN_EXISTING;
			sharemode = FILE_SHARE_READ | FILE_SHARE_WRITE;

			res = GetFileAttributes(filename);
			if (res == INVALID_FILE_ATTRIBUTES) {
				if (pErr) *pErr = SystemErrorToFileError(::GetLastError());
				goto _error;
			}

			if (res & FILE_ATTRIBUTE_DIRECTORY)
			{
				file->is_directory = true;
				sharemode = FILE_SHARE_READ;
				goto _open;
			}
		}

		if (flags & OPENFLAG_WRITE)
		{
			access |= GENERIC_WRITE;
			disposition = OPEN_ALWAYS;
			if (!(flags & OPENFLAG_READ))
				sharemode = 0;
		}

		if (flags & OPENFLAG_CREATE)
		{
			err = CreatePath(filename);
			if (err != FILEERR_NONE) {
				if (pErr) *pErr = err;
				goto _error;
			}

			disposition = CREATE_ALWAYS;
		}

	_open:
		HANDLE handle;
		handle = ::CreateFile(filename, access, sharemode, NULL, disposition, (file->is_directory) ? FILE_FLAG_BACKUP_SEMANTICS : 0, NULL);
		if (handle == INVALID_HANDLE_VALUE) {
			if (pErr) *pErr = SystemErrorToFileError(::GetLastError());
			goto _error;
		}

		file->m_handle = handle;
		if (pErr) *pErr = FILEERR_NONE;
		return file;

	_error:
		if (file)
			delete file;
		return NULL;
	}

	//static
	bool OSDFile::Delete(const char *filename, FILEERR *pErr)
	{
		DWORD error;
		BOOL res;
		res = ::DeleteFileA(filename);
		error = ::GetLastError();

		if (pErr) {
			*pErr = SystemErrorToFileError(error);
		}

		return res != FALSE;
	}

	UINT64 OSDFile::Size()
	{
		DWORD high;
		UINT64 filesize;
		filesize = ::GetFileSize(m_handle, &high);

		if (filesize != INVALID_FILE_SIZE)
			filesize |= (UINT64)high << 32;

		return filesize;
	}

	time_t OSDFile::CreationTime()
	{
		FILETIME ft;

		if (::GetFileTime(m_handle, &ft, NULL, NULL) == FALSE)
			return (time_t)0;

		return filetime_to_timet(ft);
	}

	time_t OSDFile::ModifyTime()
	{
		FILETIME ft;

		if (::GetFileTime(m_handle, NULL, NULL, &ft) == FALSE)
			return (time_t)0;

		return filetime_to_timet(ft);
	}

	void OSDFile::SetCreationTime(time_t time)
	{
		FILETIME ft;
		timet_to_filetime(time, &ft);
		::SetFileTime(m_handle, &ft, NULL, NULL);
	}

	void OSDFile::SetModifyTime(time_t time)
	{
		FILETIME ft;
		timet_to_filetime(time, &ft);
		::SetFileTime(m_handle, NULL, NULL, &ft);
	}


	void OSDFile::Close()
	{
		if (m_handle)
		{
			::CloseHandle(m_handle);
			m_handle = NULL;
		}
		delete this;
	}

	FILEERR OSDFile::Read(void *pBuffer, unsigned int length)
	{
		DWORD total_bytes_read = 0;
		while (total_bytes_read < length)
		{
			DWORD bytes_read;

			BOOL hRes = ::ReadFile(m_handle, (LPVOID)((BYTE*)pBuffer + total_bytes_read), length - total_bytes_read, &bytes_read, NULL);
			if (!hRes || (hRes && bytes_read == 0))
				return SystemErrorToFileError(::GetLastError());


			total_bytes_read += bytes_read;
		}

		return FILEERR_NONE;

	}

	FILEERR OSDFile::Write(const void *pBuffer, unsigned int length)
	{
		DWORD total_bytes_written = 0;
		while (total_bytes_written < length)
		{
			DWORD bytes_written;
			if (::WriteFile(m_handle, (BYTE*)pBuffer + total_bytes_written, length, &bytes_written, NULL) == FALSE)
				return SystemErrorToFileError(::GetLastError());

			total_bytes_written += bytes_written;
		}

		return FILEERR_NONE;
	}


	FILEERR OSDFile::Seek(unsigned __int64 offset)
	{
		LONG upper = (LONG)(offset >> 32);
		DWORD result = SetFilePointer(m_handle, (UINT32)offset, &upper, FILE_BEGIN);
		if (result == INVALID_SET_FILE_POINTER)
		{
			DWORD error = GetLastError();
			if (error != NO_ERROR)
				return SystemErrorToFileError(error);
		}

		return FILEERR_NONE;
	}

}