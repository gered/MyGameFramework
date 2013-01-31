#ifndef __FRAMEWORK_MATH_PLANE_H_INCLUDED__
#define __FRAMEWORK_MATH_PLANE_H_INCLUDED__

#include "../common.h"
#include <math.h>
#include "vector3.h"

enum PLANE_POINT_CLASSIFY
{
	IN_FRONT = 0,
	BEHIND = 1,
	ON_PLANE = 2
};

/**
 * <p>Represents a plane of the form:</p>
 * <code>Ax + Bx + Cx + D = 0</code>
 * <p>Provides common methods for working with planes.</p> 
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>http://www.peroxide.dk/papers/collision/collision.pdf</li>
 * </ul>
 */
struct Plane
{
/**
 * Creates an uninitialized plane.
 */
	Plane();

	/**
	 * Creates a plane with the specified properties.
	 */
	Plane(float a, float b, float c, float d);

	/**
	 * Creates a plane with the specified properties.
	 * @param p an array of 4 values that will be used for this plane's components
	 */
	Plane(const float *p);

	/**
	 * Creates a plane with the specified properties.
	 * @param origin a point that lies on the plane being created
	 * @param normal the normal of the plane
	 */
	Plane(const Vector3 &origin, const Vector3 &normal);

	/**
	 * Creates a plane with the specified properties.
	 * @param a first point of a triangle that lies on the plane
	 * @param b second point of a triangle that lies on the plane
	 * @param c third point of a triangle that lies on the plane
	 */
	Plane(const Vector3 &a, const Vector3 &b, const Vector3 &c);

	/**
	 * Determines if a point is in front, behind or located on a plane.
	 * @param plane plane to check
	 * @param point point to classify
	 * @return result of the classification test
	*/
	static PLANE_POINT_CLASSIFY ClassifyPoint(const Plane &plane, const Vector3 &point);

	/**
	 * Calculates the distance between a plane and a point. If the distance is 
	 * negative the point is located behind the plane.
	 * @param plane plane to check
	 * @param point point to measure to
	 * @return the distance between the plane and the point
	 */
	static float DistanceBetween(const Plane &plane, const Vector3 &point);

	/**
	 * Determines if a direction is point roughly towards the front side of a 
	 * plane (i.e. theta is in the range 90 - 180).
	 * @param plane plane to check
	 * @param direction direction to classify
	 * @return true if the direction is pointing towards the front side of the 
	 *         plane
	 */
	static BOOL IsFrontFacingTo(const Plane &plane, const Vector3 &direction);

	/**
	 * Normalize a plane.
	 * @param plane plane to normalize
	 * @return the normalized plane
	 */
	static Plane Normalize(const Plane &plane);

	Vector3 normal;
	float d;
};

inline Plane::Plane()
{
}

inline Plane::Plane(float a, float b, float c, float d)
{
	normal.x = a;
	normal.y = b;
	normal.z = c;
	this->d = d;
}

inline Plane::Plane(const float *p)
{
	normal.x = p[0];
	normal.y = p[1];
	normal.z = p[2];
	d = p[3];
}

inline Plane::Plane(const Vector3 &origin, const Vector3 &normal)
{
	this->normal.x = normal.x;
	this->normal.y = normal.y;
	this->normal.z = normal.z;
	d = -Vector3::Dot(origin, normal);
}

inline Plane::Plane(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	Vector3 e3 = b - a;
	Vector3 e1 = c - b;
	Vector3 crossed = Vector3::Cross(e3, e1);
	float scaleFactor = 1.0f / Vector3::Length(crossed);

	normal = crossed * scaleFactor;
		
	d = -Vector3::Dot(a, normal);
}

inline PLANE_POINT_CLASSIFY Plane::ClassifyPoint(const Plane &plane, const Vector3 &point)
{
	float planeDot = Vector3::Dot(plane.normal, point) + plane.d;
		
	if (planeDot < 0.0f)
		return BEHIND;
	else if (planeDot > 0.0f)
		return IN_FRONT;
	else
		return ON_PLANE;
}

inline float Plane::DistanceBetween(const Plane &plane, const Vector3 &point)
{
	return Vector3::Dot(point, plane.normal) + plane.d;
}

inline BOOL Plane::IsFrontFacingTo(const Plane &plane, const Vector3 &direction)
{
	if (Vector3::Dot(plane.normal, direction) <= 0.0f)
		return true;
	else
		return false;
}

inline Plane Plane::Normalize(const Plane &plane)
{
	float length = sqrtf(
		(plane.normal.x * plane.normal.x) + 
		(plane.normal.y * plane.normal.y) + 
		(plane.normal.z * plane.normal.z)
		);
	return Plane(
		plane.normal.x / length,
		plane.normal.y / length,
		plane.normal.z / length,
		plane.d / length
		);
}

#endif

