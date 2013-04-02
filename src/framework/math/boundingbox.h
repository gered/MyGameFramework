#ifndef __FRAMEWORK_MATH_BOUNDINGBOX_H_INCLUDED__
#define __FRAMEWORK_MATH_BOUNDINGBOX_H_INCLUDED__

#include "point3.h"
#include "vector3.h"
#include <math.h>

/**
 * Represents an axis-aligned bounding box (AABB) in 3D space.
 */
struct BoundingBox
{
	/**
	 * Creates a box centered on the origin with zero-length dimensions.
	 */
	BoundingBox();

	/**
	 * Creates a box with the specified properties.
	 * @param min the minimum point of the box (should be less then max)
	 * @param max the maximum point of the box (should be greater then min)
	 */
	BoundingBox(const Vector3 &min, const Vector3 &max);

	/**
	 * @param minX X coordinate of the minimum point of the box (should be less then maxX)
	 * @param minY Y coordinate of the minimum point of the box (should be less then maxY)
	 * @param minZ Z coordinate of the minimum point of the box (should be less then maxZ)
	 * @param maxX X coordinate of the maximum point of the box (should be greater then minX)
	 * @param maxY Y coordinate of the maximum point of the box (should be greater then minY)
	 * @param maxZ Z coordinate of the maximum point of the box (should be greater then minZ)
	 */
	BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

	/**
	 * Creates a box with the specified properties.
	 * @param min the minimum point of the box (should be less then max)
	 * @param max the maximum point of the box (should be greater then min)
	 */
	BoundingBox(const Point3 &min, const Point3 &max);

	/**
	 * Creates a box that will be automatically sized and positioned 
	 * to fully enclose a set of vertices.
	 * @param vertices an array of vertices to be fully enclosed within the box
	 * @param numVertices the number of vertices being passed
	 */
	BoundingBox(const Vector3 *vertices, uint numVertices);

	/**
	 * Creates a box with the specified properties.
	 * @param center the center point of the box
	 * @param halfWidth the distance from the center to the edge of the box
	 *                  on the X, Y and Z axis'
	 */
	BoundingBox(const Vector3 &center, float halfWidth);

	/**
	 * @return the absolute width of the box (X axis length)
	 */
	float GetWidth() const;

	/**
	 * @return the absolute height of the box (Y axis length)
	 */
	float GetHeight() const;

	/**
	 * @return the absolute depth of the box (Z axis length)
	 */
	float GetDepth() const;

	/**
	 * Returns the squared absolute distance from a point to the nearest
	 * edge of the given box.
	 * @param point the point
	 * @param box the box
	 * @return float the squared distance from point to box
	 */
	static float GetSquaredDistanceFromPointToBox(const Vector3 &point, const BoundingBox &box);

	/**
	 * Returns the absolute distance from a point to the nearest edge of 
	 * @param point the point
	 * @param box the box
	 * @return float the distance from point to box
	 */
	static float GetDistanceFromPointToBox(const Vector3 &point, const BoundingBox &box);

	Vector3 min;
	Vector3 max;
};

inline BoundingBox::BoundingBox()
{
	min = ZERO_VECTOR;
	max = ZERO_VECTOR;
}

inline BoundingBox::BoundingBox(const Vector3 &min, const Vector3 &max)
{
	this->min = min;
	this->max = max;
}

inline BoundingBox::BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	this->min = Vector3(minX, minY, minZ);
	this->max = Vector3(maxX, maxY, maxZ);
}

inline BoundingBox::BoundingBox(const Point3 &min, const Point3 &max)
{
	this->min = Vector3((float)min.x, (float)min.y, (float)min.z);
	this->max = Vector3((float)max.x, (float)max.y, (float)max.z);
}

inline BoundingBox::BoundingBox(const Vector3 &center, float halfWidth)
{
	this->min = Vector3(center.x - halfWidth, center.y - halfWidth, center.z - halfWidth);
	this->max = Vector3(center.x + halfWidth, center.y + halfWidth, center.z + halfWidth);
}

inline float BoundingBox::GetWidth() const
{
	return fabsf(max.x - min.x);
}

inline float BoundingBox::GetHeight() const
{
	return fabsf(max.y - min.y);
}

inline float BoundingBox::GetDepth() const
{
	return fabsf(max.z - min.z);
}

#endif

