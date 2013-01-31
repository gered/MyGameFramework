#ifndef __FRAMEWORK_MATH_WORLDPOSITION_H_INCLUDED__
#define __FRAMEWORK_MATH_WORLDPOSITION_H_INCLUDED__

#include "vector3.h"
#include "matrix4x4.h"

/**
 * Represents a "world position" in 3D space using a matrix with methods 
 * to modify this position.
 */
struct WorldPosition
{
	/**
	 * Creates a new world position object with an initial position
	 * at the origin.
	 */
	WorldPosition();

	/**
	 * Reset the position to the origin.
	 */
	void Reset();

	/**
	 * Rotate around the given axis by a given angle.
	 * @param angle the angle to rotate by in radians
	 * @param axisX the x component of the axis to rotate around
	 * @param axisY the y component of the axis to rotate around
	 * @param axisZ the z component of the axis to rotate around
	 */
	void Rotate(float angle, float axisX, float axisY, float axisZ);

	/**
	 * Rotate around the given axis by a given angle.
	 * @param angle the angle to rotate by in radians
	 * @param axis the axis to rotate around
	 */
	void Rotate(float angle, const Vector3 &axis);

	/**
	 * Scale by a given amount along each axis.
	 * @param x amount to scale by along the X axis
	 * @param y amount to scale by along the Y axis
	 * @param z amount to scale by along the Z axis
	 */
	void Scale(float x, float y, float z);

	/**
	 * Scale by a given amount along each axis.
	 * @param v amount to scale by along the X, Y and Z axis
	 */
	void Scale(const Vector3 &v);

	/**
	 * Translate by a given amount along each axis.
	 * @param x amount to translate by along the X axis
	 * @param y amount to translate by along the Y axis
	 * @param z amount to translate by along the Z axis
	 */
	void Translate(float x, float y, float z);

	/**
	* Translate by a given amount along each axis.
	 * @param v amount to translate by along the X, Y and Z axis
	 */
	void Translate(const Vector3 &v);

	Matrix4x4 current;
};

inline WorldPosition::WorldPosition()
{
	Reset();
}

inline void WorldPosition::Reset()
{
	current = IDENTITY_MATRIX;
}

inline void WorldPosition::Rotate(float angle, float axisX, float axisY, float axisZ)
{
	current *= Matrix4x4::CreateRotation(angle, Vector3(axisX, axisY, axisZ));
}

inline void WorldPosition::Rotate(float angle, const Vector3 &axis)
{
	current *= Matrix4x4::CreateRotation(angle, axis);
}

inline void WorldPosition::Scale(float x, float y, float z)
{
	current *= Matrix4x4::CreateScale(x, y, z);
}

inline void WorldPosition::Scale(const Vector3 &v)
{
	current *= Matrix4x4::CreateScale(v.x, v.y, v.z);
}

inline void WorldPosition::Translate(float x, float y, float z)
{
	current *= Matrix4x4::CreateTranslation(x, y, z);
}

inline void WorldPosition::Translate(const Vector3 &v)
{
	current *= Matrix4x4::CreateTranslation(v.x, v.y, v.z);
}

#endif
