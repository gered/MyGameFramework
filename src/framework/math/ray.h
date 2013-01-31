#ifndef __FRAMEWORK_MATH_RAY_H_INCLUDED__
#define __FRAMEWORK_MATH_RAY_H_INCLUDED__

#include "vector3.h"

/**
 * Represents a ray in 3D space originating from a point and pointing
 * in a (normalized) direction.
 */
struct Ray
{
	/**
	 * Creates a ray starting at the origin with zero length.
	 */
	Ray();

	/**
	 * Creates a ray with the specified properties.
	 * @param position the origin position of the ray
	 * @param direction the normalized direction vector of the ray
	 */
	Ray(const Vector3 &position, const Vector3 &direction);

	/**
	 * Creates a ray with the specified properties.
	 * @param positionX the X coordinate of the origin position of the ray
	 * @param positionY the Y coordinate of the origin position of the ray
	 * @param positionZ the Z coordinate of the origin position of the ray
	 * @param directionX the X coordinate of the normalized direction vector of the ray
	 * @param directionY the Y coordinate of the normalized direction vector of the ray
	 * @param directionZ the Z coordinate of the normalized direction vector of the ray
	 */
	Ray(float positionX, float positionY, float positionZ, float directionX, float directionY, float directionZ);

	/**
	 * Returns the point along the ray at given distance from the ray's origin.
	 * @param distance distance from the ray's origin
	 * @return Vector3 the point that is located the specified distance from the ray's origin
	 */
	Vector3 GetPositionAt(float distance) const;

	Vector3 position;
	Vector3 direction;
};

inline Ray::Ray()
{
	position = ZERO_VECTOR;
	direction = ZERO_VECTOR;
}

inline Ray::Ray(const Vector3 &position, const Vector3 &direction)
{
	this->position = position;
	this->direction = direction;
}

inline Ray::Ray(float positionX, float positionY, float positionZ, float directionX, float directionY, float directionZ)
{
	this->position = Vector3(positionX, positionY, positionZ);
	this->direction = Vector3(directionX, directionY, directionZ);
}

inline Vector3 Ray::GetPositionAt(float distance) const
{
	return (this->direction * distance) + this->position;
}

#endif

