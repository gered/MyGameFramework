#ifndef __FRAMEWORK_MATH_COLLISIONPACKET_H_INCLUDED__
#define __FRAMEWORK_MATH_COLLISIONPACKET_H_INCLUDED__

#include "../common.h"
#include "vector3.h"

struct CollisionPacket
{
	// defines the x/y/z radius of the entity being checked
	Vector3 ellipsoidRadius;

	bool foundCollision;
	float nearestDistance;

	// the below fields are all in "ellipsoid space"

	Vector3 esVelocity;                  // velocity of the entity
	Vector3 esNormalizedVelocity;
	Vector3 esPosition;                  // current position of the entity

	Vector3 esIntersectionPoint;         // if an intersection is found

	CollisionPacket()
	{
		ellipsoidRadius = ZERO_VECTOR;
		foundCollision = false;
		nearestDistance = 0.0f;
		esVelocity = ZERO_VECTOR;
		esNormalizedVelocity = ZERO_VECTOR;
		esPosition = ZERO_VECTOR;
		esIntersectionPoint = ZERO_VECTOR;
	}
};

#endif

