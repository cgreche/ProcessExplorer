
//Point
// File: point.h
// Last Edit: 11/08/2014 18:10 (UTC-3)
// Author: CGR

#ifndef __POINT_H__
#define __POINT_H__

namespace uilib {

	class Point
	{
		int m_x;
		int m_y;

	public:
		Point() : m_x(0), m_y(0) { }
		Point(int x, int y) : m_x(x), m_y(y) { }

		void set(int x, int y) {
			m_x = x;
			m_y = y;
		}

		void setX(int x) { m_x = x; }
		void setY(int y) { m_y = y; }
		int x() const { return m_x; }
		int y() const { return m_y; }

		Point &operator+=(const Point &point) {
			m_x += point.m_x;
			m_y += point.m_y;
			return *this;
		}

		Point &operator-=(const Point &point) {
			m_x -= point.m_x;
			m_y -= point.m_y;
			return *this;
		}

		Point &operator*=(float scalar) {
			m_x *= (int)scalar;
			m_y *= (int)scalar;
			return *this;
		}

		Point &operator/=(float scalar) {
			m_x /= (int)scalar;
			m_y /= (int)scalar;
			return *this;
		}

		friend inline bool operator==(const Point&, const Point&);
		friend inline bool operator!=(const Point&, const Point&);
		friend inline const Point operator+(const Point&, const Point &);
		friend inline const Point operator-(const Point&, const Point &);
		friend inline const Point operator*(const Point&, int);
		friend inline const Point operator*(int, const Point&);
		friend inline const Point operator+(const Point&);
		friend inline const Point operator-(const Point&);
		friend inline const Point operator/(const Point&, int);
	};

	inline bool operator==(const Point& p1, const Point& p2) {
		return p1.m_x == p2.m_x && p1.m_y == p2.m_y;
	}

	inline bool operator!=(const Point& p1, const Point& p2) {
		return p1.m_x != p2.m_x || p1.m_y != p2.m_y;
	}

	inline const Point operator+(const Point& p1, const Point &p2) {
		return Point(p1.m_x + p2.m_x, p1.m_y + p2.m_y);
	}

	inline const Point operator-(const Point& p1, const Point &p2) {
		return Point(p1.m_x - p2.m_x, p1.m_y - p2.m_y);
	}

	inline const Point operator*(const Point& p, int scalar) {
		return Point(p.m_x*scalar, p.m_y*scalar);
	}

	inline const Point operator*(int scalar, const Point& p) {
		return Point(p.m_x*scalar, p.m_y*scalar);
	}

	inline const Point operator+(const Point& p) {
		return p;
	}

	inline const Point operator-(const Point& p) {
		return Point(-p.m_x, -p.m_y);
	}

	inline const Point operator/(const Point& p, int scalar) {
		return Point(p.m_x / scalar, p.m_y / scalar);
	}

	class PointF
	{
		float m_x;
		float m_y;

	public:
		PointF() : m_x(0), m_y(0) { }
		PointF(float x, float y) : m_x(x), m_y(y) { }

		float set(float x, float y) {
			m_x = x;
			m_y = y;
		}

		float setX(float x) { m_x = x; }
		float setY(float y) { m_y = y; }

		float x() const { return m_x; }
		float y() const { return m_y; }

		PointF &operator+=(const PointF &point) {
			m_x += point.m_x;
			m_y += point.m_y;
			return *this;
		}

		PointF &operator-=(const PointF &point) {
			m_x -= point.m_x;
			m_y -= point.m_y;
			return *this;
		}

		PointF &operator*=(float scalar) {
			m_x *= scalar;
			m_y *= scalar;
			return *this;
		}

		PointF &operator/=(float scalar) {
			m_x /= scalar;
			m_y /= scalar;
			return *this;
		}
	};

}

#endif
