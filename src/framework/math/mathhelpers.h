#ifndef __FRAMEWORK_MATH_MATHHELPERS_H_INCLUDED__
#define __FRAMEWORK_MATH_MATHHELPERS_H_INCLUDED__

#include "../common.h"
#include "common.h"

#include <math.h>
#include <float.h>
#include <stdlib.h>

struct Vector3;

#ifdef _MSC_VER
#include <limits>
#ifndef INFINITY
#define INFINITY (std::numeric_limits<float>::infinity())
#endif
#endif

#ifdef _MSC_VER
#define isnan(x) _isnan(x)
#endif

/**
 * Converts coordinates from spherical to cartesian.
 * @param radius distance from the origin "O" to the point "P" (the "r" value)
 * @param inclination the angle (in radians) between the zenith direction 
 *                    and the line segment OP (the "theta" value)
 * @param azimuth the signed angle (in radians) measured from the azimuth 
 *                reference direction to the orthogonal projection of the 
 *                line segment OP on the reference plane (the "phi" value)
 * @param x variable to hold the output X cartesian coordinate
 * @param y variable to hold the output Y cartesian coordinate
 * @param z variable to hold the output Z cartesian coordinate
 */
void GetCartesianCoordsFromSpherical(float radius, float angleTheta, float angleRho, float &x, float &y, float &z);

/**
 * Converts an angle around the Y axis to a direction vector that
 * lies flat on the XZ plane. Note that the angle is offset so that 0 degrees
 * points directly down the +Y axis on a 2D cartesian grid, instead of the +X 
 * axis as one might expect.
 * @param angle the Y axis angle (in radians)
 * @return Vector3 direction vector on the XZ plane
 */
Vector3 GetDirectionFromYAxisOrientation(float angle);

/**
 * Converts euler angles to an equivalent direction vector. Note that this just
 * uses one of many other ways to do this (the others using any other rotation
 * order).
 * @param yaw the yaw rotation angle (in radians)
 * @param pitch the pitch rotation angle (in radians)
 * @return Vector3 direction vector
 */
Vector3 GetDirectionFromAngles(float yaw, float pitch);

/**
 * Returns the angle between two 2D points. Note that the returned angle is
 * offset so that 0 degrees points directly down the +Y axis on a 2D cartesian
 * grid, instead of the +X axis as one might expect.
 * @param x1 X coordinate of the first point
 * @param y1 Y coordinate of the first point
 * @param x2 X coordinate of the second point
 * @param y2 Y coordinate of the second point
 * @return float the angle (in radians) between the two points
 */
float GetAngleBetweenPoints(float x1, float y1, float x2, float y2);

/**
 * Solves a quadratic equation and returns the lowest root.
 * @param a the value of the a variable in the quadratic equation
 * @param b the value of the b variable in the quadratic equation
 * @param c the value of the c variable in the quadratic equation
 * @param maxR the maximum root value we will accept as an answer.
               anything that is higher will not be accepted as a solution
 * @param root the variable to hold the lowest root value if one is found
 * @return BOOL TRUE if the quadratic could be solved, FALSE if not
 */
BOOL GetLowestQuadraticRoot(float a, float b, float c, float maxR, float &root);

/**
 * Gets the power of two value that comes after the given value.
 * @param n minimum value for which we want to get the power of two after
 * @return uint32_t the first power of two that comes after n
 */
uint32_t GetNextPowerOf2(uint32_t n);

/**
 * Returns the 2D point on a circle's circumference given a radius and angle.
 * @param radius the radius of the circle
 * @param angle the angle around the circle (in radians)
 * @param x the variable to hold the X point on the circle's circumference
 * @param y the variable to hold the Y point on the circle's circumference
 */
void GetPointOnCircle(float radius, float angle, float &x, float &y);

/**
 * Clamps a value to a given range.
 * @param value the value to be clamped
 * @param low the low end of the range to clamp to
 * @param high the high end of the range to clamp to
 * @return T the clamped value
 */
template <class T>
inline T Clamp(const T &value, const T &low, const T &high)
{
	if (value < low)
		return low;
	if (value > high)
		return high;

	return value;
}

/**
 * Converts degrees to radians.
 * @param degrees degree value to convert
 * @return float equivalent value in radians
 */
inline float DegreesToRadians(float degrees)
{
	return degrees * PI_OVER_180;
}

/**
 * Checks two floats for equality using a defined "tolerance" to account
 * for floating point rounding errors, etc.
 * @param a first value to check
 * @param b second value to check
 * @param tolerance tolerance value to use (e.g. FLT_EPSILON)
 * @return BOOL TRUE if equal, FALSE if not
 */
