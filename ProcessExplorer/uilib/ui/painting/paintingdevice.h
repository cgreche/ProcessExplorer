
//PaintingDevice
// File name: paintingdevice.h
// Last edit: 30/11/2014 04:21 (UTC-3)
// Author: CGR

#ifndef __PAINTINGDEVICE_H__
#define __PAINTINGDEVICE_H__

#include "painter.h"

namespace uilib {

	class Painter;
	class Pen;
	class Brush;
	class Font;
	class Transform;

	class PaintingDevice
	{
	protected:
		Painter *m_curPainter;
		int m_width;
		int m_height;

	public:
		friend class Painter;
		PaintingDevice();
		virtual ~PaintingDevice();

		int width() const { return m_width; }
		int height() const { return m_height; }

		virtual void setPen(const Pen& pen) = 0;
		virtual void setBrush(const Brush& brush) = 0;
		virtual void setBackgroundBrush(const Brush& bgBrush) = 0;
		virtual void setBackgroundMode(BackgroundMode bgMode) = 0;
		virtual void setFont(const Font& font) = 0;
		virtual void setTransform(const Transform& transform) = 0;

		virtual void line(int x1, int y1, int x2, int y2) = 0;
		virtual void rectangle(int x, int y, int width, int height) = 0;
		virtual void circle(int centerX, int centerY, int radius) = 0;
		virtual void ellipse() = 0;
		virtual void fillRect(int x, int y, int width, int height, const Color& color) = 0;
		virtual void drawBitmap(int x, int y, const Bitmap &bitmap) = 0;
		virtual void plot(int x, int y, const Color& color) = 0;
		virtual void drawText(int x, int y, const string &text) = 0;
	};

}

#endif