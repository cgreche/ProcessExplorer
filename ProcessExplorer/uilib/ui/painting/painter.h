
//Painter
// File name: painter.h
// Last edit: 05/12/2015 23:53 (UTC-3)
// Author: CGR

#ifndef __PAINTER_H__
#define __PAINTER_H__

#include "paintingdevice.h"

namespace uilib {

	class PaintingDevice;
	class Control;

	enum BrushStyle
	{
		SolidBrush,
		NullBrush,
	};

	class Brush
	{
		BrushStyle m_style;
		Color m_color;

	public:
		Brush() :m_color(0, 0, 0), m_style(SolidBrush) { }
		Brush(BrushStyle style) : m_style(style) { }
		Brush(const Color& color) : m_color(color), m_style(SolidBrush) { }

		void setStyle(BrushStyle style) { m_style = style; }
		BrushStyle style() const { return m_style; }

		void setColor(const Color &color) { m_color = color; }
		Color color() const { return m_color; }
	};

	enum PenStyle
	{
		SolidPen,
		DashedPen,
		DottedPen,
		NullPen
	};

	class Pen
	{
		PenStyle m_style;
		int m_width;
		Color m_color;

	public:
		Pen(int width, const Color& color) : m_width(width), m_color(color) { }
		Pen() : m_style(SolidPen), m_width(0), m_color(0, 0, 0) { }

		void setStyle(PenStyle style) { m_style = style; }
		PenStyle style() const { return m_style; }

		void setWidth(int width) { m_width = width; }
		int width() const { return m_width; }

		void setColor(const Color &color) { m_color = color; }
		Color color() const { return m_color; }
	};


	enum BackgroundMode
	{
		Transparent,
		Opaque,
	};


	class Painter
	{
		OSDPainter &m_osdRef;

	protected:
		Control *m_pControl; //if we are painting on a control, this reference is valid
		PaintingDevice *m_pDevice; //
		bool m_painting;

		Pen m_pen;
		Brush m_backBrush;
		Brush m_brush;
		Font m_font;
		int m_x;
		int m_y;

		Bitmap *m_bitmap;

		BackgroundMode m_bgMode;

		void *m_systemBitmap;
		u8 *m_bitmapData;

		Transform m_transform;

	public:
		Painter();
		Painter(PaintingDevice *pDevice);
		Painter(Control *control);
		~Painter();
		friend class OSDEdit;
		OSDPainter& osdRef() const { return m_osdRef; }

		Control *control() const { return m_pControl; }
		PaintingDevice *device() const { return m_pDevice; }
		BackgroundMode backgroundMode() const { return m_bgMode; }
		Transform currentTransform() const { return m_transform; }

		bool begin(Control *control);
		bool begin(PaintingDevice *pDevice);
		bool end();

		//	virtual void setFont(const Font &font);
		void setPen(const Pen &pen);
		void setPenColor(const Color &color);
		void setBrush(const Brush &brush);
		void setBrushStyle(BrushStyle style);
		void setBackgroundBrush(const Brush& brush);
		void setBackgroundMode(BackgroundMode bgMode);
		void setFont(const Font& font);

		void clear();

		void line(int x1, int y1, int x2, int y2);
		void rectangle(int x, int y, int width, int height);
		void circle(int center, unsigned int radius);
		void ellipse(int x, int width, int height);

		void fillRect(int x, int y, int width, int height, const Color& color);
		inline void fillRect(int x, int y, int width, int height);

		void drawBitmap(int x, int y, const Bitmap& bitmap);
		void plot(uint x, uint y, const Color &color);

		void drawText(int x, int y, const string& text);

		void setTransform(const Transform& transform, bool combine);
		void resetTransform();
		void scale(float sx, float sy);
		void shear(float sh, float sv);
		void rotate(float degrees);
		void translate(float dx, float dy);

		const Pen& pen() const { return m_pen; }
		const Brush& brush() const { return m_brush; }
		const Font& font() const { return m_font; }
	};

	inline void Painter::fillRect(int x, int y, int width, int height)
	{
		fillRect(x, y, width, height, m_brush.color());
	}

}

#endif