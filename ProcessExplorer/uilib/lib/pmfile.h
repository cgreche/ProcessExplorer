#ifndef __PMFILE_H__
#define __PMFILE_H__

#include "osd\\osdfile.h"

#include <time.h>

namespace uilib {

#define PMEOF (SHORT)-1

#define MIN_BUFFER_SIZE 1024

	class PMFile
	{
	protected:
		PMFile();
		~PMFile();

	public:

		enum openmode
		{
			notopen = -1,
			read = 0,
			write,
			readwrite,
			create //used like write, but setting timestamps
		};

		static PMFile *Open(const char *filename, openmode mode, FILEERR *err);
		static bool Delete(const char *filename, FILEERR *err);


		void Close();

		char GetCh();

		void forward();
		//todo: to fix
		void backward(unsigned int n = 1);
		void rewind(); //uhu
		bool eof();

	protected:
		OSDFile *m_file;

		openmode m_OpenMode;

		char m_Filename[256];
		UINT64 m_FileSize;
		UINT64 m_BufferSize;

		char *m_Buffer;
		DWORD m_Pos;

		//
		time_t m_creationtime;
		time_t m_modifytime;


		unsigned int m_CurrLine;
		unsigned int m_CurrCol;

		FILEERR OpenInternal(openmode mode);

	public:
		//acessors
		char *FileName() { return m_Filename; }
		UINT64 FileSize() { return m_FileSize; }
		time_t CreationTime() { return m_creationtime; }
		time_t ModifyTime() { return m_modifytime; }
		void SetCreationTime(time_t time) { m_creationtime = time; }
		void SetModifyTime(time_t time) { m_modifytime = time; }

		bool Reserve(unsigned int size);
		void SetPos(unsigned int pos);
		DWORD GetPos() { return m_Pos; }
		char *Buffer() { return m_Buffer; }

		void Write(void *buffer, unsigned int length);
		void WriteString(const char *string, unsigned int count = 0)
		{
			if (!count) count = strlen(string);
			Write((void*)string, count);
		}
		void WriteByte(unsigned char b) { Write((void*)&b, 1); }
		void Write2(short val) { Write((void*)&val, 2); }
		void Write4(long val) { Write((void*)&val, 4); }

		void Read(void *buffer, unsigned int length = 1);
		void Read2(void *number) { Read((void*)number, 4); }
		void Read4(void *number) { Read((void*)number, 4); }


	};

	static inline int is_space(int ch)
	{
		return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
	}

}

#endif