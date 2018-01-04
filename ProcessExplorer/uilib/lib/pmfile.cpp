
#include <stdio.h>
#include <time.h>

#include "pmfile.h"

namespace uilib {

	PMFile::PMFile()
	{
		m_OpenMode = openmode::notopen;

		m_FileSize = 0;
		m_Buffer = NULL;
		m_BufferSize = 0;
		m_Pos = 0;

		m_CurrLine = 0;
		m_CurrCol = 0;
	}

	PMFile::~PMFile()
	{
		if (m_Buffer)
		{
			delete[] m_Buffer;
			m_Buffer = NULL;
		}
		if (m_file)
			m_file->Close();
	}




	PMFile *PMFile::Open(const char *filename, PMFile::openmode mode, FILEERR *err)
	{
		PMFile *pmf;
		FILEERR currerr;

		pmf = new PMFile;
		if (!pmf)
		{
			if (err)
				*err = FILEERR_OUT_OF_MEMORY;
			return NULL;
		}

		strncpy(pmf->m_Filename, filename, MAX_PATH - 1);
		pmf->m_Filename[MAX_PATH - 1] = '\0';

		currerr = pmf->OpenInternal(mode);
		if (currerr != FILEERR_NONE)
		{
			delete pmf;
			pmf = NULL;
		}

		if (err)
			*err = currerr;

		return pmf;
	}

	//static
	bool PMFile::Delete(const char *filename, FILEERR *err)
	{
		return OSDFile::Delete(filename, err);
	}

	FILEERR PMFile::OpenInternal(PMFile::openmode mode)
	{
		DWORD flags;
		FILEERR currerr;

		if (mode == openmode::write)
			flags = OPENFLAG_WRITE;
		else if (mode == openmode::read)
			flags = OPENFLAG_READ;
		else if (mode == openmode::readwrite)
			flags = OPENFLAG_WRITE | OPENFLAG_READ;
		else if (mode == openmode::create)
			flags = OPENFLAG_WRITE | OPENFLAG_CREATE;
		else
			return FILEERR_ACCESS_DENIED;

		m_OpenMode = mode;

		m_file = OSDFile::Open(m_Filename, flags, &currerr);
		if (currerr == FILEERR_FILE_NOT_FOUND && mode == openmode::write)
		{
			m_OpenMode = openmode::create;
			flags = OPENFLAG_WRITE | OPENFLAG_CREATE;
			m_file = OSDFile::Open(m_Filename, flags, &currerr);
		}

		//pos open
		if (currerr == FILEERR_NONE)
		{
			if (flags & OPENFLAG_READ)
			{
				m_FileSize = m_file->Size();
				m_BufferSize = m_FileSize;

				if (!Reserve((unsigned int)m_BufferSize + 1)) //file size + null
					return FILEERR_OUT_OF_MEMORY;

				m_file->Read(m_Buffer, (unsigned int)m_FileSize);
				m_Pos = 0;
				m_CurrCol = 0; m_CurrLine = 0;
			}

			if (flags & OPENFLAG_WRITE)
				Reserve(MIN_BUFFER_SIZE);

			m_creationtime = m_file->CreationTime();
			if (m_creationtime == 0)
				m_creationtime = time(NULL);
			m_modifytime = m_file->ModifyTime();
			if (m_creationtime == 0)
				m_modifytime = m_creationtime;
		}

		return currerr;
	}


	void PMFile::Close()
	{
		if (m_OpenMode == openmode::write || m_OpenMode == openmode::create)
		{
			m_file->Write(m_Buffer, (DWORD)m_FileSize);
			if (m_OpenMode == openmode::create)
			{
				m_file->SetCreationTime(m_creationtime);
				m_file->SetModifyTime(m_modifytime);
			}
		}
		delete this;
	}



	char PMFile::GetCh()
	{
		if (eof())
			return (char)-1;

		char ch = m_Buffer[m_Pos++];

		if (ch == '\n') {
			++m_CurrLine; m_CurrCol = 0;
		}
		else
			++m_CurrCol;

		return ch;
	}




	void PMFile::forward() {
		if (eof()) return;
		++m_CurrCol;
		if (m_Buffer[m_Pos++] == '\n')
		{
			++m_CurrLine;
			m_CurrCol = 0;
		}

	}

	//todo: to fix
	void PMFile::backward(unsigned int n) {
		if (m_Pos > (n - 1)) {
			if (m_Buffer[m_Pos] == '\n')
				--m_CurrLine;
			else
				--m_CurrCol -= n;
			m_Pos -= n;
		}
		else
		{
			m_Pos = 0;
			m_CurrLine = 0;
			m_CurrCol = 0;
		}
	}


	void PMFile::rewind() //uhu
	{
		m_Pos = 0;
		m_CurrCol = 0; m_CurrLine = 0;
	}


	bool PMFile::eof() { return ((m_Pos + 1) > m_FileSize); }







	bool PMFile::Reserve(unsigned int size)
	{
		if (m_BufferSize > size) return true;

		DWORD addsize = size > MIN_BUFFER_SIZE ? size : MIN_BUFFER_SIZE;
		DWORD buffersize = addsize;

		char *newbuf = new char[buffersize];
		if (!newbuf)
			return false;

		memset(newbuf, 0, (DWORD)buffersize);
		if (m_Buffer)
		{
			memcpy(newbuf, m_Buffer, (DWORD)m_BufferSize);
			delete[] m_Buffer;
		}

		m_Buffer = newbuf;
		m_BufferSize = buffersize;
		return true;
	}




	void PMFile::SetPos(unsigned int pos)
	{
		if ((m_Pos + 1) > m_BufferSize)
			pos -= ((pos + 1) - (DWORD)m_BufferSize);

		m_Pos = pos;

	}



	void PMFile::Write(void *buffer, unsigned int length)
	{
		if (!buffer || length == 0) return;
		if (!Reserve(m_Pos + length)) return;

		memcpy(&m_Buffer[m_Pos], buffer, length);
		m_Pos += length;

		if (m_Pos > m_FileSize)
			m_FileSize = m_Pos;
	}

	void PMFile::Read(void *buffer, unsigned int length)
	{
		if (!buffer || length == 0 || m_Pos == m_FileSize) return;
		if ((m_Pos + length) > m_FileSize)
			length = (DWORD)m_FileSize - m_Pos;

		memcpy(buffer, &m_Buffer[m_Pos], length);
		m_Pos += length;
	}

}











