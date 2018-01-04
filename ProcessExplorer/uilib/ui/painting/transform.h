#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "..\\..\\lib\\matrix.h"

namespace uilib {

	class Transform
	{
		Matrix33 m_matrix;

	public:
		Transform();
		Transform(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33);
		Transform(float m11, float m12, float m21, float m22, float dx, float dy);

		float m11() const { return m_matrix.m11(); }
		float m12() const { return m_matrix.m12(); }
		float m13() const { return m_matrix.m13(); }
		float m21() const { return m_matrix.m21(); }
		float m22() const { return m_matrix.m22(); }
		float m23() const { return m_matrix.m23(); }
		float m31() const { return m_matrix.m31(); }
		float m32() const { return m_matrix.m32(); }
		float m33() const { return m_matrix.m33(); }

		Transform &reset();
		Transform &translate(float dx, float dy);
		Transform &rotate(float angle);
		Transform &rotateRadians(float radians);
		Transform &scale(float sx, float sy);
		Transform &shear(float sh, float sv);

		Transform &combine(const Transform& trans);

		Transform getInverse();

		void map(float *x, float *y);
		void map(float x, float y, float *outX, float *outY);
	};

}

#endif