#ifndef __FRAMEWORK_MATH_RECTF_H_INCLUDED__
#define __FRAMEWORK_MATH_RECTF_H_INCLUDED__

/**
 * Represents a 2D rectangle using floats for the coordinates.
 */
struct RectF
{
	/**
	 * Creates an uninitialized rect.
	 */
	RectF();

	/**
	 * Creates a rect with the specified properties.
	 * @param left left X coordinate
	 * @param top top Y coordinate
	 * @param right right X coordinate
	 * @param bottom bottom Y coordinate
	 */
	RectF(float left, float top, float right, float bottom);

	/**
	 * Sets new rect dimensions.
	 * @param left new left X coordinate
	 * @param top new top Y coordinate
	 * @param right new right X coordinate
	 * @param bottom new bottom Y coordinate
	 */
	void Set(float left, float top, float right, float bottom);

	/**
	 * Tests if a point is contained inside this rectangle.
	 * @param x x coordinate of the point to test
	 * @param y y coordinate of the point to test
	 * @return BOOL TRUE if the point is contained, FALSE if not
	 */
	BOOL Contains(float x, float y) const;

	/**
	 * @return the rect's width
	 */
	float GetWidth() const;

	/**
	 * @return the rect's height
	 */
	float GetHeight() const;

	float left;
	float top;
	float right;
	float bottom;
};

inline RectF::RectF()
{
}

inline RectF::RectF(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline void RectF::Set(float left, float top, float right, float bottom)
{
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
}

inline BOOL RectF::Contains(float x, float y) const
{
	if (x >= left && y >= top && x <= right && y <= bottom)
		return TRUE;
	else
		return FALSE;
}

inline float RectF::GetWidth() const
{
	return right - left;
}

inline float RectF::GetHeight() const
{
	return bottom - top;
}

#endif

