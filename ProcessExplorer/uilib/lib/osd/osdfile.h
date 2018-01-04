#ifndef __OSDFILE_H__
#define __OSDFILE_H__

#include <windows.h> //temp
#include <time.h>

#include "..\\file.h"

namespace uilib {

	class OSDFile
	{
		OSDFile();
		virtual ~OSDFile();
	public:

		//OSDFile methods
		static OSDFile *Open(const char *filename, u32 flags, FILEERR *pErr);
		static bool Delete(const char *filename, FILEERR *pErr);

		void Close();

		unsigned __int64 Size();
		time_t CreationTime();
		time_t ModifyTime();
		void SetCreationTime(time_t time);
		void SetModifyTime(time_t time);

		FILEERR Read(void *pBuffer, unsigned int length);
		FILEERR Write(const void *pBuffer, unsigned int length);
		FILEERR Seek(unsigned __int64 offset);

		//windows only
		static FILEERR CreatePath(const char *path);
		static FILEERR SystemErrorToFileError(unsigned long error);
		static time_t filetime_to_timet(FILETIME &ft);
		static void timet_to_filetime(time_t t, LPFILETIME pft);

	protected: //as var
		HANDLE m_handle;
		bool is_directory;
	};

}

#endif