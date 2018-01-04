
#include "matrix.h"

namespace uilib {

	Matrix33::Matrix33()
		: _m11(1), _m12(0), _m13(0),
		_m21(0), _m22(1), _m23(0),
		_m31(0), _m32(0), _m33(1)
	{
	}


	Matrix33::Matrix33(float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33)
		: _m11(m11), _m12(m12), _m13(m13),
		_m21(m21), _m22(m22), _m23(m23),
		_m31(m31), _m32(m32), _m33(m33)
	{
	}


	void Matrix33::setIdentity()
	{
		_m12 = _m13 = _m21 = _m23 = _m31 = _m32 = 0.0f;
		_m11 = _m22 = _m33 = 1.0f;
	}

	bool Matrix33::isIdentity()
	{
		return _m12 == 0 && _m13 == 0 && _m21 == 0 && _m23 == 0 && _m31 == 0 && _m32 == 0.0f
			&& _m11 == 1.0f && _m22 == 1.0f && _m33 == 1.0f;
	}


	bool Matrix33::operator==(const Matrix33 &matrix2) const
	{
		return _m11 == matrix2._m11
			&& _m12 == matrix2._m12
			&& _m13 == matrix2._m13
			&& _m21 == matrix2._m21
			&& _m22 == matrix2._m22
			&& _m23 == matrix2._m23
			&& _m31 == matrix2._m31
			&& _m32 == matrix2._m32
			&& _m33 == matrix2._m33;
	}

	bool Matrix33::operator!=(const Matrix33 &matrix2) const
	{
		return _m11 != matrix2._m11
			|| _m12 != matrix2._m12
			|| _m13 != matrix2._m13
			|| _m21 != matrix2._m21
			|| _m22 != matrix2._m22
			|| _m23 != matrix2._m23
			|| _m31 != matrix2._m31
			|| _m32 != matrix2._m32
			|| _m33 != matrix2._m33;
	}

	Matrix33 &Matrix33::operator=(const Matrix33 &matrix2)
	{
		_m11 = matrix2._m11;
		_m12 = matrix2._m12;
		_m13 = matrix2._m13;
		_m21 = matrix2._m21;
		_m22 = matrix2._m22;
		_m23 = matrix2._m23;
		_m31 = matrix2._m31;
		_m32 = matrix2._m32;
		_m33 = matrix2._m33;
		return *this;
	}

	Matrix33 &Matrix33::operator*=(const Matrix33 &matrix2)
	{
		float m11 = _m11*matrix2._m11 + _m12*matrix2._m21 + _m13*matrix2._m31;
		float m12 = _m11*matrix2._m12 + _m12*matrix2._m22 + _m13*matrix2._m32;
		float m13 = _m11*matrix2._m13 + _m12*matrix2._m23 + _m13*matrix2._m33;

		float m21 = _m21*matrix2._m11 + _m22*matrix2._m21 + _m23*matrix2._m31;
		float m22 = _m21*matrix2._m12 + _m22*matrix2._m22 + _m23*matrix2._m32;
		float m23 = _m21*matrix2._m13 + _m22*matrix2._m23 + _m23*matrix2._m33;

		float m31 = _m31*matrix2._m11 + _m32*matrix2._m21 + _m33*matrix2._m31;
		float m32 = _m31*matrix2._m12 + _m32*matrix2._m22 + _m33*matrix2._m32;
		float m33 = _m31*matrix2._m13 + _m32*matrix2._m23 + _m33*matrix2._m33;

		_m11 = m11; _m12 = m12; _m13 = m13;
		_m21 = m21; _m22 = m22; _m23 = m23;
		_m31 = m31; _m32 = m32; _m33 = m33;
		return *this;
	}

	Matrix33 Matrix33::operator*(const Matrix33 &matrix2) const
	{
		float m11 = _m11*matrix2._m11 + _m12*matrix2._m21 + _m13*matrix2._m31;
		float m12 = _m11*matrix2._m12 + _m12*matrix2._m22 + _m13*matrix2._m32;
		float m13 = _m11*matrix2._m13 + _m12*matrix2._m23 + _m13*matrix2._m33;

		float m21 = _m21*matrix2._m11 + _m22*matrix2._m21 + _m23*matrix2._m31;
		float m22 = _m21*matrix2._m12 + _m22*matrix2._m22 + _m23*matrix2._m32;
		float m23 = _m21*matrix2._m13 + _m22*matrix2._m23 + _m23*matrix2._m33;

		float m31 = _m31*matrix2._m11 + _m32*matrix2._m21 + _m33*matrix2._m31;
		float m32 = _m31*matrix2._m12 + _m32*matrix2._m22 + _m33*matrix2._m32;
		float m33 = _m31*matrix2._m13 + _m32*matrix2._m23 + _m33*matrix2._m33;

		return Matrix33(m11, m12, m13, m21, m22, m23, m31, m32, m33);
	}

	float Matrix33::determinant()
	{
		return _m11*_m22 - _m12*_m21;
	}

	Matrix33 Matrix33::getInverse()
	{
		float dtr = determinant();
		if (dtr == 0.0f) {
			return Matrix33();
		}

		float dinv = 1.0f / dtr;
		return Matrix33((_m22*dinv), (-_m12*dinv), 0,
			(-_m21*dinv), (_m11*dinv), 0,
			((_m21*_m32 - _m22*_m31)*dinv),
			((_m12*_m31 - _m11*_m32)*dinv),
			1);
	}

}