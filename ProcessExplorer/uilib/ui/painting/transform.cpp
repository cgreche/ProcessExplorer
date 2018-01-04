#include "transform.h"
#include <math.h>

namespace uilib {

	Transform::Transform()
	{
	}

	Transform::Transform(float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33)
		: m_matrix(m11, m12, m13, m21, m22, m23, m31, m32, m33)
	{
	}

	Transform::Transform(float m11, float m12, float m21, float m22, float dx, float dy)
		: m_matrix(m11, m12, 0, m21, m22, 0, dx, dy, 1)
	{
	}

	Transform& Transform::reset()
	{
		m_matrix.setIdentity();
		return *this;
	}

	Transform& Transform::translate(float dx, float dy)
	{
		Transform trans(1.0f, 0, 0, 1.0f, dx, dy);
		return combine(trans);
	}

#define DEG2RAD (3.1415926f/180.0f);
	Transform& Transform::rotate(float degrees)
	{
		float cosa = 0;
		float sina = 0;

		if (degrees == 90.f || degrees == -270.f)
			sina = 1.f;
		else if (degrees == 270.f || degrees == -90.f)
			sina = -1.f;
		else if (degrees == 180.f)
			cosa = -1.f;
		else {
			float a = degrees*DEG2RAD;
			cosa = cosf(a);
			sina = sinf(a);
		}

		Transform rot(cosa, sina, -sina, cosa, 0, 0);
		return combine(rot);
	}

	Transform& Transform::rotateRadians(float rads)
	{
		float cosa = cosf(rads);
		float sina = sinf(rads);

		Transform rot(cosa, sina, -sina, cosa, 0, 0);
		return combine(rot);
	}

	Transform& Transform::scale(float sx, float sy)
	{
		Transform sc(sx, 0, 0, sy, 0, 0);
		return combine(sc);
	}

	Transform& Transform::shear(float sh, float sv)
	{
		Transform s(1, sv, sh, 1, 0, 0);
		return combine(s);
	}

	Transform& Transform::combine(const Transform &trans)
	{
		m_matrix *= trans.m_matrix;
		return *this;
	}

	Transform Transform::getInverse()
	{
		Matrix33 matrix = m_matrix.getInverse();
		return Transform(matrix.m11(), matrix.m12(), matrix.m21(), matrix.m22(), matrix.m31(), matrix.m32());
	}

	void Transform::map(float *x, float *y)
	{
		if (!x || !y)
			return;

		float _x = *x;
		float _y = *y;
		Matrix33 &m = m_matrix;
		*x = _x*m.m11() + _y*m.m21() + m.m31();
		*y = _x*m.m12() + _y*m.m22() + m.m32();
	}

	void Transform::map(float x, float y, float *outX, float *outY)
	{
		Matrix33 &m = m_matrix;
		*outX = x*m.m11() + y*m.m21() + m.m31();
		*outY = x*m.m12() + y*m.m22() + m.m32();
	}

}