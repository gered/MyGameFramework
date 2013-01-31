#ifndef __FRAMEWORK_MATH_BOUNDINGSPHERE_H_INCLUDED__
#define __FRAMEWORK_MATH_BOUNDINGSPHERE_H_INCLUDED__

#include "vector3.h"

/**
 * Represents a sphere in 3D space.
 */
struct BoundingSphere
{
	/**
	 * Creates a sphere centered on the origin with a zero-length radius.
	 */
	BoundingSphere();

	/**
	 * Creates a sphere with the specified properties.
	 * @param center the center point of the sphere
	 * @@param radius the sphere's radius
	 */
	BoundingSphere(const Vector3 &center, float radius);

	/**
	 * Creates a sphere with the specified properties.
	 * @param centerX X coordinate of the sphere's center point
	 * @param centerY Y coordinate of the sphere's center point
	 * @param centerZ Z coordinate of the sphere's center point
	 * @param radius the sphere's radius
	 */
	BoundingSphere(float centerX, float centerY, float centerZ, float radius);

	/**
	 * Creates a sphere that will be automatically sized and positioned 
	 * to fully enclose a set of vertices.
	 * @param vertices an array of vertices to be fully enclosed within the sphere
	 * @param numVertices the number of vertices being passed
	 */
	BoundingSphere(const Vector3 *vertices, int numVertices);

	Vector3 center;
	float radius;
};

inline BoundingSphere::BoundingSphere()
{
	center = ZERO_VECTOR;
	radius = 0.0f;
}

inline BoundingSphere::BoundingSphere(const Vector3 &center, float radius)
{
	this->center = center;
	this->radius = radius;
}

inline BoundingSphere::BoundingSphere(float centerX, float centerY, float centerZ, float radius)
{
	this->center = Vector3(centerX, centerY, centerZ);
	this->radius = radius;
}

#endif

