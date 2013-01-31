#ifndef __FRAMEWORK_MATH_CIRCLE_H_INCLUDED__
#define __FRAMEWORK_MATH_CIRCLE_H_INCLUDED__

#include "../common.h"

/**
 * Represents a 2D circle.
 */
struct Circle
{
	/**
	 * Creates an uninitialized circle.
	 */
	Circle();

	/**
	 * Creates a circle with the specified properties.
	 * @param x X coordinate of the circle's center point
	 * @param y Y coordinate of the circle's center point
	 * @param radius the radius of the circle
	 */
	Circle(int32_t x, int32_t y, uint32_t radius);

	/**
	 * Set new properties for this circle.
	 * @param x X coordinate of the circle's center point
	 * @param y Y coordinate of the circle's center point
	 * @param radius the radius of the circle
	 */
	void Set(int32_t x, int32_t y, uint32_t radius);

	/**
	 * @return the circle's diameter
	 */
	int32_t GetDiameter() const;

	int32_t x;
	int32_t y;
	uint32_t radius;
};

inline Circle::Circle()
{
}

inline Circle::Circle(int32_t x, int32_t y, uint32_t radius)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
}

inline void Circle::Set(int32_t x, int32_t y, uint32_t radius)
{
	this->x = x;
	this->y = y;
	this->radius = radius;
}

inline int32_t Circle::GetDiameter() const
{
	return radius * 2;
}

#endif

