#ifndef __FRAMEWORK_MATH_QUATERNION_H_INCLUDED__
#define __FRAMEWORK_MATH_QUATERNION_H_INCLUDED__

#include <math.h>
#include "common.h"
#include "vector3.h"
#include "matrix4x4.h"

/**
 * Represents a quaternion to store an orientation or angular
 * displacement and provides methods for conversion/manipulation
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>More OpenGL Game Programming (Dave Astle, 2006)</li>
 * <li>http://www.dhpoware.com/source/mathlib.html</li>
 * </ul>
 */
struct Quaternion
{
	/**
	 * Creates an uninitialized quaternion.
	 */
	Quaternion();

	/**
	 * Creates a quaternion with the specified properties.
	 */
	Quaternion(float x, float y, float z, float w);

	/**
	 * Creates a quaternion with the specified properties.
	 */
	Quaternion(const Vector3 &v, float w);

	/**
	 * Creates a quaternion with the specified properties.
	 * @param q an array with 4 values that will be used as this quaternion's components
	 */
	Quaternion(const float *q);

	/**
	 * Set new component values for this quaternion.
	 */
	void Set(float x, float y, float z, float w);

	/**
	 * Converts a quaternion to a rotation matrix.
	 * @return equivalent rotation matrix
	 */
	Matrix4x4 ToMatrix() const;

	/**
	 * @return the vector component of the quaternion.
	 */
	Vector3 GetVector() const;

	/**
	 * Computes the conjugate of a quaternion. If the quaternion
	 * is unit length, this also returns the inverse.
	 * @param q quaternion to retrieve the conjugate of
	 * @return the conjugate of the quaternion
	 */
	static Quaternion Conjugate(const Quaternion &q);

	/**
	 * Converts Euler angles to a quaternion.
	 * @param x x angle of rotation (in radians)
	 * @param y y angle of rotation (in radians)
	 * @param z z angle of rotation (in radians)
	 * @return quaternion equivalent to the Euler angle orientation
	 */
	static Quaternion CreateFromEulerAngles(float x, float y, float z);

	/**
	 * Converts an axis angle to a quaternion.
	 * @param angle the angle to rotate be (in radians)
	 * @param axis the axis being rotated about
	 * @return quaternion equivalent to the axis angle orientation
	 */
	static Quaternion CreateFromAxisAngle(float angle, const Vector3 &axis);

	/**
	 * Converts a rotation matrix to a quaternion.
	 * @param matrix matrix to be converted
	 * @return quaternion equivalent of the rotation matrix
	 */
	static Quaternion CreateFromRotationMatrix(const Matrix4x4 &matrix);

	/**
	 * Computes the cross product of two quaternions.
	 * @param a first quaternion
	 * @param b second quaternion
	 * @return the cross product
	 */
	static Quaternion Cross(const Quaternion &a, const Quaternion &b);

	/**
	 * Computes the dot product of 2 quaternions.
	 * @param a first quaternion
	 * @param b second quaternion
	 * @return the dot product
	 */
	static float Dot(const Quaternion &a, const Quaternion &b);

	/**
	 * Converts a quaternion to an axis-angle representation.
	 * @param q the normalized quaternion to convert
	 * @param angle the angle (in radians)
	 * @param axis a vector that will contain the axis
	 */
	static void ExtractAxisAngle(const Quaternion &q, float &angle, Vector3 &axis);

	/**
	 * Computes the inverse of a given quaternion.
	 * @param q quaternion to retrieve the inverse of
	 * @return inverse of the quaternion
	 */
	static Quaternion Inverse(const Quaternion &q);

	/**
	 * Computes the length (magnitude) of a quaternion.
	 * @param q quaternion to retrieve the length of
	 * @return the length
	 */
	static float Length(const Quaternion &q);

	/**
	 * Computes the squared length of a quaternion (the length minus the 
	 * sqrt call).
	 * @param q quaternion to retrieve the squared length of
	 * @return the squared length
	 */
	static float LengthSquared(const Quaternion &q);

