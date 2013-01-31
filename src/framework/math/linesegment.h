#ifndef __FRAMEWORK_MATH_LINESEGMENT_H_INCLUDED__
#define __FRAMEWORK_MATH_LINESEGMENT_H_INCLUDED__

#include "vector3.h"
#include "ray.h"

/**
 * Represents a line segment in 3D space.
 */
struct LineSegment
{
	/**
	 * Creates an uninitialized line segment.
	 */
	LineSegment();

	/**
	 * Creates a line segment with the specified properties.
	 * @param a beginning of the line segment
	 * @param b ending of the line segment
	 */
	LineSegment(const Vector3 &a, const Vector3 &b);

	/**
	 * Creates a line segment with the specified properties.
	 * @param ax X coordinate of the beginning of the line segment
	 * @param ay Y coordinate of the beginning of the line segment
	 * @param az Z coordinate of the beginning of the line segment
	 * @param bx X coordinate of the ending of the line segment
	 * @param by Y coordinate of the ending of the line segment
	 * @param bz Z coordinate of the ending of the line segment
	 */
	LineSegment(float ax, float ay, float az, float bx, float by, float bz);

	/**
	 * Creates a line segment with the specified properties.
	 * @param ray a ray that this line segment will be based off of
	 * @param length the length from the origin of the ray that will be
	 *               used to determine the end point of this line segment
	 */
	LineSegment(const Ray &ray, float length);

	/**
	 * Returns the direction the line segment is pointing if we assume
	 * that point "a" is the start position and "b" is the end position.
	 * @return Vector3 the direction that the line segment is pointing in
	 *                 (not a normalized vector)
	 */
	Vector3 GetDirection() const;

	Vector3 a;
	Vector3 b;
};

inline LineSegment::LineSegment()
{
	a = ZERO_VECTOR;
	b = ZERO_VECTOR;
}

inline LineSegment::LineSegment(const Vector3 &a, const Vector3 &b)
{
	this->a = a;
	this->b = b;
}

inline LineSegment::LineSegment(float ax, float ay, float az, float bx, float by, float bz)
{
	this->a = Vector3(ax, ay, bz);
	this->b = Vector3(bx, by, bz);
}

inline LineSegment::LineSegment(const Ray &ray, float length)
{
	this->a = ray.position;
	this->b = ray.GetPositionAt(length);
}

inline Vector3 LineSegment::GetDirection() const
{
	return b - a;
}

#endif
