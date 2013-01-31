#ifndef __FRAMEWORK_VECTOR3_H_INCLUDED__
#define __FRAMEWORK_VECTOR3_H_INCLUDED__

#include "../common.h"
#include <math.h>

/**
 * <p>Represents a 3D vector and provides common methods for vector math.</p>
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>http://www.dhpoware.com/source/mathlib.html</li>
 * <li>http://www.peroxide.dk/papers/collision/collision.pdf</li>
 * </ul>
 */
struct Vector3
{
	/**
	 * Creates an uninitialized vector.
	 */
	Vector3();

	/**
	 * Creates a vector with the specified properties.
	 * @param x X coordinate of the vector
	 * @param y Y coordinate of the vector
	 * @param z Z coordinate of the vector
	 */
	Vector3(float x, float y, float z);

	/**
	 * Creates a vector with the specified properties.
	 * @param v an array with 3 values that will be used as this vector's components
	 */
	Vector3(const float *v);

	/**
	 * Sets new coordinates for this vector.
	 * @param x new X coordinate of the vector
	 * @param y new Y coordinate of the vector
	 * @param z new Z coordinate of the vector
	 */
	void Set(float x, float y, float z);

	/**
	 * Sets new coordinates for this vector.
	 * @param v another vector whose coordinates are to be copied and used for this vector
	 */
	void Set(const Vector3 &v);

	/**
	 * Computes the cross product of 2 vectors.
	 * @param a first vector
	 * @param b second vector
	 * @return the cross product
	 */
	static Vector3 Cross(const Vector3 &a, const Vector3 &b);

	/**
	 * Calculates the distance between two points.
	 * @param a the first point
	 * @param b the second point
	 * @return the distance between both points
	 */
	static float Distance(const Vector3 &a, const Vector3 &b);
	
	/**
	 * Calculates the squared distance between two points (the distance minus
	 * the sqrt call).
	 * @param a the first point
	 * @param b the second point
	 * @return the squared distance between both points
	 */
	static float DistanceSquared(const Vector3 &a, const Vector3 &b);

	/**
	 * Computes the dot product of 2 vectors.
	 * @param a first vector
	 * @param b second vector
	 * @return the dot product
	 */
	static float Dot(const Vector3 &a, const Vector3 &b);

	/**
	 * Checks if a given point lies inside a triangle or not.
	 * @param point point to test
	 * @param a first vector of the triangle
	 * @param b second vector of the triangle
	 * @param c third vector of the triangle
	 * @return TRUE if the point lies inside the triangle
	 */
	static BOOL IsPointInTriangle(const Vector3 &point, const Vector3 &a, const Vector3 &b, const Vector3 &c);

	/**
	 * Returns the length (magnitude) of a vector.
	 * @param v vector to calculate the length of
	 * @return the vector length
	 */
	static float Length(const Vector3 &v);

	/**
	 * Returns the squared length of a vector (the magnitude minus the sqrt 
	 * call).
	 * @param v vector to calculate the squared length of
	 * @return squared length of the vector
	 */
	static float LengthSquared(const Vector3 &v);

	/**
	 * Linearly interpolates between two vectors.
	 * @param a the first vector
	 * @param b the second vector
	 * @param interpolation the amount to interpolate
	 * @return Vector3 the interpolated vector
	 */
	static Vector3 Lerp(const Vector3 &a, const Vector3 &b, float interpolation);

	/**
	 * Normalizes a vector.
	 * @param v vector to normalize
	 * @return the normalized vector
	 */
	static Vector3 Normalize(const Vector3 &v);

	/**
	 * Adjusts a vector so that it's length is equal to the given 
	 * length.
	 * @param v the original vector to be adjusted
	 * @param length desired vector length (magnitude)
	 * @return the resulting vector after it's length has been converted to the 
	 *         desired amount
	 */
	static Vector3 SetLength(const Vector3 &v, float length);

	/**
	* Calculates a normal vector for the given 3 vectors making up a 
	* triangle (counter clockwise order).
	* @param a first vertex
	* @param b second vertex
	* @param c third vertex 
	* @return normal vector for the triangle
	*/
	static Vector3 SurfaceNormal(const Vector3 &a, const Vector3 &b, const Vector3 &c);

	float x;
	float y;
	float z;
};

bool operator==(const Vector3 &left, const Vector3 &right);
Vector3 operator-(const Vector3 &left);
Vector3 operator+(const Vector3 &left, const Vector3 &right);
Vector3 &operator+=(Vector3 &left, const Vector3 &right);
Vector3 operator-(const Vector3 &left, const Vector3 &right);
Vector3 &operator-=(Vector3 &left, const Vector3 &right);
Vector3 operator*(const Vector3 &left, float right);
Vector3 &operator*=(Vector3 &left, float right);
Vector3 operator/(const Vector3 &left, float right);
Vector3 &operator/=(Vector3 &left, float right);
Vector3 operator*(const Vector3 &left, const Vector3 &right);
Vector3 &operator*=(Vector3 &left, const Vector3 &right);
Vector3 operator/(const Vector3 &left, const Vector3 &right);
Vector3 &operator/=(Vector3 &left, const Vector3 &right);

