
//Color
// File name: color.h
// Last edit: 26/07/2014 21:53 (UTC-3)
// Author: CGR

#ifndef __COLOR_H__
#define __COLOR_H__

namespace uilib {

	class Color
	{
		u8 m_red, m_green, m_blue, m_alpha;

	public:
		Color() {
			setRGBA(0, 0, 0, 255);
		}
		Color(int r, int g, int b, int a = 255) { setRGBA(r, g, b, a); }

		void setRGB(int r, int g, int b) { setRGBA(r, g, b, 255); }
		void setRGBA(int r, int g, int b, int a) {
			m_red = r & 0xff;
			m_green = g & 0xff;
			m_blue = b & 0xff;
			m_alpha = a & 0xff;
		}

		int red() const { return m_red; }
		int green() const { return m_green; }
		int blue() const { return m_blue; }
		int alpha() const { return m_alpha; }
	};

}

#endif