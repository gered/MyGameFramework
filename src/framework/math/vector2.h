#ifndef __FRAMEWORK_VECTOR2_H_INCLUDED__
#define __FRAMEWORK_VECTOR2_H_INCLUDED__

#include "../common.h"
#include <math.h>

/**
 * Represents a 2D vector and provides common methods for vector math. 
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>http://www.dhpoware.com/source/mathlib.html</li>
 * </ul>
 */
struct Vector2
{
	/**
	 * Creates an uninitialized vector.
	 */
	Vector2();

	/**
	 * Creates a vector with the specified properties.
	 * @param x X coordinate of the vector
	 * @param y Y coordinate of the vector
	 */
	Vector2(float x, float y);

	/**
	 * Creates a vector with the specified properties.
	 * @param v an array with 2 values that will be used as this vector's components
	 */
	Vector2(const float *v);

	/**
	 * Sets new coordinates for this vector.
	 * @param x new X coordinate of the vector
	 * @param y new Y coordinate of the vector
	 */
	void Set(float x, float y);

	/**
	 * Sets new coordinates for this vector.
	 * @param v another vector whose coordinates are to be copied and used for this vector
	 */
	void Set(const Vector2 &v);

	/**
	 * Calculates the distance between two points.
	 * @param a the first point
	 * @param b the second point
	 * @return the distance between both points
	 */
	static float Distance(const Vector2 &a, const Vector2 &b);

	/**
	 * Calculates the squared distance between two points (the distance minus
	 * the sqrt call).
	 * @param a the first point
	 * @param b the second point
	 * @return the squared distance between both points
	 */
	static float DistanceSquared(const Vector2 &a, const Vector2 &b);
	
	/**
	 * Computes the dot product of 2 vectors.
	 * @param a first vector
	 * @param b second vector
	 * @return the dot product
	 */
	static float Dot(const Vector2 &a, const Vector2 &b);

	/**
	 * Returns the length (magnitude) of a vector.
	 * @param v vector to calculate the length of
	 * @return the vector length
	 */
	static float Length(const Vector2 &v);

	/**
	 * Returns the squared length of a vector (the magnitude minus the sqrt 
	 * call).
	 * @param v vector to calculate the squared length of
	 * @return squared length of the vector
	 */
	static float LengthSquared(const Vector2 &v);

	/**
	 * Normalizes a vector.
	 * @param v vector to normalize
	 * @return the normalized vector
	 */
	static Vector2 Normalize(const Vector2 &v);

	/**
	 * Adjusts a vector so that it's length is equal to the given length.
	 * @param v the original vector to be adjusted
	 * @param length desired vector length (magnitude)
	 * @return the resulting vector after it's length has been converted to the 
	 *         desired amount
	 */
	static Vector2 SetLength(const Vector2 &v, float length);

	float x;
	float y;
};

#define ZERO_VECTOR2 Vector2(0.0f, 0.0f)

bool operator==(const Vector2 &left, const Vector2 &right);
Vector2 operator-(const Vector2 &left);
Vector2 operator+(const Vector2 &left, const Vector2 &right);
Vector2 &operator+=(Vector2 &left, const Vector2 &right);
Vector2 operator-(const Vector2 &left, const Vector2 &right);
Vector2 &operator-=(Vector2 &left, const Vector2 &right);
Vector2 operator*(const Vector2 &left, float right);
Vector2 &operator*=(Vector2 &left, float right);
Vector2 operator/(const Vector2 &left, float right);
Vector2 &operator/=(Vector2 &left, float right);
Vector2 operator*(const Vector2 &left, const Vector2 &right);
Vector2 &operator*=(Vector2 &left, const Vector2 &right);
Vector2 operator/(const Vector2 &left, const Vector2 &right);
Vector2 &operator/=(Vector2 &left, const Vector2 &right);

inline Vector2::Vector2()
{
}

inline Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

inline Vector2::Vector2(const float *v)
{
	x = v[0];
	y = v[1];
}

inline void Vector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

inline void Vector2::Set(const Vector2 &v)
{
	this->x = v.x;
	this->y = v.y;
}

inline float Vector2::Distance(const Vector2 &a, const Vector2 &b)
{
	return sqrtf(
		((b.x - a.x) * (b.x - a.x)) + 
		((b.y - a.y) * (b.y - a.y))
		);
}

inline float Vector2::DistanceSquared(const Vector2 &a, const Vector2 &b)
{
	return 
		((b.x - a.x) * (b.x - a.x)) + 
		((b.y - a.y) * (b.y - a.y));
}

inline float Vector2::Dot(const Vector2 &a, const Vector2 &b)
{
	return 
		(a.x * b.y) + 
		(a.y * b.y);
}

inline float Vector2::Length(const Vector2 &v)
{
	return sqrtf(
		(v.x * v.x) + 
		(v.y * v.y)
		);
}

inline float Vector2::LengthSquared(const Vector2 &v)
{
	return 
		(v.x * v.x) + 
		(v.y * v.y);
}

inline Vector2 Vector2::Normalize(const Vector2 &v)
{
	float inverseLength = 1.0f / Length(v);
	return Vector2(
		v.x * inverseLength,
		v.y * inverseLength
		);
}

inline Vector2 Vector2::SetLength(const Vector2 &v, float length)
{
	return v * (length / Length(v));
}

inline bool operator==(const Vector2 &left, const Vector2 &right)
{
	return (left.x == right.x && left.y == right.y);
}

inline Vector2 operator-(const Vector2 &left)
{
	return Vector2(-left.x, -left.y);
}

inline Vector2 operator+(const Vector2 &left, const Vector2 &right)
{
	return Vector2(left.x + right.x, left.y + right.y);
}

inline Vector2 &operator+=(Vector2 &left, const Vector2 &right)
{
	left.x += right.x;
	left.y += right.y;

	return left;
}

inline Vector2 operator-(const Vector2 &left, const Vector2 &right)
{
	return Vector2(left.x - right.x, left.y - right.y);
}

inline Vector2 &operator-=(Vector2 &left, const Vector2 &right)
{
	left.x -= right.x;
	left.y -= right.y;

	return left;
}

inline Vector2 operator*(const Vector2 &left, float right)
{
	return Vector2(left.x * right, left.y * right);
}

inline Vector2 &operator*=(Vector2 &left, float right)
{
	left.x *= right;
	left.y *= right;

	return left;
}

inline Vector2 operator/(const Vector2 &left, float right)
{
	return Vector2(left.x / right, left.y / right);
}

inline Vector2 &operator/=(Vector2 &left, float right)
{
	left.x /= right;
	left.y /= right;

	return left;
}

inline Vector2 operator*(const Vector2 &left, const Vector2 &right)
{
	return Vector2(left.x * right.x, left.y * right.y);
}

inline Vector2 &operator*=(Vector2 &left, const Vector2 &right)
{
	left.x *= right.x;
	left.y *= right.y;

	return left;
}

inline Vector2 operator/(const Vector2 &left, const Vector2 &right)
{
	return Vector2(left.x / right.x, left.y / right.y);
}

inline Vector2 &operator/=(Vector2 &left, const Vector2 &right)
{
	left.x /= right.x;
	left.y /= right.y;

	return left;
}

#endif

