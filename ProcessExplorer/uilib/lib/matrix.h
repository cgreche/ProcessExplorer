#ifndef __MATRIX_H__
#define __MATRIX_H__

namespace uilib {

	class Matrix33
	{
		float _m11, _m12, _m13;
		float _m21, _m22, _m23;
		float _m31, _m32, _m33;

	public:
		Matrix33();
		Matrix33(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33);

		float m11() const { return _m11; }
		float m12() const { return _m12; }
		float m13() const { return _m13; }
		float m21() const { return _m21; }
		float m22() const { return _m22; }
		float m23() const { return _m23; }
		float m31() const { return _m31; }
		float m32() const { return _m32; }
		float m33() const { return _m33; }

		void setIdentity();
		bool isIdentity();

		float determinant();
		Matrix33 getInverse();

		bool operator==(const Matrix33 &matrix2) const;
		bool operator!=(const Matrix33 &matrix2) const;

		Matrix33 &operator=(const Matrix33 &matrix2);
		Matrix33 &operator*=(const Matrix33 &matrix2);
		Matrix33 operator*(const Matrix33 &matrix2) const;
	};

}

#endif