#ifndef __FRAMEWORK_MATH_POINT2_H_INCLUDED__
#define __FRAMEWORK_MATH_POINT2_H_INCLUDED__
#include "../common.h"

/**
 * Represents a point in 2D space using integers for the coordinates.
 */
struct Point2
{
	/**
	 * Creates an uninitialized point.
	 */
	Point2();

	/**
	 * Creates a point with the specified properties.
	 * @param x X coordinate of the point
	 * @param y Y coordinate of the point
	 */
	Point2(int x, int y);

	/**
	 * Creates a point with the specified properties.
	 * @param p an array with 2 values to use as the point's X and Y coordinates
	 */
	Point2(const int *p);

	/**
	 * Sets new coordinates for this point.
	 * @param x new X coordinate of the point
	 * @param y new Y coordinate of the point
	 */
	void Set(int x, int y);

	/**
	 * Sets new coordinates for this point.
	 * @param p another point whose coordinates are to be copied and used for this point
	 */
	void Set(const Point2 &p);

	int x;
	int y;
};

#define ZERO_POINT2 Point2(0, 0)

bool operator==(const Point2 &left, const Point2 &right);
Point2 operator-(const Point2 &left);
Point2 operator+(const Point2 &left, const Point2 &right);
Point2 &operator+=(Point2 &left, const Point2 &right);
Point2 operator-(const Point2 &left, const Point2 &right);
Point2 &operator-=(Point2 &left, const Point2 &right);
Point2 operator*(const Point2 &left, int right);
Point2 &operator*=(Point2 &left, int right);
Point2 operator/(const Point2 &left, int right);
Point2 &operator/=(Point2 &left, int right);
Point2 operator*(const Point2 &left, const Point2 &right);
Point2 &operator*=(Point2 &left, const Point2 &right);
Point2 operator/(const Point2 &left, const Point2 &right);
Point2 &operator/=(Point2 &left, const Point2 &right);

inline Point2::Point2()
{
}

inline Point2::Point2(int x, int y)
{
	this->x = x;
	this->y = y;
}

inline Point2::Point2(const int *p)
{
	this->x = p[0];
	this->y = p[1];
}

inline void Point2::Set(int x, int y)
{
	this->x = x;
	this->y = y;
}

inline void Point2::Set(const Point2 &p)
{
	this->x = p.x;
	this->y = p.y;
}

inline bool operator==(const Point2 &left, const Point2 &right)
{
	return (left.x == right.x && left.y == right.y);
}

inline Point2 operator-(const Point2 &left)
{
	return Point2(-left.x, -left.y);
}

inline Point2 operator+(const Point2 &left, const Point2 &right)
{
	return Point2(left.x + right.x, left.y + right.y);
}

inline Point2 &operator+=(Point2 &left, const Point2 &right)
{
	left.x += right.x;
	left.y += right.y;

	return left;
}

inline Point2 operator-(const Point2 &left, const Point2 &right)
{
	return Point2(left.x - right.x, left.y - right.y);
}

inline Point2 &operator-=(Point2 &left, const Point2 &right)
{
	left.x -= right.x;
	left.y -= right.y;

	return left;
}

inline Point2 operator*(const Point2 &left, int right)
{
	return Point2(left.x * right, left.y * right);
}

inline Point2 &operator*=(Point2 &left, int right)
{
	left.x *= right;
	left.y *= right;

	return left;
}

inline Point2 operator/(const Point2 &left, int right)
{
	return Point2(left.x / right, left.y / right);
}

inline Point2 &operator/=(Point2 &left, int right)
{
	left.x /= right;
	left.y /= right;

	return left;
}

inline Point2 operator*(const Point2 &left, const Point2 &right)
{
	return Point2(left.x * right.x, left.y * right.y);
}

inline Point2 &operator*=(Point2 &left, const Point2 &right)
{
	left.x *= right.x;
	left.y *= right.y;

	return left;
}

inline Point2 operator/(const Point2 &left, const Point2 &right)
{
	return Point2(left.x / right.x, left.y / right.y);
}

inline Point2 &operator/=(Point2 &left, const Point2 &right)
{
	left.x /= right.x;
	left.y /= right.y;

	return left;
}

#endif

