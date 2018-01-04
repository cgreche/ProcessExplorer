
//Rect
// File: rect.h
// Last Edit: 12/08/2014 18:57 (UTC-3)
// Author: CGR

#ifndef __RECT_H__
#define __RECT_H__

namespace uilib {

	struct Rect
	{
		int m_x1, m_y1, m_x2, m_y2;

	public:
		Rect() : m_x1(0), m_y1(0), m_x2(0), m_y2(0) { }
		Rect(int x1, int y1, int x2, int y2) : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2) { }

		void set(int x1, int y1, int x2, int y2) { m_x1 = x1; m_y1 = y1; m_x2 = x2; m_y2 = y2; }
		int x1() const { return m_x1; }
		int y1() const { return m_y1; }
		int x2() const { return m_x2; }
		int y2() const { return m_y2; }
	};

	class RectF
	{
		float m_x1, m_y1, m_x2, m_y2;
	};

}

#endif
