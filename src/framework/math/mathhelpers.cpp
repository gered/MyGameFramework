#include "mathhelpers.h"

#include "vector3.h"
#include <math.h>

void GetCartesianCoordsFromSpherical(float radius, float inclination, float azimuth, float &x, float &y, float &z)
{
	x = radius * sinf(inclination) * sinf(azimuth);
	y = radius * cosf(inclination);
	z = radius * sinf(inclination) * cosf(azimuth);
}

Vector3 GetDirectionFromYAxisOrientation(float angle)
{
	Vector3 facing;
	facing.y = 0.0f;

	// TODO: perhaps the whole "90 degree offset" thing we're doing below 
	//       is too scenario-specific. maybe have an overload of this function
	//       which accepts an offset angle parameter?
	// 
	// GetPointOnCircle() returns an angle based on a 2D cartesian grid where
	// 0 degrees points in the +X direction. We want it to point in the
	// +Y direction (2D cartesian Y = our 3D Z), so we offset our angle by 90
	// degrees before calling it to get the intended result
	float adjustedAngle = RolloverClamp(angle - RADIANS_90, RADIANS_0, RADIANS_360);
	GetPointOnCircle(1.0f, adjustedAngle, facing.x, facing.z);

	return facing;
}

Vector3 GetDirectionFromAngles(float yaw, float pitch)
{
	Vector3 result;
	result.x = cosf(yaw) * cosf(pitch);
	result.y = sinf(yaw) * cosf(pitch);
	result.z = sinf(pitch);

	return result;
}

float GetAngleBetweenPoints(float x1, float y1, float x2, float y2)
{
	float angle = atan2f(y2 - y1, x2 - x1);
	
	// we offset the angle by 90 degrees to ensure 0 degrees points in the
	// +Y direction on a 2D cartesian grid instead of +X. This corresponds with
	// the rest of our direction coordinate system.
	return angle - RADIANS_90;
}

BOOL GetLowestQuadraticRoot(float a, float b, float c, float maxR, float &root)
{
	float determinant = (b * b) - (4.0f * a * c);

	// If the determinant is negative, there is no solution (can't square root a negative)
	if (determinant < 0.0f)
		return FALSE;

	float sqrtDeterminant = sqrtf(determinant);
	float root1 = (-b - sqrtDeterminant) / (2 * a);
	float root2 = (-b + sqrtDeterminant) / (2 * a);
	// Sort so root1 <= root2
	if (root1 > root2)
	{
		float temp = root2;
		root2 = root1;
		root1 = temp;
	}

	// Get the lowest root
	if (root1 > 0 && root1 < maxR)
	{
		root = root1;
		return TRUE;
	}

	if (root2 > 0 && root2 < maxR)
	{
		root = root2;
		return TRUE;
	}

	// No valid solutions found
	return FALSE;
}

void GetPointOnCircle(float radius, float angle, float &x, float &y)
{
	x = radius * cosf(angle);
	y = radius * sinf(angle);
}

uint32_t GetNextPowerOf2(uint32_t n)
{
	uint32_t i = n & (~n + 1);
	while (i < n)
		i <<= 1;

	return i;
}

template <>
float Sign<float>(const float &value)
{
	if (value < 0.0f)
		return -1.0f;
	else if (value > 0.0f)
		return 1.0f;
	else
		return 0.0f;
}

template <>
float RolloverClamp<float>(const float &value, const float &low, const float &high)
{
	float temp = value;
	do
	{
		float range = fabsf(high - low);
		if (temp < low)
			temp = temp + range;
		if (value > high)
			temp = temp - range;
	}
	while (temp < low || temp > high); // loop through as many times as necessary to put the value within the low/high range
	
	return temp;
}

