#include <windows.h>
#include "..\\..\\types.h"
#include "..\\string.h"

namespace uilib {

	s32 system_copyconvertstr(u8 destencoding, u8 *pDest, u16 size, u8 srcencoding, const u8 *pSrc)
	{
		if (!pSrc) return -1;

		s32 _destreqsize = 0;

		u32 _srcenc;
		u32 _destenc;

		if (srcencoding == destencoding) // no need conversion
		{
			if (srcencoding == ENC_UTF16)
			{
				_destreqsize = (wcslen((const WCHAR*)pSrc) + 1) * sizeof(WCHAR);
			}
			else
			{
				_destreqsize = strlen((const char*)pSrc) + 1;
			}


			if (pDest)
			{
				if (size <= 0 || size > _destreqsize)
					size = (u16)_destreqsize;
				memcpy(pDest, pSrc, size);
			}
			else
				size = (u16)_destreqsize;

			return (s32)size;
		}



		if (srcencoding == ENC_SYSTEM)
			_srcenc = CP_ACP;
		else if (srcencoding == ENC_UTF8)
			_srcenc = CP_UTF8;

		if (destencoding == ENC_SYSTEM)
			_destenc = CP_ACP;
		else if (destencoding == ENC_UTF8)
			_destenc = CP_UTF8;


		if (srcencoding == ENC_UTF16)
		{
			_destreqsize = WideCharToMultiByte(_destenc, 0, (WCHAR*)pSrc, -1, NULL, 0, NULL, NULL);
			if (pDest)
				WideCharToMultiByte(_destenc, 0, (WCHAR*)pSrc, -1, (char*)pDest, _destreqsize, NULL, NULL);
		}
		else
		{
			_destreqsize = MultiByteToWideChar(_srcenc, 0, (const char*)pSrc, -1, NULL, 0);
			if (destencoding == ENC_UTF16) /*SYSTEM CHAR or UTF8 to UTF16*/
			{
				_destreqsize *= sizeof(WCHAR);
				if (pDest)
				{
					if (size <= 0 || size > _destreqsize)
						size = (u16)_destreqsize;
					MultiByteToWideChar(_srcenc, 0, (const char *)pSrc, -1, (WCHAR*)pDest, size / 2);
				}
			}
			else /*SYSTEM CHAR to UTF 8 or UTF8 to SYSTEM CHAR*/
			{
				WCHAR *wstring = new WCHAR[_destreqsize];
				if (!wstring) return -1;
				MultiByteToWideChar(_srcenc, 0, (const char *)pSrc, -1, wstring, _destreqsize);

				_destreqsize = WideCharToMultiByte(_destenc, 0, (WCHAR*)wstring, -1, NULL, 0, NULL, NULL);
				if (pDest)
				{
					if (size <= 0 || size > _destreqsize)
						size = (u16)_destreqsize;
					WideCharToMultiByte(_destenc, 0, (WCHAR*)wstring, -1, (char *)pDest, _destreqsize, NULL, NULL);
				}

				delete[] wstring;
			}

		}

		if (!pDest)
			size = (u16)_destreqsize;

		return (s32)size;
	}

}