inline BOOL IsCloseEnough(float a, float b, float tolerance = TOLERANCE)
{
	return fabsf((a - b) / ((b == 0.0f) ? 1.0f : b)) < tolerance;
}

/**
 * Determines if a given number is a power of two.
 * @param n number to check
 * @return BOOL TRUE if a power of two, FALSE if not
 */
inline BOOL IsPowerOf2(int32_t n)
{
	return (n != 0) && !(n & (n - 1));
}

/**
 * Linearly interpolates between two values.
 * @param a first value (low end of range)
 * @param b second value (high end of range)
 * @param t the amount to interpolate between the two values
 * @return T the interpolated value
 */
template <class T>
inline T Lerp(const T &a, const T &b, const T &t)
{
	return a + (b - a) * t;
}

/**
 * Given a linearly interpolated value and the original range (high and 
 * low) of the linear interpolation, this will determine what the original
 * 0.0 to 1.0 value (weight) was used to perform the interpolation.
 * @param a first value (low end of range)
 * @param b second value (high end of range)
 * @param lerpValue the result of the original interpolation
 * @return T the interpolation value (weight, in the range 0.0 to 1.0) 
 *           used in the original interpolation
 */
template <class T>
inline T InverseLerp(const T &a, const T &b, const T &lerpValue)
{
	return (lerpValue - a) / (b - a);
}

/**
 * Converts radians to degrees.
 * @param radians radian value to convert
 * @return float equivalent value in degrees
 */
inline float RadiansToDegrees(float radians)
{
	return radians * (1.0f / PI_OVER_180);
}

/**
 * Clamps a value to a given range, but if the value is outside the range
 * instead of returning the low/high end of the range, this will continue
 * counting after moving to the opposite end of the range to arrive at a
 * final value.
 * @param value the value to be clamped
 * @param low the low end of the range to clamp to
 * @param high the high end of the range to clamp to
 * @return T the clamped value
 */
template <class T>
inline T RolloverClamp(const T &value, const T &low, const T &high)
{
	T temp = value;
	do
	{
		float range = abs(high - low);
		if (temp < low)
			temp = temp + range;
		if (value > high)
			temp = temp - range;
	}
	while (temp < low || temp > high); // loop through as many times as necessary to put the value within the low/high range
	
	return temp;
}

template <> float RolloverClamp<float>(const float &value, const float &low, const float &high);

/**
 * Rounds a value to the nearest whole number. Will round up if the
 * fractional part of the value is greater then or equal to 0.5.
 * @param value the value to round
 * @return float the rounded value
 */
inline float Round(float value)
{
	return ceilf(value + 0.5f);
}

/**
 * Re-scales a given value from an old min/max range to a new and
 * different min/max range such that the value is approximately
 * at the same distance between both min and max values.
 * @param value the value to be rescaled which is currently between
 *              originalMin and originalMax
 * @param originalMin original min value (low end of range)
 * @param originalMax original max value (high end of range)
 * @param newMin new min value (low end of range)
 * @param newMax new max value (high end of range)
 * @return T re-scaled value which will fall between newMin and newMax
 */
template <class T>
inline T ScaleRange(const T &value, const T &originalMin, const T &originalMax, const T &newMin, const T &newMax)
{
	return (value / ((originalMax - originalMin) / (newMax - newMin))) + newMin;
}

/**
 * Returns the sign of the given value.
 * @param value value to get the sign of
 * @return T the sign of the given value
 */
template <class T>
inline T Sign(const T &value)
{
	if (value < 0)
		return -1;
	else if (value > 0)
		return 1;
	else
		return 0;
}

template <> float Sign<float>(const float &value);

/**
 * Interpolates between two values using a cubic equation.
 * @param low low end of range to interpolate between
 * @param high high end of range to interpolate between
 * @param t amount to interpolate by (the weight)
 * @return float the interpolated value
 */
inline float SmoothStep(float low, float high, float t)
{
	float n = Clamp(t, 0.0f, 1.0f);
	return Lerp(low, high, (n * n) * (3.0f - (2.0f * n)));
}

/**
 * Rounds a value to the nearest whole number. If the value has a
 * fractional component that will be used to either round up or down.
 * If the value is negative, this will round down.
 * @param value the value to round
 * @return float the rounded value
 */
inline float SymmetricalRound(float value)
{
	if (value > 0.0f)
		return floorf(value + 0.5f);
	else
		return ceilf(value - 0.5f);
}

#endif

