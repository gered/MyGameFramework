#ifndef __FRAMEWORK_MATH_RECT_H_INCLUDED__
#define __FRAMEWORK_MATH_RECT_H_INCLUDED__

#include "../common.h"

/**
 * Represents a 2D rectangle using integers for the coordinates.
 */
struct Rect
{
	/**
	 * Creates an uninitialized rect.
	 */
	Rect();

	/**
	 * Creates a rect with the specified properties.
	 * @param left left X coordinate
	 * @param top top Y coordinate
	 * @param right right X coordinate
	 * @param bottom bottom Y coordinate
	 */
	Rect(int32_t left, int32_t top, int32_t right, int32_t bottom);

	/**
	 * Sets new rect dimensions.
	 * @param left new left X coordinate
	 * @param top new top Y coordinate
	 * @param right new right X coordinate
	 * @param bottom new bottom Y coordinate
	 */
	void Set(int32_t left, int32_t top, int32_t right, int32_t bottom);

	/**
	 * Tests if a point is contained inside this rectangle.
	 * @param x x coordinate of the point to test
	 * @param y y coordinate of the point to test
	 * @return TRUE if the point is contained, FALSE if not
	 */
	BOOL Contains(int32_t x, int32_t y) const;

	/**
	 * @return the rect's width
	 */
	int32_t GetWidth() const;

	/**
	 * @return the rect's height
	 */
	int32_t GetHeight() const;

	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

inline Rect::Rect()
{
}

inline Rect::Rect(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline void Rect::Set(int32_t left, int32_t top, int32_t right, int32_t bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline BOOL Rect::Contains(int32_t x, int32_t y) const
{
	if (x >= left && y >= top && x <= right && y <= bottom)
		return TRUE;
	else
		return FALSE;
}

inline int32_t Rect::GetWidth() const
{
	return right - left;
}

inline int32_t Rect::GetHeight() const
{
	return bottom - top;
}

#endif

