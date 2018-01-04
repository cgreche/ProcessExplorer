
//Painter
// File name: painter.cpp
// Last edit: 05/12/2015 22:50 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

#include <math.h>

namespace uilib {

	Painter::Painter()
		: m_osdRef(*new OSDPainter(*this)), m_pControl(NULL), m_pDevice(NULL), m_painting(false)
	{
	}

	Painter::Painter(PaintingDevice *pDevice)
		: m_osdRef(*new OSDPainter(*this)), m_pControl(NULL), m_pDevice(NULL), m_painting(false)
	{
		begin(pDevice);
	}

	Painter::Painter(Control *control)
		: m_osdRef(*new OSDPainter(*this)), m_pControl(NULL), m_pDevice(NULL), m_painting(false)
	{
		begin(control);
	}

	Painter::~Painter()
	{
		end();
	}

	bool Painter::begin(PaintingDevice *pDevice)
	{
		if (pDevice) {
			m_painting = true;
			osdRef().begin(pDevice);
			m_pDevice = pDevice;
			return true;
		}

		return false;
	}

	bool Painter::begin(Control *control)
	{
		if (control) {
			m_painting = true;
			osdRef().begin(control);
			m_pControl = control;
			if (control->paintDevice()) //if a paint device was set by OSD, ref us to it
				m_pDevice = control->paintDevice();
			return true;
		}
		return false;
	}

	bool Painter::end()
	{
		if (m_painting) {
			m_painting = false;
			osdRef().end();
			m_pControl = NULL;
			m_pDevice = NULL;
		}
		return true;
	}

	void Painter::clear()
	{
		if (!m_bitmap)
			return;

		int x, y, width, height;
		x = 0;
		y = 0;
		width = m_bitmap->width();
		height = m_bitmap->height();
		fillRect(x, y, width, height, m_backBrush.color());
	}

	/*
	void Painter::setFont(const Font& font)
	{
		m_font = font;
	}
	*/

	void Painter::setPen(const Pen& pen)
	{
		m_pen = pen;
		m_pDevice->setPen(pen);
	}

	void Painter::setPenColor(const Color &color)
	{
		m_pen.setColor(color);
		m_pDevice->setPen(m_pen);
	}

	void Painter::setBrush(const Brush& brush)
	{
		m_brush = brush;
		m_pDevice->setBrush(brush);
	}

	void Painter::setBrushStyle(BrushStyle style)
	{
		m_brush.setStyle(style);
		m_pDevice->setBrush(m_brush);
	}

	void Painter::setBackgroundBrush(const Brush& brush)
	{
		m_backBrush = brush;
		m_pDevice->setBackgroundBrush(brush);
	}

	void Painter::setBackgroundMode(BackgroundMode bgMode)
	{
		m_bgMode = bgMode;
		m_pDevice->setBackgroundMode(bgMode);
	}

	void Painter::setFont(const Font& font)
	{
		m_font = font;
		m_pDevice->setFont(font);
	}

	void Painter::line(int x1, int x2, int y1, int y2)
	{
		m_pDevice->line(x1, x2, y1, y2);
	}

	/*
	static int dotsPattern[2] = {18,6};
	static int numModes = 2;
	static int curDots = 0;
	static int curMode = 0;
	static int lastX = -1;
	static int lastY = -1;

	void Painter::moveTo(int x, int y)
	{
		m_x = x;
		m_y = y;
		curDots = 0;
		curMode = 0;
		lastX = -1;
		lastY = -1;
	}


	void Painter::lineTo(int x, int y)
	{
		int x0, x1, y0, y1;
		int dx, dy;
		int temp;
		int error;

		x0 = m_x;
		x1 = x;
		y0 = m_y;
		y1 = y;

		m_x = x1;
		m_y = y1;

		bool steep = abs(y-y0) > abs(x-x0);

		if(steep) {
			temp = x0;
			x0 = y0;
			y0 = temp;

			temp = x1;
			x1 = y1;
			y1 = temp;
		}

		if(x0 > x1) {
		//	temp = x0;
		//	x0 = x1;
		//	x1 = temp;

		//	temp = y0;
		//	y0 = y1;
		//	y1 = temp;
		}

		dx = abs(x1-x0);
		dy = abs(y1-y0);
		error = dx/2;

		int xstep;
		int ystep;

		if(x0 < x1) {
			xstep = -1;
		}
		else {
			xstep = 1;
		}
		if(y0 < y1) {
			ystep = 1;
		}
		else {
			ystep = -1;
		}

		x = x0;
		y = y0;

		//distance to draw will be the same as deltax
		int xDist = dx;
		while(xDist) {
			--xDist;

			Color color = m_pen.color();

			bool toDraw = false;
			if(x != lastX || y != lastY) {
				while(curDots++ >= dotsPattern[curMode]) {
					if(++curMode >= numModes)
						curMode = 0;
					curDots = 0;
				}
				toDraw = curMode == 0;

				lastX = x;
				lastY = y;
			}

			int dots = curDots;

			if(toDraw) {
				if(steep) {
					plot(y,x,color);
				}
				else {
					plot(x,y,color);
				}
			}

			x -= xstep;
			error -= dy;
			if(error < 0) {
				y += ystep;
				error += dx;
			}
		}
	}
	*/

