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
	Rect(int left, int top, int right, int bottom);

	/**
	 * Sets new rect dimensions.
	 * @param left new left X coordinate
	 * @param top new top Y coordinate
	 * @param right new right X coordinate
	 * @param bottom new bottom Y coordinate
	 */
	void Set(int left, int top, int right, int bottom);

	/**
	 * Tests if a point is contained inside this rectangle.
	 * @param x x coordinate of the point to test
	 * @param y y coordinate of the point to test
	 * @return TRUE if the point is contained, FALSE if not
	 */
	BOOL Contains(int x, int y) const;

	/**
	 * @return the rect's width
	 */
	int GetWidth() const;

	/**
	 * @return the rect's height
	 */
	int GetHeight() const;

	int left;
	int top;
	int right;
	int bottom;
};

inline Rect::Rect()
{
}

inline Rect::Rect(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline void Rect::Set(int left, int top, int right, int bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline BOOL Rect::Contains(int x, int y) const
{
	if (x >= left && y >= top && x <= right && y <= bottom)
		return TRUE;
	else
		return FALSE;
}

inline int Rect::GetWidth() const
{
	return right - left;
}

inline int Rect::GetHeight() const
{
	return bottom - top;
}

#endif

