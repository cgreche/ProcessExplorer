
//PaintingDevice
// File name: paintingdevice.cpp
// Last edit: 29/11/2014 12:34 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

#include <math.h>

namespace uilib {

	void systemPlot(u8 *data, u32 x, u32 y, const Color &color);
	/*
	void* createSystemBitmap(u8 **pData, u32 width, u32 height)
	{
		BITMAPV5HEADER bi;
		void *lpBits;
		HBITMAP hBitmap;


		bi.bV5Size           = sizeof(BITMAPV5HEADER);
		bi.bV5Width           = width;
		bi.bV5Height          = -(s32)height;
		bi.bV5Planes = 1;
		bi.bV5BitCount = 32;
		bi.bV5Compression = BI_BITFIELDS;
		// The following mask specification specifies a supported 32 BPP
		// alpha format for Windows XP.
		bi.bV5RedMask   =  0x000000FF;
		bi.bV5GreenMask =  0x0000FF00;
		bi.bV5BlueMask  =  0x00FF0000;
		bi.bV5AlphaMask =  0xFF000000;

		HDC hdc;
		hdc = ::GetDC(NULL);

		// Create the DIB section with an alpha channel.
		hBitmap = ::CreateDIBSection(hdc, (BITMAPINFO *)&bi, DIB_RGB_COLORS,
			(void **)&lpBits, NULL, (DWORD)0);

		if(!hBitmap) {
			DWORD error = ::GetLastError();
			return NULL;
		}

	//	u32 x=0,y;
	//	for(y = 0; y < height; ++y) {
	//		for(x = 0; x < width; ++x) {
	//			systemPlot((u8*)lpBits,x,y,Color(255,255,255));
	//		}
	//	}

		::ReleaseDC(NULL,hdc);

		if(pData)
			*pData = (u8*)lpBits;

		return (void*)hBitmap;
	}

	void destroySystemBitmap(void *systemBitmap)
	{
		DWORD error;
		error = ::DeleteObject((HBITMAP)systemBitmap);
	}

	void systemPlot(u8 *data, u32 x, u32 y, const Color &color)
	{
		if(x < 0 || x >= 640) return;
		if(y < 0 || y >= 480) return;
		u32 pitch = 4;
		data = data + (y*640*pitch)+(x*pitch);
		*(u32*)data = RGB(color.red(),color.green(),color.blue());
	}

	*/





	PaintingDevice::PaintingDevice()
	{
	}

	PaintingDevice::~PaintingDevice()
	{
	}

	/*
	bool PaintingDevice::resize(int width, int height)
	{
		if(m_systemBitmap) {
			destroySystemBitmap(m_systemBitmap);
			m_systemBitmap = NULL;
		}

		if(m_bitmap) {
			delete m_bitmap;
			m_bitmap = NULL;
		}

		m_systemBitmap = createSystemBitmap(&m_bitmapData,width,height);
		if(!m_systemBitmap)
			return false;
		m_bitmap = new Bitmap(width,height);
		return true;
	}
	*/

	/*

	void PaintingDevice::flip()
	{
		HDC hdc = (HDC)m_pDevice;

		if(!m_bitmapData)
			return;


		BITMAPV5HEADER bi;
		void *lpBits;
		ZeroMemory(&bi,sizeof(BITMAPV5HEADER));

		bi.bV5Size           = sizeof(BITMAPV5HEADER);
		bi.bV5Width           = 640;
		bi.bV5Height          = -480;
		bi.bV5Planes = 1;
		bi.bV5BitCount = 32;
		bi.bV5Compression = BI_BITFIELDS;
		// The following mask specification specifies a supported 32 BPP
		// alpha format for Windows XP.
		bi.bV5RedMask   =  0x000000FF;
		bi.bV5GreenMask =  0x0000FF00;
		bi.bV5BlueMask  =  0x00FF0000;
		bi.bV5AlphaMask =  0xFF000000;

		lpBits = (void*)m_bitmapData;

		int deviceW = ::GetDeviceCaps(hdc,HORZRES);
		int deviceH = ::GetDeviceCaps(hdc,VERTRES);

		u32 pitch = 640*4;
		u32 *dest;
		for(u32 y = 0; y < 480; ++y) {
			dest = (u32*)((u8*)lpBits + (y*pitch));
			for(u32 x = 0; x < 640; ++x) {
				*dest = *(u32*)m_bitmap->data(x,y);
				dest++;
			}
		}

		int result =
		::SetDIBitsToDevice(hdc, 		 // hDC
			0,					 // DestX
			0,					 // DestY
			640,		 // nDestWidth
			480,//bi.bV5Height,		 // nDestHeight
			0,					 // SrcX
			0,					 // SrcY
			0,					 // nStartScan
			480,//bi.bV5Height, // nNumScans
			lpBits,			   // lpBits
			(LPBITMAPINFO)&bi, 			 // lpBitsInfo
			DIB_RGB_COLORS);			 // wUsage

		DWORD error = ::GetLastError();
	}

	*/

}