	/**
	 * Linearly interpolates between two quaternions.
	 * @param a the first quaternion
	 * @param b the second quaternion
	 * @param interpolation the amount to interpolate
	 * @return the interpolated quaternion
	 */
	static Quaternion Lerp(const Quaternion &a, const Quaternion &b, float interpolation);

	/**
	 * Normalizes a quaternion (only if necessary).
	 * @param q quaternion to be normalized
	 * @return the normalized quaternion
	 */
	static Quaternion Normalize(const Quaternion &q);

	/**
	 * Interpolates between two quaternions using spherical
	 * interpolation.
	 * @param a the first quaternion
	 * @param b the second quaternion
	 * @param interpolation the amount to interpolate
	 * @return the interpolated quaternion
	 */
	static Quaternion Slerp(const Quaternion &a, const Quaternion &b, float interpolation);

	float x;
	float y;
	float z;
	float w;
};

Quaternion operator+(const Quaternion &left, const Quaternion &right);
Quaternion &operator+=(Quaternion &left, const Quaternion &right);
Quaternion operator-(const Quaternion &left, const Quaternion &right);
Quaternion &operator-=(Quaternion &left, const Quaternion &right);
Quaternion operator*(const Quaternion &left, float right);
Quaternion &operator*=(Quaternion &left, float right);
Quaternion operator*(const Quaternion &left, const Quaternion &right);
Quaternion &operator*=(Quaternion &left, const Quaternion &right);
Quaternion operator/(const Quaternion &left, float right);
Quaternion &operator/=(Quaternion &left, float right);
Vector3 operator*(const Vector3 &left, const Quaternion &right);

#define IDENTITY_QUATERNION Quaternion(0.0f, 0.0f, 0.0f, 1.0f)

inline Quaternion::Quaternion()
{
}

