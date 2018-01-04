#ifndef __FILE_H__
#define __FILE_H__

#include "..\\types.h"
#include <time.h>

namespace uilib {

	class OSDFile;

	enum FILEERR
	{
		FILEERR_NONE = 0,
		FILEERR_OUT_OF_MEMORY,
		FILEERR_FILE_NOT_FOUND,
		FILEERR_ACCESS_DENIED,
		FILEERR_ALREADY_OPEN,
		FILEERR_UNKNOWN = -1
	};


#define OPENFLAG_READ 1
#define OPENFLAG_WRITE 2
#define OPENFLAG_CREATE 4

	class File
	{
		File();
		~File();

		OSDFile *m_osdfile;
	public:

		//OSDFile methods
		static File *Open(const char *filename, u32 flags, FILEERR *pErr);
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
	};

}

#endif