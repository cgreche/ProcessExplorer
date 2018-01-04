#ifndef __DIR_H__
#define __DIR_H__

#include <windows.h>
#include <shlwapi.h>

namespace uilib {

	enum ENTRY_ATTRIBUTE
	{
		ATTR_INVALID = -1,
		ATTR_FILE = 0,
		ATTR_DIR = 1
	};


	typedef struct st_dir_entry
	{
		char filename[MAX_PATH];
		UINT64 filesize;
		ENTRY_ATTRIBUTE attr;
	}dir_entry;

	class CDirectoryManager
	{
		HANDLE m_handle;
		WIN32_FIND_DATA finddata;					// current raw data

		char m_filter[MAX_PATH];
		bool m_bIsFirst;

		static char dirname[MAX_PATH];
	public:
		CDirectoryManager();
		~CDirectoryManager();
		bool Open(const char *dirname, const char *filter);
		bool Find(dir_entry *entry);
		void Close();

		static const char *GetApplicationDirectory();
		static const char *GetCurrentDirectory();
		static const char *SetCurrentDirectory(const char *dirname, bool force);
		static const char *GetCurrentApplicationFilename();
		static const char *PathFromFile(const char *filename);
	};

}

#endif