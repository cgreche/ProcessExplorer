
#include "file.h"
#include "osd\\osdfile.h"

namespace uilib {

	File::File() { }
	File::~File() { }

	//static
	File *File::Open(const char *filename, u32 flags, FILEERR *pErr)
	{
		File *file;
		OSDFile *osdfile;
		osdfile = OSDFile::Open(filename, flags, pErr);
		if (!osdfile)
			return NULL;

		file = new File();
		if (!file) {
			if (pErr)
				*pErr = FILEERR_OUT_OF_MEMORY;

			osdfile->Close();
			return NULL;
		}

		file->m_osdfile = osdfile;
		return file;
	}

	UINT64 File::Size()
	{
		return m_osdfile->Size();
	}

	time_t File::CreationTime()
	{
		return m_osdfile->CreationTime();
	}

	time_t File::ModifyTime()
	{
		return m_osdfile->CreationTime();
	}

	void File::SetCreationTime(time_t time)
	{
		m_osdfile->SetCreationTime(time);
	}

	void File::SetModifyTime(time_t time)
	{
		m_osdfile->SetModifyTime(time);
	}


	void File::Close()
	{
		m_osdfile->Close();
		delete this;
	}

	FILEERR File::Read(void *pBuffer, unsigned int length)
	{
		return m_osdfile->Read(pBuffer, length);
	}

	FILEERR File::Write(const void *pBuffer, unsigned int length)
	{
		return m_osdfile->Write(pBuffer, length);
	}


	FILEERR File::Seek(unsigned __int64 offset)
	{
		return m_osdfile->Seek(offset);
	}

}