inline Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline Quaternion::Quaternion(const Vector3 &v, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

inline Quaternion::Quaternion(const float *q)
{
	x = q[0];
	y = q[1];
	z = q[2];
	w = q[3];
}

inline void Quaternion::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline Matrix4x4 Quaternion::ToMatrix() const
{
	Matrix4x4 output;

	output.m[_11] = 1.0f - (2.0f * ((y * y) + (z * z)));
	output.m[_21] = 2.0f * ((x * y) + (z * w));
	output.m[_31] = 2.0f * ((z * x) - (y * w));
	output.m[_41] = 0.0f;

	output.m[_12] = 2.0f * ((x * y) - (z * w));
	output.m[_22] = 1.0f - (2.0f * ((z * z) + (x * x)));
	output.m[_32] = 2.0f * ((y * z) + (x * w));
	output.m[_42] = 0.0f;

	output.m[_13] = 2.0f * ((z * x) + (y * w));
	output.m[_23] = 2.0f * ((y * z) - (x * w));
	output.m[_33] = 1.0f - (2.0f * ((y * y) + (x * x)));
	output.m[_43] = 0.0f;

	output.m[_14] = 0.0f;
	output.m[_24] = 0.0f;
	output.m[_34] = 0.0f;
	output.m[_44] = 1.0f;

	return output;
}

inline Vector3 Quaternion::GetVector() const
{
	return Vector3(x, y, z);
}

inline Quaternion Quaternion::Conjugate(const Quaternion &q)
{
	return Quaternion(-q.x, -q.y, -q.z, q.w);
}

inline Quaternion Quaternion::CreateFromEulerAngles(float x, float y, float z)
{
	Quaternion qx(sinf(x / 2.0f), 0.0f, 0.0f, cosf(x / 2.0f));
	Quaternion qy(0.0f, sinf(y / 2.0f), 0.0f, cosf(y / 2.0f));
	Quaternion qz(0.0f, 0.0f, sinf(z / 2.0f), cosf(z / 2.0f));

	return Normalize(qz * qy * qx);
}

inline Quaternion Quaternion::CreateFromAxisAngle(float angle, const Vector3 &axis)
{
	Quaternion result;

	float c = cosf(angle / 2.0f);
	float s = sinf(angle / 2.0f);

	result.w = c;
	result.x = axis.x * s;
	result.y = axis.y * s;
	result.z = axis.z * s;

	return Normalize(result);
}

inline Quaternion Quaternion::CreateFromRotationMatrix(const Matrix4x4 &matrix)
{
	Quaternion result;

	float n = matrix.m[_11] + matrix.m[_22] + matrix.m[_33];
	if (n > 0.0f)
	{
		float a = sqrtf(n + 1.0f);
		result.w = a / 2.0f;
		a = 0.5f / a;
		result.x = (matrix.m[_32] - matrix.m[_23]) * a;
		result.y = (matrix.m[_13] - matrix.m[_31]) * a;
		result.z = (matrix.m[_21] - matrix.m[_12]) * a;
	}
	else if ((matrix.m[_11] >= matrix.m[_22]) && (matrix.m[_11] >= matrix.m[_33]))
	{
		float a = sqrtf(1.0f + matrix.m[_11] - matrix.m[_22] - matrix.m[_33]);
		float b = 0.5f / a;

		result.x = 0.5f * a;
		result.y = (matrix.m[_21] + matrix.m[_12]) * b;
		result.z = (matrix.m[_31] + matrix.m[_13]) * b;
		result.w = (matrix.m[_32] - matrix.m[_23]) * b;
	}
	else if (matrix.m[_22] > matrix.m[_33])
	{
		float a = sqrtf(1.0f + matrix.m[_22] - matrix.m[_11] - matrix.m[_33]);
		float b = 0.5f / a;

		result.x = (matrix.m[_12] + matrix.m[_21]) * b;
		result.y = 0.5f * a;
		result.z = (matrix.m[_23] + matrix.m[_32]) * b;
		result.w = (matrix.m[_13] - matrix.m[_31]) * b;
	}
	else
	{
		float a = sqrtf(1.0f + matrix.m[_33] - matrix.m[_11] - matrix.m[_22]);
		float b = 0.5f / a;

		result.x = (matrix.m[_13] + matrix.m[_31]) * b;
		result.y = (matrix.m[_23] + matrix.m[_32]) * b;
		result.z = 0.5f * a;
		result.w = (matrix.m[_21] - matrix.m[_12]) * b;
	}

	return Normalize(result);
}

inline Quaternion Quaternion::Cross(const Quaternion &a, const Quaternion &b)
{
	return a * b;
}

inline float Quaternion::Dot(const Quaternion &a, const Quaternion &b)
{
	return (a.w * b.w) + (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline void Quaternion::ExtractAxisAngle(const Quaternion &q, float &angle, Vector3 &axis)
{
	angle = 2.0f * acosf(q.w);
	float n = sqrtf(1.0f - (q.w * q.w));
	if (n > 0.0001f)
		axis = q.GetVector() / n;
	else
		axis = X_AXIS;
}

inline Quaternion Quaternion::Inverse(const Quaternion &q)
{
	float inverseSquaredLength = 1.0f / LengthSquared(q);
	return Quaternion(
		-q.x * inverseSquaredLength,
		-q.y * inverseSquaredLength,
		-q.z * inverseSquaredLength,
		q.w * inverseSquaredLength
		);
}

inline float Quaternion::Length(const Quaternion &q)
{
	return sqrtf((q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z));
}

inline float Quaternion::LengthSquared(const Quaternion &q)
{
	return (q.w * q.w) + (q.x * q.x) + (q.y * q.y) + (q.z * q.z);
}

inline Quaternion Quaternion::Lerp(const Quaternion &a, const Quaternion &b, float interpolation)
{
	return (a * (1.0f - interpolation)) + (b * interpolation);
}

inline Quaternion Quaternion::Normalize(const Quaternion &q)
{
	float inverseLength = 1.0f / Length(q);
	return Quaternion(
		q.x * inverseLength,
		q.y * inverseLength,
		q.z * inverseLength,
		q.w * inverseLength
		);
}

inline Quaternion Quaternion::Slerp(const Quaternion &a, const Quaternion &b, float interpolation)
{
	if (LengthSquared(a) == 0.0f)
	{
		if (LengthSquared(b) == 0.0f)
			return IDENTITY_QUATERNION;
		else
			return b;
	}
	else if (LengthSquared(b) == 0.0f)
		return a;

	Quaternion q1 = a;
	Quaternion q2 = b;

	float cosHalfAngle = q1.w * q2.w + Vector3::Dot(q1.GetVector(), q2.GetVector());

	if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
		return q1;
	else if (cosHalfAngle < 0.0f)
	{
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		q2.w = -q2.w;
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	if (cosHalfAngle < 0.99f)
	{
		float halfAngle = acosf(cosHalfAngle);
		float sinHalfAngle = sinf(halfAngle);
		float oneOverSinHalfAngle = 1.0f / sinHalfAngle;
		blendA = sinf(halfAngle * (1.0f - interpolation)) * oneOverSinHalfAngle;
		blendB = sinf(halfAngle * interpolation) * oneOverSinHalfAngle;
	}
	else
	{
		blendA = 1.0f - interpolation;
		blendB = interpolation;
	}

	Quaternion result(q1.GetVector() * blendA + q2.GetVector() * blendB, q1.w * blendA + q2.w * blendB);
	if (LengthSquared(result) > 0.0f)
		return Normalize(result);
	else
		return IDENTITY_QUATERNION;
}

inline Quaternion operator+(const Quaternion &left, const Quaternion &right)
{
	return Quaternion(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z,
		left.w + right.w
	);
}

inline Quaternion &operator+=(Quaternion &left, const Quaternion &right)
{
	left = left + right;
	return left;
}

inline Quaternion operator-(const Quaternion &left, const Quaternion &right)
{
	return Quaternion(
		left.x - right.x,
		left.y - right.y,
		left.z - right.z,
		left.w - right.w
	);
}

inline Quaternion &operator-=(Quaternion &left, const Quaternion &right)
{
	left = left - right;
	return left;
}

inline Quaternion operator*(const Quaternion &left, float right)
{
	return Quaternion(
		left.x * right,
		left.y * right,
		left.z * right,
		left.w * right
	);
}

inline Quaternion &operator*=(Quaternion &left, float right)
{
	left = left * right;
	return left;
}

inline Quaternion operator*(const Quaternion &left, const Quaternion &right)
{
	return Quaternion(
		(left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y),
		(left.w * right.y) + (left.y * right.w) + (left.z * right.x) - (left.x * right.z),
		(left.w * right.z) + (left.z * right.w) + (left.x * right.y) - (left.y * right.x),
		(left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z)
	);
}

inline Quaternion &operator*=(Quaternion &left, const Quaternion &right)
{
	left = left * right;
	return left;
}

inline Quaternion operator/(const Quaternion &left, float right)
{
	return Quaternion(
		left.x / right,
		left.y / right,
		left.z / right,
		left.w / right
	);
}

inline Quaternion &operator/=(Quaternion &left, float right)
{
	left = left / right;
	return left;
}

inline Vector3 operator*(const Vector3 &left, const Quaternion &right)
{
	return Vector3(
		((left.x * ((1.0f - (right.y * (right.y + right.y))) - (right.z * (right.z + right.z)))) + (left.y * ((right.x * (right.y + right.y)) - (right.w * (right.z + right.z))))) + (left.z * ((right.x * (right.z + right.z)) + (right.w * (right.y + right.y)))),
		((left.x * ((right.x * (right.y + right.y)) + (right.w * (right.z + right.z)))) + (left.y * ((1.0f - (right.x * (right.x + right.x))) - (right.z * (right.z + right.z))))) + (left.z * ((right.y * (right.z + right.z)) - (right.w * (right.x + right.x)))),
		((left.x * ((right.x * (right.z + right.z)) - (right.w * (right.y + right.y)))) + (left.y * ((right.y * (right.z + right.z)) + (right.w * (right.x + right.x))))) + (left.z * ((1.0f - (right.x * (right.x + right.x))) - (right.y * (right.y + right.y))))
	);
}

#endif