#define ZERO_VECTOR Vector3(0.0f, 0.0f, 0.0f)
#define X_AXIS Vector3(1.0f, 0.0f, 0.0f)
#define Y_AXIS Vector3(0.0f, 1.0f, 0.0f)
#define Z_AXIS Vector3(0.0f, 0.0f, 1.0f)
#define UP Vector3(0.0f, 1.0f, 0.0f)
#define DOWN Vector3(0.0f, -1.0f, 0.0f)
#define FORWARD Vector3(0.0f, 0.0f, -1.0f)
#define BACKWARD Vector3(0.0f, 0.0f, 1.0f)
#define LEFT Vector3(-1.0f, 0.0f, 0.0f)
#define RIGHT Vector3(1.0f, 0.0f, 0.0f)

inline Vector3::Vector3()
{
}

inline Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Vector3::Vector3(const float *v)
{
	x = v[0];
	y = v[1];
	z = v[2];
}

inline void Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline void Vector3::Set(const Vector3 &v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

inline Vector3 Vector3::Cross(const Vector3 &a, const Vector3 &b)
{
	return Vector3(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x)
		);
}

inline float Vector3::Distance(const Vector3 &a, const Vector3 &b)
{
	return sqrtf(
		((b.x - a.x) * (b.x - a.x)) + 
		((b.y - a.y) * (b.y - a.y)) + 
		((b.z - a.z) * (b.z - a.z))
		);
}

inline float Vector3::DistanceSquared(const Vector3 &a, const Vector3 &b)
{
	return 
		((b.x - a.x) * (b.x - a.x)) + 
		((b.y - a.y) * (b.y - a.y)) + 
		((b.z - a.z) * (b.z - a.z));
}

inline float Vector3::Dot(const Vector3 &a, const Vector3 &b)
{
	return
		(a.x * b.x) + 
		(a.y * b.y) + 
		(a.z * b.z);
}

inline BOOL Vector3::IsPointInTriangle(const Vector3 &point, const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	Vector3 v0 = c - a;
	Vector3 v1 = b - a;
	Vector3 v2 = point - a;
		
	float dot00 = (v0.x * v0.x) + (v0.y * v0.y) + (v0.z * v0.z);
	float dot01 = (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
	float dot02 = (v0.x * v2.x) + (v0.y * v2.y) + (v0.z * v2.z);
	float dot11 = (v1.x * v1.x) + (v1.y * v1.y) + (v1.z * v1.z);
	float dot12 = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
		
	float denom = dot00 * dot11 - dot01 * dot01;
	if (denom == 0)
		return FALSE;
		
	float u = (dot11 * dot02 - dot01 * dot12) / denom;
	float v = (dot00 * dot12 - dot01 * dot02) / denom;
		
	if (u >= 0 && v >= 0 && u + v <= 1)
		return TRUE;
	else
		return FALSE;
}

inline float Vector3::Length(const Vector3 &v)
{
	return sqrtf(
		(v.x * v.x) + 
		(v.y * v.y) + 
		(v.z * v.z)
		);
}

inline float Vector3::LengthSquared(const Vector3 &v)
{
	return 
		(v.x * v.x) + 
		(v.y * v.y) + 
		(v.z * v.z);
}

inline Vector3 Vector3::Lerp(const Vector3 &a, const Vector3 &b, float interpolation)
{
	return a + (b - a) * interpolation;
}

inline Vector3 Vector3::Normalize(const Vector3 &v)
{
	float inverseLength = 1.0f / Length(v);
	return Vector3(
		v.x * inverseLength,
		v.y * inverseLength,
		v.z * inverseLength
		);
}

inline Vector3 Vector3::SetLength(const Vector3 &v, float length)
{
	return v * (length / Length(v));
}

inline Vector3 Vector3::SurfaceNormal(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	return Normalize(Cross((b - a), (c - a)));
}

inline bool operator==(const Vector3 &left, const Vector3 &right)
{
	return (left.x == right.x && left.y == right.y && left.z == right.z);
}

inline Vector3 operator-(const Vector3 &left)
{
	return Vector3(-left.x, -left.y, -left.z);
}

inline Vector3 operator+(const Vector3 &left, const Vector3 &right)
{
	return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline Vector3 &operator+=(Vector3 &left, const Vector3 &right)
{
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;

	return left;
}

inline Vector3 operator-(const Vector3 &left, const Vector3 &right)
{
	return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline Vector3 &operator-=(Vector3 &left, const Vector3 &right)
{
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;

	return left;
}

inline Vector3 operator*(const Vector3 &left, float right)
{
	return Vector3(left.x * right, left.y * right, left.z * right);
}

inline Vector3 &operator*=(Vector3 &left, float right)
{
	left.x *= right;
	left.y *= right;
	left.z *= right;

	return left;
}

inline Vector3 operator/(const Vector3 &left, float right)
{
	return Vector3(left.x / right, left.y / right, left.z / right);
}

inline Vector3 &operator/=(Vector3 &left, float right)
{
	left.x /= right;
	left.y /= right;
	left.z /= right;

	return left;
}

inline Vector3 operator*(const Vector3 &left, const Vector3 &right)
{
	return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
}

inline Vector3 &operator*=(Vector3 &left, const Vector3 &right)
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;

	return left;
}

inline Vector3 operator/(const Vector3 &left, const Vector3 &right)
{
	return Vector3(left.x / right.x, left.y / right.y, left.z / right.z);
}

inline Vector3 &operator/=(Vector3 &left, const Vector3 &right)
{
	left.x /= right.x;
	left.y /= right.y;
	left.z /= right.z;

	return left;
}

#endif

