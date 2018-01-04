
//OSDPaintingDevice
// File: osdpaintingdevice.cpp
// Last edit: 05/12/2015 21:46 (UTC-3)
// Author: CGR

#include <math.h>

#include "..\\..\\uibase.h"

#pragma comment(lib,"Msimg32.Lib")

namespace uilib {

	class PaintingDeviceGDI;

#define __SAFE_DELETE(x) \
	if(x) { ::DeleteObject(x); x = NULL; }


	void PaintingDeviceGDI::__updateBrush()
	{
		HDC hdc = m_handler;
		HBRUSH hBrush;

		if (m_hBrush == NULL) {
			hBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
		else {
			hBrush = m_hBrush;
		}

		::SelectObject(hdc, hBrush);
	}


	PaintingDeviceGDI::PaintingDeviceGDI(HDC handler)
	{
		HDC hdc = handler;
		::SetGraphicsMode(hdc, GM_ADVANCED);

		m_handler = handler;
		m_hPen = NULL;
		m_hBackBrush = NULL;
		m_hBrush = NULL;
	}

	PaintingDeviceGDI::~PaintingDeviceGDI()
	{
		__SAFE_DELETE(m_hPen);
		__SAFE_DELETE(m_hBackBrush);
		__SAFE_DELETE(m_hBrush);
	}

	void PaintingDeviceGDI::setFont(const Font &font)
	{
		::SelectObject(m_handler, font.osdRef().object());
	}

	void PaintingDeviceGDI::setPen(const Pen &pen)
	{
		__SAFE_DELETE(m_hPen);
		HDC hdc = m_handler;
		Color penColor = pen.color();
		int penStyle;
		int penWidth;

		switch (pen.style()) {
		default:
		case SolidPen:
			penStyle = PS_SOLID;
			break;

		case DashedPen:
			penStyle = PS_DASH;
			break;

		case DottedPen:
			penStyle = PS_DOT;
			break;
		}
		penWidth = pen.width();
		m_hPen = ::CreatePen(penStyle, penWidth, RGB(penColor.red(), penColor.green(), penColor.blue()));
		::SelectObject(hdc, m_hPen);

		//Also, use SetTextColor, because the color of the text will be the same as our pen color
		::SetTextColor(hdc, RGB(penColor.red(), penColor.green(), penColor.blue()));
	}

	void PaintingDeviceGDI::setBrush(const Brush &brush)
	{
		__SAFE_DELETE(m_hBrush);
		HDC hdc = m_handler;
		if (brush.style() == SolidBrush) {
			Color brushColor = brush.color();
			m_hBrush = ::CreateSolidBrush(RGB(brushColor.red(), brushColor.green(), brushColor.blue()));
		}
		else if (brush.style() == NullBrush) {
			m_hBrush = NULL;
		}
		__updateBrush();
	}

	void PaintingDeviceGDI::setBackgroundBrush(const Brush& brush)
	{
		__SAFE_DELETE(m_hBackBrush);
		Color brushColor = brush.color();
		m_hBackBrush = ::CreateSolidBrush(RGB(brushColor.red(), brushColor.green(), brushColor.blue()));
	}

	void PaintingDeviceGDI::setBackgroundMode(BackgroundMode bgMode)
	{
		HDC hdc = m_handler;
		int mode = -1;
		if (bgMode == Transparent)
			mode = TRANSPARENT;
		else if (bgMode == Opaque)
			mode = OPAQUE;

		if (mode != -1) {
			::SetBkMode(hdc, mode);
			__updateBrush();
		}
	}

	void PaintingDeviceGDI::line(int x1, int y1, int x2, int y2)
	{
		HDC hdc = m_handler;
		::MoveToEx(hdc, x1, y1, NULL);
		::LineTo(hdc, x2, y2);
	}

	void PaintingDeviceGDI::rectangle(int x, int y, int width, int height)
	{
		//Drawing a rectangle with GDI is buggy
		//if the graphics mode are set as GM_ADVANCED, 1 more pixel is drawn, horizontally and vertically
		HDC hdc = m_handler;
		//lets just substract one for now
		::Rectangle(hdc, x, y, x + width - 1, y + height - 1);
	}

	void PaintingDeviceGDI::circle(int centerX, int centerY, int radius)
	{
	}

	void PaintingDeviceGDI::ellipse()
	{
	}

	void PaintingDeviceGDI::fillRect(int x, int y, int width, int height, const Color& color)
	{
		HDC hdc = m_handler;
		RECT rc = { x,y,x + width,y + height };
		HBRUSH hBrush = ::CreateSolidBrush(RGB(color.red(), color.green(), color.blue()));
		::FillRect(hdc, &rc, hBrush);
		::DeleteObject(hBrush);
	}

	void PaintingDeviceGDI::drawBitmap(int x, int y, const Bitmap& bitmap)
	{
		HDC hdc = m_handler;

		int width = bitmap.width();
		int height = bitmap.height();
		Bitmap::Format format = bitmap.format();

		HDC bmpDC = ::CreateCompatibleDC(hdc);
		HBITMAP hbmp = ::CreateCompatibleBitmap(hdc, width, height);
		::SelectObject(bmpDC, hbmp);

		BITMAPINFO bmi = { 0 };
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		//	HBITMAP hbm = (HBITMAP)::GetCurrentObject(hdc,OBJ_BITMAP);
		int a = ::GetDIBits(bmpDC, hbmp, 0, 0, NULL, &bmi, DIB_RGB_COLORS);
		u8 *bits = NULL;

		bool update = false;

		if (format == Bitmap::Indexed8) {
			Palette *palette = bitmap.palette();
			if (!palette) {
				::DeleteObject(hbmp);
				::DeleteDC(bmpDC);
				return;
			}

			bits = new u8[width * 4 * height];

			int _x, _y;
			int srcPitch = bitmap.pitch();
			int destPitch = width * 4;
			for (_y = 0; _y < height; ++_y) {
				u8 *pSrc = bitmap.data();
				pSrc += _y*srcPitch;
				u8 *pDest = &bits[_y*destPitch];
				for (_x = 0; _x < width; ++_x) {
					Rgb srcColor = palette->getColor(*pSrc);
					DWORD destColor = getR(srcColor) << 16 | getG(srcColor) << 8 | getB(srcColor);
					*(DWORD*)pDest = destColor;
					pDest += 4;
					pSrc += 1;
				}
			}

			update = true;
		}
		else if (format == Bitmap::RGB32) {
			bits = new u8[width * 4 * height];

			int _x, _y;
			for (_y = 0; _y < height; ++_y) {
				for (_x = 0; _x < width; ++_x) {
					u32* pSrc = (u32*)bitmap.data(_x, _y);
					u32 destPitch = width * 4;
					u8* pDest = &bits[(_y*destPitch) + (_x * 4)];
					u32 color = *(u32*)pSrc;
					pDest[0] = (u8)((color >> 0) & 0xff);
					pDest[1] = (u8)((color >> 8) & 0xff);
					pDest[2] = (u8)((color >> 16) & 0xff);
					pDest[3] = 0;
				}
			}

			update = true;
		}

		if (update) {
			bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = 0;
			bmi.bmiHeader.biXPelsPerMeter = 0;
			bmi.bmiHeader.biYPelsPerMeter = 0;
			bmi.bmiHeader.biClrUsed = 0;
			bmi.bmiHeader.biClrImportant = 0;

			bmi.bmiHeader.biWidth = width;
			bmi.bmiHeader.biHeight = -(s32)height;

			u32 res;

			//		res = ::SetDIBitsToDevice(
			//			bmpDC,
			//			x,y,width,height,
			//			0,0,0,height,
			//			bits,&bmi,DIB_RGB_COLORS);

			res = ::SetDIBits(
				bmpDC, hbmp,
				0, height,
				bits, &bmi, DIB_RGB_COLORS);

			DWORD error = ::GetLastError();
		}

		if (bits)
			delete[] bits;

		//	if(bitmap.useColorMask()) {
		//		::TransparentBlt(hdc,x,y,width,height,bmpDC,0,0,width,height,bitmap.colorMask());
		//	}
		//	else {
		::BitBlt(hdc, x, y, width, height, bmpDC, 0, 0, SRCCOPY);
		//	}

		::DeleteObject(hbmp);
		::DeleteDC(bmpDC);


		/*
		BITMAPINFO bi;
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = 32*8;
		bi.bmiHeader.biHeight = -16*8;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biBitCount = 32;
		bi.bmiHeader.biCompression = BI_RGB;
		bi.bmiHeader.biSizeImage = 0;
		bi.bmiHeader.biXPelsPerMeter = 0;
		bi.bmiHeader.biYPelsPerMeter = 0;
		bi.bmiHeader.biClrUsed = 0;
		bi.bmiHeader.biClrImportant = 0;

		::StretchDIBits(hdc,
			0,0,width,height,
			0,0,32*8,16*8,
			bmpData,
			&bi,DIB_RGB_COLORS,SRCCOPY);
		*/
	}

	void PaintingDeviceGDI::plot(int x, int y, const Color &color)
	{
	}

	void PaintingDeviceGDI::drawText(int x, int y, const string& text)
	{
		HDC hdc = m_handler;
		RECT rc = { x,y,x + 100,x + 25 };
		::DrawText(hdc, text.constData(), -1, &rc, DT_LEFT | DT_NOCLIP);
	}

	void PaintingDeviceGDI::setTransform(const Transform& transform)
	{
		HDC hdc = m_handler;
		XFORM xForm;
		xForm.eM11 = transform.m11();
		xForm.eM12 = transform.m12();
		xForm.eM21 = transform.m21();
		xForm.eM22 = transform.m22();
		xForm.eDx = transform.m31();
		xForm.eDy = transform.m32();
		::SetWorldTransform(hdc, &xForm);
	}

}