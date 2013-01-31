#ifndef __FRAMEWORK_VECTOR4_H_INCLUDED__
#define __FRAMEWORK_VECTOR4_H_INCLUDED__

#include "../common.h"
#include <math.h>

/**
 * Represents a 4D vector and provides common methods for vector math.
 * Referenced/based on code from:
 */
struct Vector4
{
	/**
	 * Creates an uninitialized vector.
	 */
	Vector4();
	
	/**
	 * Creates a vector with the specified properties.
	 * @param x X coordinate of the vector
	 * @param y Y coordinate of the vector
	 * @param z Z coordinate of the vector
	 * @param w W coordinate of the vector
	 */
	Vector4(float x, float y, float z, float w);
	
	/**
	 * Creates a vector with the specified properties.
	 * @param v an array with 4 values that will be used as this vector's components
	 */
	Vector4(const float *v);
	
	/**
	 * Sets new coordinates for this vector.
	 * @param x new X coordinate of the vector
	 * @param y new Y coordinate of the vector
	 * @param z new Z coordinate of the vector
	 * @param w new W coordinate of the vector
	 */
	void Set(float x, float y, float z, float w);
	
	/**
	 * Sets new coordinates for this vector.
	 * @param v another vector whose coordinates are to be copied and used for this vector
	 */
	void Set(const Vector4 &v);
		
	/**
	 * Calculates the distance between two points.
	 * @param a the first point
	 * @param b the second point
	 * @return the distance between both points
	 */
	static float Distance(const Vector4 &a, const Vector4 &b);
	
	/**
	 * Calculates the squared distance between two points (the distance minus
	 * the sqrt call).
	 * @param a the first point
	 * @param b the second point
	 * @return the squared distance between both points
	 */
	static float DistanceSquared(const Vector4 &a, const Vector4 &b);
	
	/**
	 * Computes the dot product of 2 vectors.
	 * @param a first vector
	 * @param b second vector
	 * @return the dot product
	 */
	static float Dot(const Vector4 &a, const Vector4 &b);
		
	/**
	 * Returns the length (magnitude) of a vector.
	 * @param v vector to calculate the length of
	 * @return the vector length
	 */
	static float Length(const Vector4 &v);
	
	/**
	 * Returns the squared length of a vector (the magnitude minus the sqrt 
	 * call).
	 * @param v vector to calculate the squared length of
	 * @return squared length of the vector
	 */
	static float LengthSquared(const Vector4 &v);
	
	/**
	 * Linearly interpolates between two vectors.
	 * @param a the first vector
	 * @param b the second vector
	 * @param interpolation the amount to interpolate
	 * @return Vector3 the interpolated vector
	 */
	static Vector4 Lerp(const Vector4 &a, const Vector4 &b, float interpolation);
	
	/**
	 * Normalizes a vector.
	 * @param v vector to normalize
	 * @return the normalized vector
	 */
	static Vector4 Normalize(const Vector4 &v);
	
	/**
	 * Adjusts a vector so that it's length is equal to the given 
	 * length.
	 * @param v the original vector to be adjusted
	 * @param length desired vector length (magnitude)
	 * @return the resulting vector after it's length has been converted to the 
	 *         desired amount
	 */
	static Vector4 SetLength(const Vector4 &v, float length);
	
	float x;
	float y;
	float z;
	float w;
};

bool operator==(const Vector4 &left, const Vector4 &right);
Vector4 operator-(const Vector4 &left);
Vector4 operator+(const Vector4 &left, const Vector4 &right);
Vector4 &operator+=(Vector4 &left, const Vector4 &right);
Vector4 operator-(const Vector4 &left, const Vector4 &right);
Vector4 &operator-=(Vector4 &left, const Vector4 &right);
Vector4 operator*(const Vector4 &left, float right);
Vector4 &operator*=(Vector4 &left, float right);
Vector4 operator/(const Vector4 &left, float right);
Vector4 &operator/=(Vector4 &left, float right);
Vector4 operator*(const Vector4 &left, const Vector4 &right);
Vector4 &operator*=(Vector4 &left, const Vector4 &right);
Vector4 operator/(const Vector4 &left, const Vector4 &right);
Vector4 &operator/=(Vector4 &left, const Vector4 &right);

