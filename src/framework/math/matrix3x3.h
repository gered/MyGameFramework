#ifndef __FRAMEWORK_MATH_MATRIX3X3_H_INCLUDED__
#define __FRAMEWORK_MATH_MATRIX3X3_H_INCLUDED__

#include "../common.h"
#include <math.h>
#include "common.h"
#include "mathhelpers.h"
#include "vector3.h"
#include "matrix4x4.h"

enum MATRIX3X3_ELEMENTS
{
	M3X3_11 = 0,
	M3X3_12 = 3,
	M3X3_13 = 6,
	M3X3_21 = 1,
	M3X3_22 = 4,
	M3X3_23 = 7,
	M3X3_31 = 2,
	M3X3_32 = 5,
	M3X3_33 = 8
};

/**
 * Represents a 3x3 column-major Matrix and provides common methods for
 * matrix math.
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>http://www.dhpoware.com/source/mathlib.html</li>
 * <li>http://www.songho.ca/opengl/gl_transform.html</li>
 * <li>http://www.opengl.org/sdk/docs/man/</li>
 * </ul>
 */
struct Matrix3x3
{
	/**
	 * Creates an uninitialized matrix.
	 */
	Matrix3x3();

	/**
	 * Creates a matrix with the specified elements.
	 */
	Matrix3x3(
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33
		);

	/**
	 * Creates a matrix with the specified elements.
	 * @param mv an array of 9 values that will be used as this matrix's
	 *           elements
	 */
	Matrix3x3(const float *mv);

	/**
	 * Creates a matrix from a 4x4 one.
	 * @param m the 4x4 matrix to create this matrix from (top-left 3x3 elements
	 *          will be copied into this one)
	 */
	Matrix3x3(const Matrix4x4 &m);

	/**
	 * Sets new element values for this matrix.
	 */
	void Set(
		float m11, float m12, float m13,
		float m21, float m22, float m23,
		float m31, float m32, float m33
		);

	/**
	 * @return the determinant of this matrix
	 */
	float GetDeterminant() const;

	/**
	 * @return the forward vector (z-axis)
	 */
	Vector3 GetForward() const;

	/**
	 * @return the backward vector (inverse z-axis)
	 */
	Vector3 GetBackward() const;

	/**
	 * @return the left vector (x-axis)
	 */
	Vector3 GetLeft() const;

	/**
	 * @return the right vector (inverse x-axis)
	 */
	Vector3 GetRight() const;

	/**
	 * @return the up vector (y-axis)
	 */
	Vector3 GetUp() const;

	/**
	 * @return the down vector (inverse y-axis)
	 */
	Vector3 GetDown() const;

	/**
	 * @return an equivalent 4x4 matrix
	 */
	Matrix4x4 To4x4() const;

	/** 
	 * @return identity matrix
	 */
	static Matrix3x3 Identity();

	/**
	 * Constructs a rotation matrix from Euler angles.
	 * @param x the x angle (in radians)
	 * @param y the y angle (in radians)
	 * @param z the z angle (in radians)
	 * @return a rotation matrix representing the provided angles
	 */
	static Matrix3x3 CreateFromEulerAngles(float x, float y, float z);

	/**
	 * Sets up a rotation matrix about the specified axis. This returns a matrix
	 * equivalent to the matrix that OpenGL multiples into the modelview matrix
	 * whenever glRotate(angle, axis.x, axis.y, axis.z) is called.
	 * @param angle the angle to rotate by (in radians)
	 * @param axis unit vector denoting the axis for the rotation
	 * @return a rotation matrix representing the specific axis rotation
	 */
	static Matrix3x3 CreateRotation(float angle, const Vector3 &axis);

	/**
	 * Constructs a rotation matrix for a rotation around the x axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 1.0f, 0.0f, 0.0f) is called.
	 * @param angle angle to rotate the x axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix3x3 CreateRotationX(float angle);

	/**
	 * Constructs a rotation matrix for a rotation around the y axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 0.0f, 1.0f, 0.0f) is called.
	 * @param angle angle to rotate the y axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix3x3 CreateRotationY(float angle);

	/**
	 * Constructs a rotation matrix for a rotation around the z axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 0.0f, 0.0f, 1.0f) is called.
	 * @param angle angle to rotate the z axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix3x3 CreateRotationZ(float angle);

	/**
	 * Calculates the inverse of the specified matrix.
	 * @param m the matrix to calculate the inverse of
	 * @return the inverse of the specified matrix
	 */
	static Matrix3x3 Inverse(const Matrix3x3 &m);

