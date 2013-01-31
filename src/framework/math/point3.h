#ifndef __FRAMEWORK_MATH_POINT3_H_INCLUDED__
#define __FRAMEWORK_MATH_POINT3_H_INCLUDED__

#include "../common.h"

/**
 * Represents a point in 3D space using integers for the coordinates.
 */
struct Point3
{
	/**
	 * Creates an uninitialized point.
	 */
	Point3();

	/**
	 * Creates a point with the specified properties.
	 * @param x X coordinate of the point
	 * @param y Y coordinate of the point
	 * @param z Z coordinate of the point
	 */
	Point3(int32_t x, int32_t y, int32_t z);

	/**
	 * Creates a point with the specified properties.
	 * @param p an array with 3 values to use as the point's X, Y and Z coordinates
	 */
	Point3(const int32_t *p);

	/**
	 * Sets new coordinates for this point.
	 * @param x new X coordinate of the point
	 * @param y new Y coordinate of the point
	 * @param z new Z coordinate of the point
	 */
	void Set(int32_t x, int32_t y, int32_t z);

	/**
	 * Sets new coordinates for this point.
	 * @param p another point whose coordinates are to be copied and used for this point
	 */
	void Set(const Point3 &p);

	int32_t x;
	int32_t y;
	int32_t z;
};

#define ZERO_POINT Point3(0, 0, 0)

bool operator==(const Point3 &left, const Point3 &right);
Point3 operator-(const Point3 &left);
Point3 operator+(const Point3 &left, const Point3 &right);
Point3 &operator+=(Point3 &left, const Point3 &right);
Point3 operator-(const Point3 &left, const Point3 &right);
Point3 &operator-=(Point3 &left, const Point3 &right);
Point3 operator*(const Point3 &left, int32_t right);
Point3 &operator*=(Point3 &left, int32_t right);
Point3 operator/(const Point3 &left, int32_t right);
Point3 &operator/=(Point3 &left, int32_t right);
Point3 operator*(const Point3 &left, const Point3 &right);
Point3 &operator*=(Point3 &left, const Point3 &right);
Point3 operator/(const Point3 &left, const Point3 &right);
Point3 &operator/=(Point3 &left, const Point3 &right);

inline Point3::Point3()
{
}

inline Point3::Point3(int32_t x, int32_t y, int32_t z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline Point3::Point3(const int32_t *p)
{
	this->x = p[0];
	this->y = p[1];
	this->z = p[2];
}

inline void Point3::Set(int32_t x, int32_t y, int32_t z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline void Point3::Set(const Point3 &p)
{
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;
}

inline bool operator==(const Point3 &left, const Point3 &right)
{
	return (left.x == right.x && left.y == right.y && left.z == right.z);
}

inline Point3 operator-(const Point3 &left)
{
	return Point3(-left.x, -left.y, -left.z);
}

inline Point3 operator+(const Point3 &left, const Point3 &right)
{
	return Point3(left.x + right.x, left.y + right.y, left.z + right.z);
}

inline Point3 &operator+=(Point3 &left, const Point3 &right)
{
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;

	return left;
}

inline Point3 operator-(const Point3 &left, const Point3 &right)
{
	return Point3(left.x - right.x, left.y - right.y, left.z - right.z);
}

inline Point3 &operator-=(Point3 &left, const Point3 &right)
{
	left.x -= right.x;
	left.y -= right.y;
	left.z -= right.z;

	return left;
}

inline Point3 operator*(const Point3 &left, int32_t right)
{
	return Point3(left.x * right, left.y * right, left.z * right);
}

inline Point3 &operator*=(Point3 &left, int32_t right)
{
	left.x *= right;
	left.y *= right;
	left.z *= right;

	return left;
}

inline Point3 operator/(const Point3 &left, int32_t right)
{
	return Point3(left.x / right, left.y / right, left.z / right);
}

inline Point3 &operator/=(Point3 &left, int32_t right)
{
	left.x /= right;
	left.y /= right;
	left.z /= right;

	return left;
}

inline Point3 operator*(const Point3 &left, const Point3 &right)
{
	return Point3(left.x * right.x, left.y * right.y, left.z * right.z);
}

inline Point3 &operator*=(Point3 &left, const Point3 &right)
{
	left.x *= right.x;
	left.y *= right.y;
	left.z *= right.z;

	return left;
}

inline Point3 operator/(const Point3 &left, const Point3 &right)
{
	return Point3(left.x / right.x, left.y / right.y, left.z / right.z);
}

inline Point3 &operator/=(Point3 &left, const Point3 &right)
{
	left.x /= right.x;
	left.y /= right.y;
	left.z /= right.z;

	return left;
}

#endif

