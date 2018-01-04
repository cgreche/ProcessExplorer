
//Size
// File: size.h
// Last Edit: 11/08/2014 19:50 (UTC-3)
// Author: CGR

#ifndef __SIZE_H__
#define __SIZE_H__

namespace uilib {

	class Size
	{
		int m_width;
		int m_height;

	public:
		Size() : m_width(0), m_height(0) { }
		Size(int w, int h) : m_width(w), m_height(h) { }

		void set(int w, int h) {
			m_width = w;
			m_height = h;
		}

		void setWidth(int w) { m_width = w; }
		void setHeight(int h) { m_height = h; }
		int width() const { return m_width; }
		int height() const { return m_height; }

		Size &operator+=(const Size &size) {
			m_width += size.m_width;
			m_height += size.m_height;
			return *this;
		}

		Size &operator-=(const Size &size) {
			m_width -= size.m_width;
			m_height -= size.m_height;
			return *this;
		}

		Size &operator*=(float scalar) {
			m_width *= (int)scalar;
			m_height *= (int)scalar;
			return *this;
		}

		Size &operator/=(float scalar) {
			m_width /= (int)scalar;
			m_height /= (int)scalar;
			return *this;
		}

		friend inline bool operator==(const Size&, const Size&);
		friend inline bool operator!=(const Size&, const Size&);
		friend inline const Size operator+(const Size&, const Size&);
		friend inline const Size operator-(const Size&, const Size&);
		friend inline const Size operator*(const Size&, int);
		friend inline const Size operator*(int, const Size&);
		friend inline const Size operator/(const Size&, int);
	};

	inline bool operator==(const Size& s1, const Size& s2) {
		return s1.m_width == s2.m_width && s1.m_height == s2.m_height;
	}

	inline bool operator!=(const Size& s1, const Size& s2) {
		return s1.m_width != s2.m_width || s1.m_height != s2.m_height;
	}

	inline const Size operator+(const Size& s1, const Size& s2) {
		return Size(s1.m_width + s2.m_width, s1.m_height + s2.m_height);
	}

	inline const Size operator-(const Size& s1, const Size& s2) {
		return Size(s1.m_width - s2.m_width, s2.m_height - s2.m_height);
	}

	inline const Size operator*(const Size& s, int scalar) {
		return Size(s.m_width*scalar, s.m_height*scalar);
	}

	inline const Size operator*(int scalar, const Size& s) {
		return Size(s.m_width*scalar, s.m_height*scalar);
	}

	inline const Size operator/(const Size& s, int scalar) {
		return Size(s.m_width / scalar, s.m_height / scalar);
	}


	class SizeF
	{
		float m_width;
		float m_height;

	public:
		SizeF() : m_width(0), m_height(0) { }
		SizeF(float w, float h) : m_width(w), m_height(h) { }

		float set(float w, float h) {
			m_width = w;
			m_height = h;
		}

		float setWidth(float w) { m_width = w; }
		float setHeight(float h) { m_height = h; }

		float width() const { return m_width; }
		float height() const { return m_height; }

		SizeF &operator+=(const SizeF &size) {
			m_width += size.m_width;
			m_height += size.m_height;
			return *this;
		}

		SizeF &operator-=(const SizeF &size) {
			m_width -= size.m_width;
			m_height -= size.m_height;
			return *this;
		}

		SizeF &operator*=(float scalar) {
			m_width *= scalar;
			m_height *= scalar;
			return *this;
		}

		SizeF &operator/=(float scalar) {
			m_width /= scalar;
			m_height /= scalar;
			return *this;
		}
	};

}

#endif