	/**
	 * Calculates the transpose of a given matrix.
	 * @param m the matrix to get the transpose of
	 * @return the transpose matrix
	 */
	static Matrix3x3 Transpose(const Matrix3x3 &m);

	/**
	 * Transforms the given vertex by a matrix.
	 * @param m the transformation matrix to use
	 * @param v the vertex to be transformed
	 * @return the transformed vertex
	 */
	static Vector3 Transform(const Matrix3x3 &m, const Vector3 &v);
	
	float m[9];
};

Matrix3x3 operator+(const Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 &operator+=(Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 operator-(const Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 &operator-=(Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 operator*(const Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 &operator*=(Matrix3x3 &left, const Matrix3x3 &right);
Matrix3x3 operator*(const Matrix3x3 &left, float right);
Matrix3x3 &operator*=(Matrix3x3 &left, float right);
Vector3 operator*(const Vector3 &left, const Matrix3x3 &right);

#define IDENTITY_MATRIX3X3 Matrix3x3::Identity()

inline Matrix3x3::Matrix3x3()
{
}

inline Matrix3x3::Matrix3x3(
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33
	)
{
	m[M3X3_11] = m11;
	m[M3X3_12] = m12;
	m[M3X3_13] = m13;
	m[M3X3_21] = m21;
	m[M3X3_22] = m22;
	m[M3X3_23] = m23;
	m[M3X3_31] = m31;
	m[M3X3_32] = m32;
	m[M3X3_33] = m33;
}

inline Matrix3x3::Matrix3x3(const float *mv)
{
	m[M3X3_11] = mv[M3X3_11];
	m[M3X3_12] = mv[M3X3_12];
	m[M3X3_13] = mv[M3X3_13];
	m[M3X3_21] = mv[M3X3_21];
	m[M3X3_22] = mv[M3X3_22];
	m[M3X3_23] = mv[M3X3_23];
	m[M3X3_31] = mv[M3X3_31];
	m[M3X3_32] = mv[M3X3_32];
	m[M3X3_33] = mv[M3X3_33];
}

inline Matrix3x3::Matrix3x3(const Matrix4x4 &m)
{
	this->m[M3X3_11] = m.m[_11];
	this->m[M3X3_12] = m.m[_12];
	this->m[M3X3_13] = m.m[_13];
	this->m[M3X3_21] = m.m[_21];
	this->m[M3X3_22] = m.m[_22];
	this->m[M3X3_23] = m.m[_23];
	this->m[M3X3_31] = m.m[_31];
	this->m[M3X3_32] = m.m[_32];
	this->m[M3X3_33] = m.m[_33];
}

inline void Matrix3x3::Set(
	float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33
	)
{
	m[M3X3_11] = m11;
	m[M3X3_12] = m12;
	m[M3X3_13] = m13;
	m[M3X3_21] = m21;
	m[M3X3_22] = m22;
	m[M3X3_23] = m23;
	m[M3X3_31] = m31;
	m[M3X3_32] = m32;
	m[M3X3_33] = m33;
}

inline float Matrix3x3::GetDeterminant() const
{
	return
		m[M3X3_11] * m[M3X3_22] * m[M3X3_33] + 
		m[M3X3_12] * m[M3X3_23] * m[M3X3_31] + 
		m[M3X3_13] * m[M3X3_21] * m[M3X3_32] - 
		m[M3X3_11] * m[M3X3_23] * m[M3X3_32] - 
		m[M3X3_12] * m[M3X3_21] * m[M3X3_33] - 
		m[M3X3_13] * m[M3X3_22] * m[M3X3_31];
}

inline Vector3 Matrix3x3::GetForward() const
{
	return Vector3(m[M3X3_13], m[M3X3_23], m[M3X3_33]);
}

inline Vector3 Matrix3x3::GetBackward() const
{
	return Vector3(-m[M3X3_13], -m[M3X3_23], -m[M3X3_33]);
}

inline Vector3 Matrix3x3::GetLeft() const
{
	return Vector3(m[M3X3_11], m[M3X3_21], m[M3X3_31]);
}

inline Vector3 Matrix3x3::GetRight() const
{
	return Vector3(-m[M3X3_11], -m[M3X3_21], -m[M3X3_31]);
}

inline Vector3 Matrix3x3::GetUp() const
{
	return Vector3(m[M3X3_12], m[M3X3_22], m[M3X3_32]);
}

inline Vector3 Matrix3x3::GetDown() const
{
	return Vector3(-m[M3X3_12], -m[M3X3_22], -m[M3X3_32]);
}

inline Matrix4x4 Matrix3x3::To4x4() const
{
	Matrix4x4 out;

	out.m[_11] = m[M3X3_11];
	out.m[_12] = m[M3X3_12];
	out.m[_13] = m[M3X3_13];
	out.m[_14] = 0.0f;

	out.m[_21] = m[M3X3_21];
	out.m[_22] = m[M3X3_22];
	out.m[_23] = m[M3X3_23];
	out.m[_24] = 0.0f;

	out.m[_31] = m[M3X3_31];
	out.m[_32] = m[M3X3_32];
	out.m[_33] = m[M3X3_33];
	out.m[_34] = 0.0f;

	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;

	return out;
}

inline Matrix3x3 Matrix3x3::Identity()
{
	Matrix3x3 identity;

	identity.m[M3X3_11] = 1.0f;
	identity.m[M3X3_12] = 0.0f;
	identity.m[M3X3_13] = 0.0f;
	identity.m[M3X3_21] = 0.0f;
	identity.m[M3X3_22] = 1.0f;
	identity.m[M3X3_23] = 0.0f;
	identity.m[M3X3_31] = 0.0f;
	identity.m[M3X3_32] = 0.0f;
	identity.m[M3X3_33] = 1.0f;

	return identity;
}

inline Matrix3x3 Matrix3x3::CreateFromEulerAngles(float x, float y, float z)
{
	Matrix3x3 rotateZ = CreateRotationZ(z);
	Matrix3x3 rotateY = CreateRotationY(y);
	Matrix3x3 rotateX = CreateRotationX(x);

	// "right-to-left" column-major matrix concatenation
	return rotateZ * rotateY * rotateX;
}


inline Matrix3x3 Matrix3x3::CreateRotation(float angle, const Vector3 &axis)
{
	Matrix3x3 out;
		
	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[M3X3_11] = (axis.x * axis.x) * (1.0f - c) + c;
	out.m[M3X3_12] = (axis.x * axis.y) * (1.0f - c) - (axis.z * s);
	out.m[M3X3_13] = (axis.x * axis.z) * (1.0f - c) + (axis.y * s);

	out.m[M3X3_21] = (axis.y * axis.x) * (1.0f - c) + (axis.z * s);
	out.m[M3X3_22] = (axis.y * axis.y) * (1.0f - c) + c;
	out.m[M3X3_23] = (axis.y * axis.z) * (1.0f - c) - (axis.x * s);

	out.m[M3X3_31] = (axis.z * axis.x) * (1.0f - c) - (axis.y * s);
	out.m[M3X3_32] = (axis.z * axis.y) * (1.0f - c) + (axis.x * s);
	out.m[M3X3_33] = (axis.z * axis.z) * (1.0f - c) + c;
		
	return out;
}

inline Matrix3x3 Matrix3x3::CreateRotationX(float angle)
{
	Matrix3x3 out;
		
	float s = sinf(angle);
	float c = cosf(angle);
	
	out.m[M3X3_11] = 1.0f;
	out.m[M3X3_12] = 0.0f;
	out.m[M3X3_13] = 0.0f;
		
	out.m[M3X3_21] = 0.0f;
	out.m[M3X3_22] = c;
	out.m[M3X3_23] = -s;
		
	out.m[M3X3_31] = 0.0f;
	out.m[M3X3_32] = s;
	out.m[M3X3_33] = c;
		
	return out;
}

inline Matrix3x3 Matrix3x3::CreateRotationY(float angle)
{
	Matrix3x3 out;

	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[M3X3_11] = c;
	out.m[M3X3_12] = 0.0f;
	out.m[M3X3_13] = s;
		
	out.m[M3X3_21] = 0.0f;
	out.m[M3X3_22] = 1.0f;
	out.m[M3X3_23] = 0.0f;
		
	out.m[M3X3_31] = -s;
	out.m[M3X3_32] = 0.0f;
	out.m[M3X3_33] = c;
		
	return out;
}

inline Matrix3x3 Matrix3x3::CreateRotationZ(float angle)
{
	Matrix3x3 out;

	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[M3X3_11] = c;
	out.m[M3X3_12] = -s;
	out.m[M3X3_13] = 0.0f;
		
	out.m[M3X3_21] = s;
	out.m[M3X3_22] = c;
	out.m[M3X3_23] = 0.0f;
		
	out.m[M3X3_31] = 0.0f;
	out.m[M3X3_32] = 0.0f;
	out.m[M3X3_33] = 1.0f;
		
	return out;
}

inline Matrix3x3 Matrix3x3::Inverse(const Matrix3x3 &m)
{
	float d = m.GetDeterminant();
	if (IsCloseEnough(d, 0.0f))
		return IDENTITY_MATRIX3X3;
	else
	{
		Matrix3x3 out;
		
		d = 1.0f / d;
			
		out.m[M3X3_11] = d * (m.m[M3X3_22] * m.m[M3X3_33] - m.m[M3X3_32] * m.m[M3X3_23]);
		out.m[M3X3_21] = d * (m.m[M3X3_31] * m.m[M3X3_23] - m.m[M3X3_21] * m.m[M3X3_33]);
		out.m[M3X3_31] = d * (m.m[M3X3_21] * m.m[M3X3_32] - m.m[M3X3_31] * m.m[M3X3_22]);
		out.m[M3X3_12] = d * (m.m[M3X3_32] * m.m[M3X3_13] - m.m[M3X3_12] * m.m[M3X3_33]);
		out.m[M3X3_22] = d * (m.m[M3X3_11] * m.m[M3X3_33] - m.m[M3X3_31] * m.m[M3X3_13]);
		out.m[M3X3_32] = d * (m.m[M3X3_31] * m.m[M3X3_12] - m.m[M3X3_11] * m.m[M3X3_32]);
		out.m[M3X3_13] = d * (m.m[M3X3_12] * m.m[M3X3_23] - m.m[M3X3_22] * m.m[M3X3_13]);
		out.m[M3X3_23] = d * (m.m[M3X3_21] * m.m[M3X3_13] - m.m[M3X3_11] * m.m[M3X3_23]);
		out.m[M3X3_33] = d * (m.m[M3X3_11] * m.m[M3X3_22] - m.m[M3X3_21] * m.m[M3X3_12]);
				
		return out;
	}
}

inline Matrix3x3 Matrix3x3::Transpose(const Matrix3x3 &m)
{
	Matrix3x3 out;
		
	out.m[M3X3_11] = m.m[M3X3_11];
	out.m[M3X3_12] = m.m[M3X3_21];
	out.m[M3X3_13] = m.m[M3X3_31];

	out.m[M3X3_21] = m.m[M3X3_12];
	out.m[M3X3_22] = m.m[M3X3_22];
	out.m[M3X3_23] = m.m[M3X3_32];

	out.m[M3X3_31] = m.m[M3X3_13];
	out.m[M3X3_32] = m.m[M3X3_23];
	out.m[M3X3_33] = m.m[M3X3_33];
		
	return out;
}

inline Vector3 Matrix3x3::Transform(const Matrix3x3 &m, const Vector3 &v)
{
	Vector3 out;
	
	out.x = v.x * m.m[M3X3_11] + v.y * m.m[M3X3_12] + v.z * m.m[M3X3_13];
	out.y = v.x * m.m[M3X3_21] + v.y * m.m[M3X3_22] + v.z * m.m[M3X3_23];
	out.z = v.x * m.m[M3X3_31] + v.y * m.m[M3X3_32] + v.z * m.m[M3X3_33];
	
	return out;
}

inline Matrix3x3 operator+(const Matrix3x3 &left, const Matrix3x3 &right)
{
	Matrix3x3 result;

	result.m[M3X3_11] = left.m[M3X3_11] + right.m[M3X3_11]; 
	result.m[M3X3_12] = left.m[M3X3_12] + right.m[M3X3_12];
	result.m[M3X3_13] = left.m[M3X3_13] + right.m[M3X3_13];
	result.m[M3X3_21] = left.m[M3X3_21] + right.m[M3X3_21];
	result.m[M3X3_22] = left.m[M3X3_22] + right.m[M3X3_22];
	result.m[M3X3_23] = left.m[M3X3_23] + right.m[M3X3_23];
	result.m[M3X3_31] = left.m[M3X3_31] + right.m[M3X3_31];
	result.m[M3X3_32] = left.m[M3X3_32] + right.m[M3X3_32];
	result.m[M3X3_33] = left.m[M3X3_33] + right.m[M3X3_33];

	return result;
}

inline Matrix3x3 &operator+=(Matrix3x3 &left, const Matrix3x3 &right)
{
	left = left + right;
	return left;
}

inline Matrix3x3 operator-(const Matrix3x3 &left, const Matrix3x3 &right)
{
	Matrix3x3 result;

	result.m[M3X3_11] = left.m[M3X3_11] - right.m[M3X3_11]; 
	result.m[M3X3_12] = left.m[M3X3_12] - right.m[M3X3_12];
	result.m[M3X3_13] = left.m[M3X3_13] - right.m[M3X3_13];
	result.m[M3X3_21] = left.m[M3X3_21] - right.m[M3X3_21];
	result.m[M3X3_22] = left.m[M3X3_22] - right.m[M3X3_22];
	result.m[M3X3_23] = left.m[M3X3_23] - right.m[M3X3_23];
	result.m[M3X3_31] = left.m[M3X3_31] - right.m[M3X3_31];
	result.m[M3X3_32] = left.m[M3X3_32] - right.m[M3X3_32];
	result.m[M3X3_33] = left.m[M3X3_33] - right.m[M3X3_33];

	return result;
}

inline Matrix3x3 &operator-=(Matrix3x3 &left, const Matrix3x3 &right)
{
	left = left - right;
	return left;
}

inline Matrix3x3 operator*(const Matrix3x3 &left, const Matrix3x3 &right)
{
	Matrix3x3 result;

	result.m[M3X3_11] = left.m[M3X3_11] * right.m[M3X3_11] + left.m[M3X3_12] * right.m[M3X3_21] + left.m[M3X3_13] * right.m[M3X3_31];
	result.m[M3X3_12] = left.m[M3X3_11] * right.m[M3X3_12] + left.m[M3X3_12] * right.m[M3X3_22] + left.m[M3X3_13] * right.m[M3X3_32];
	result.m[M3X3_13] = left.m[M3X3_11] * right.m[M3X3_13] + left.m[M3X3_12] * right.m[M3X3_23] + left.m[M3X3_13] * right.m[M3X3_33];

	result.m[M3X3_21] = left.m[M3X3_21] * right.m[M3X3_11] + left.m[M3X3_22] * right.m[M3X3_21] + left.m[M3X3_23] * right.m[M3X3_31];
	result.m[M3X3_22] = left.m[M3X3_21] * right.m[M3X3_12] + left.m[M3X3_22] * right.m[M3X3_22] + left.m[M3X3_23] * right.m[M3X3_32];
	result.m[M3X3_23] = left.m[M3X3_21] * right.m[M3X3_13] + left.m[M3X3_22] * right.m[M3X3_23] + left.m[M3X3_23] * right.m[M3X3_33];

	result.m[M3X3_31] = left.m[M3X3_31] * right.m[M3X3_11] + left.m[M3X3_32] * right.m[M3X3_21] + left.m[M3X3_33] * right.m[M3X3_31];
	result.m[M3X3_32] = left.m[M3X3_31] * right.m[M3X3_12] + left.m[M3X3_32] * right.m[M3X3_22] + left.m[M3X3_33] * right.m[M3X3_32];
	result.m[M3X3_33] = left.m[M3X3_31] * right.m[M3X3_13] + left.m[M3X3_32] * right.m[M3X3_23] + left.m[M3X3_33] * right.m[M3X3_33];

	return result;
}

inline Matrix3x3 &operator*=(Matrix3x3 &left, const Matrix3x3 &right)
{
	left = left * right;
	return left;
}

inline Matrix3x3 operator*(const Matrix3x3 &left, float right)
{
	Matrix3x3 result;

	result.m[M3X3_11] = left.m[M3X3_11] * right;
	result.m[M3X3_12] = left.m[M3X3_12] * right;
	result.m[M3X3_13] = left.m[M3X3_13] * right;
	result.m[M3X3_21] = left.m[M3X3_21] * right;
	result.m[M3X3_22] = left.m[M3X3_22] * right;
	result.m[M3X3_23] = left.m[M3X3_23] * right;
	result.m[M3X3_31] = left.m[M3X3_31] * right;
	result.m[M3X3_32] = left.m[M3X3_32] * right;
	result.m[M3X3_33] = left.m[M3X3_33] * right;

	return result;
}

inline Matrix3x3 &operator*=(Matrix3x3 &left, float right)
{
	left = left * right;
	return left;
}

inline Vector3 operator*(const Vector3 &left, const Matrix3x3 &right)
{
	return Matrix3x3::Transform(right, left);
}

#endif