inline Vector4::Vector4()
{
}

inline Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline Vector4::Vector4(const float *v)
{
	x = v[0];
	y = v[1];
	z = v[2];
	w = v[3];
}

inline void Vector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline void Vector4::Set(const Vector4 &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

inline float Vector4::Distance(const Vector4 &a, const Vector4 &b)
{
	return Vector4::Length(b - a);
}

inline float Vector4::DistanceSquared(const Vector4 &a, const Vector4 &b)
{
	return Vector4::LengthSquared(b - a);
}

inline float Vector4::Dot(const Vector4 &a, const Vector4 &b)
{
	return
		(a.x * b.x) + 
		(a.y * b.y) + 
		(a.z * b.z) +
		(a.w * b.w);
}

inline float Vector4::Length(const Vector4 &v)
{
	return sqrtf(
		(v.x * v.x) + 
		(v.y * v.y) + 
		(v.z * v.z) +
		(v.w * v.w)
		);
}

inline float Vector4::LengthSquared(const Vector4 &v)
{
	return 
		(v.x * v.x) + 
		(v.y * v.y) + 
		(v.z * v.z) +
		(v.w * v.w);
}

inline Vector4 Vector4::Lerp(const Vector4 &a, const Vector4 &b, float interpolation)
{
	return a + (b - a) * interpolation;
}

inline Vector4 Vector4::Normalize(const Vector4 &v)
{
	float inverseLength = 1.0f / Length(v);
	return Vector4(
		v.x * inverseLength,
		v.y * inverseLength,
		v.z * inverseLength,
		v.w * inverseLength
		);
}

inline Vector4 Vector4::SetLength(const Vector4 &v, float length)
{
	return v * (length / Length(v));
}

inline bool operator==(const Vector4 &left, const Vector4 &right)
{
	return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);
}

inline Vector4 operator-(const Vector4 &left)
{
	return Vector4(-left.x, -left.y, -left.z, -left.w);
}

inline Vector4 operator+(const Vector4 &left, const Vector4 &right)
{
	return Vector4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
}

inline Vector4 &operator+=(Vector4 &left, const Vector4 &right)
{
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	left.w += right.w;
	
	return left;
}

inline Vector4 operator-(const Vector4 &left, const Vector4 &right)
{
	return Vector4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
}

inline Vector4 &operator-=(Vector4 &left, const Vector4 &right)
{
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;
	left.w -= right.w;
	
	return left;
}

inline Vector4 operator*(const Vector4 &left, float right)
{
	return Vector4(left.x * right, left.y * right, left.z * right, left.w * right);
}

inline Vector4 &operator*=(Vector4 &left, float right)
{
	left.x *= right;
	left.y *= right;
	left.z *= right;
	left.w *= right;
	
	return left;
}

inline Vector4 operator/(const Vector4 &left, float right)
{
	return Vector4(left.x / right, left.y / right, left.z / right, left.w / right);
}

inline Vector4 &operator/=(Vector4 &left, float right)
{
	left.x /= right;
	left.y /= right;
	left.z /= right;
	left.w /= right;
	
	return left;
}

inline Vector4 operator*(const Vector4 &left, const Vector4 &right)
{
	return Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
}

inline Vector4 &operator*=(Vector4 &left, const Vector4 &right)
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;
	left.w *= right.w;
	
	return left;
}

inline Vector4 operator/(const Vector4 &left, const Vector4 &right)
{
	return Vector4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
}

inline Vector4 &operator/=(Vector4 &left, const Vector4 &right)
{
	left.x /= right.x;
	left.y /= right.y;
	left.z /= right.z;
	left.w /= right.w;
	
	return left;
}

#endif