	void Painter::rectangle(int x, int y, int width, int height)
	{
		/*
		if(width == 0 || height == 0)
			return;

		Color color = m_pen.color();
		Color insideColor = m_brush.color();

		if(width < 0) {
			x = x+width;
			width = -width;
		}

		if(height <0) {
			y = y+height;
			height = -height;
		}

		//draw edges
		s32 i;
		s32 startX, endX, startY, endY;

		startX = x;
		startY = y;
		endX = x+width;
		startY = endY = y;
		for(i = startX; i < endX; ++i)
			plot(i,startY,color);

		startX = endX = x+width-1;
		startY = y;
		endY = y+height;
		for(i = startY; i < endY; ++i)
			plot(startX,i,color);

		startX = x;
		endX = x+width;
		startY = endY = y+height-1;
		for(i = startX; i < endX; ++i)
			plot(i,startY,color);

		startX = endX = x;
		startY = y;
		endY = y+height-1;
		for(i = startY; i < endY; ++i)
			plot(startX,i,color);

		//paint the interior
		endY = y+height-1;
		fillRect(x+1,y+1,width-2,height-2);
		*/

		m_pDevice->rectangle(x, y, width, height);
	}

	void Painter::fillRect(int x, int y, int w, int h, const Color &color)
	{
		/*
		if(w <= 0 || h <= 0)
			return;

		int _y, _x;
		int endX = x+w;
		int endY = y+h;

		u8 *data = m_bitmapData;
		for(_y = y; _y < endY; ++_y) {
			for(_x = x; _x < endX; ++_x) {
	//			*(u32*)m_bitmap->data(_x,_y) = RGB(color.red, color.green, color.blue);
				plot(_x,_y,color);
			}
		}
		*/
		m_pDevice->fillRect(x, y, w, h, color);
	}

	void Painter::drawBitmap(int x, int y, const Bitmap& bitmap)
	{
		m_pDevice->drawBitmap(x, y, bitmap);
	}

	void Painter::plot(uint x, uint y, const Color &color)
	{
		if (x >= m_bitmap->width()) return;
		if (y >= m_bitmap->height()) return;
		//	*(u32*)m_bitmap->data(x,y) = RGB(color.red, color.green, color.blue);
		//	systemPlot(m_bitmapData,x,y,color);
	}

	/*
	void Painter::flip()
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


	void Painter::drawText(int x, int y, const string& text)
	{
		m_pDevice->drawText(x, y, text);
	}


	void Painter::setTransform(const Transform &transform, bool combine)
	{
		if (combine) {
			m_transform.combine(transform);
		}
		else {
			m_transform = transform;
		}

		m_pDevice->setTransform(m_transform);
	}

	void Painter::resetTransform()
	{
		m_transform.reset();
		m_pDevice->setTransform(m_transform);
	}

	void Painter::scale(float sx, float sy)
	{
		m_transform.scale(sx, sy);
		m_pDevice->setTransform(m_transform);
	}

	void Painter::shear(float sh, float sv)
	{
		m_transform.shear(sh, sv);
		m_pDevice->setTransform(m_transform);
	}

	void Painter::rotate(float degrees)
	{
		/*
		float a = degrees*3.1415926f/180.0f;
		float cosa = cosf(a);
		float sina = sinf(a);

	//	float originX = (float)m_width/2;
	//	float originY = (float)m_height/2;

		xForm.eM11 = cosa;
		xForm.eM12 = sina;
		xForm.eM21 = -sina;
		xForm.eM22 = cosa;
		xForm.eDx = originX - cosa*originX + sina*originY;
		xForm.eDy = originY - cosa*originY - sina*originX;
		*/

		m_transform.rotate(degrees);
		m_pDevice->setTransform(m_transform);
	}

	void Painter::translate(float dx, float dy)
	{
		m_transform.translate(dx, dy);
		m_pDevice->setTransform(m_transform);
	}

}