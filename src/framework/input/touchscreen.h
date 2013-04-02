#ifndef __FRAMEWORK_INPUT_TOUCHSCREEN_H_INCLUDED__
#define __FRAMEWORK_INPUT_TOUCHSCREEN_H_INCLUDED__

#include "../common.h"

class TouchscreenListener;
struct Rect;
struct Circle;

const int INVALID_TOUCH_POINTER = -1;

/**
 * Interface for a touch point class associated with a touch screen input device.
 */
class TouchPointer
{
public:
	TouchPointer()                                         {}
	virtual ~TouchPointer()                                {}

	/**
	 * @return a unique ID that can be used to refer to this touch point
	 *         regardless of what order it is in in the list of touch
	 *         points maintained by the touch screen device
	 */
	virtual int GetId() const = 0;

	/**
	 * @return current X coordinate of the touch point
	 */
	virtual uint GetX() const = 0;

	/**
	 * @return current Y coordinate of the touch point
	 */
	virtual uint GetY() const = 0;

	/**
	 * @return amount the touch point moved since the last update along the X axis
	 */
	virtual int GetDeltaX() const = 0;

	/**
	 * @return amount the touch point moved since the last update along the Y axis
	 */
	virtual int GetDeltaY() const = 0;

	/**
	 * @return true if this touch point is currently down
	 */
	virtual bool IsTouching() const = 0;

	/**
	 * Checks if this touch point is currently down within a given area.
	 * @param left left X coordinate of the area to check
	 * @param top top Y coordinate of the area to check
	 * @param right right X coordinate of the area to check
	 * @param bottom bottom Y coordinate of the area to check
	 * @return true if the touch point is currently down within the given area
	 */
	virtual bool IsTouchingWithinArea(uint left, uint top, uint right, uint bottom) const = 0;

	/**
	 * Checks if this touch point is currently down within a given area.
	 * @param area the area to check
	 * @return true if the touch point is currently down within the given area
	 */
	virtual bool IsTouchingWithinArea(const Rect &area) const = 0;

	/**
	 * Checks if this touch point is currently down within a given area.
	 * @param centerX X coordinate of the center of a circular area to check
	 * @param centerY Y coordinate of the center of a circular area to check
	 * @param radius the radius of the circular area to check
	 * @return true if the touch point is currently down within the given area
	 */
	virtual bool IsTouchingWithinArea(uint centerX, uint centerY, uint radius) const = 0;

	/**
	 * Checks if this touch point is currently down within a given area.
	 * @param area the area to check
	 * @return true if the touch point is currently down within the given area
	 */
	virtual bool IsTouchingWithinArea(const Circle &area) const = 0;
};

/**
 * Interface for a touch screen input device class.
 */
class Touchscreen
{
public:
	Touchscreen()                                          {}
	virtual ~Touchscreen()                                 {}

	/**
	 * @return true if multitouch support was detected in the touch screen input device
	 */
	virtual bool IsMultitouchAvailable() const = 0;

	/**
	 * @return the maximum number of touch points supported (not necessarily the number currently down)
	 */
	virtual uint GetPointerCount() const = 0;

	/**
	 * @return true if at least one touch point is currently down
	 */
	virtual bool IsTouching() const = 0;

	/**
	 * Determines if the touch screen was tapped during the last tick, but not
	 * the tick before that (in other words, was just initially tapped).
	 * @return true if the touch screen was just tapped in the last tick
	 */
	virtual bool WasTapped() = 0;

	/**
	 * @return the primary touch pointer (the first one down or the only one
	 *         down). This will never be NULL, but it may refer to a touch
	 *         point whose IsTouching returns false.
	 */
	virtual const TouchPointer* GetPrimaryPointer() const = 0;

	/**
	 * Gets a touch pointer by it's array index. The exact pointer returned
	 * may vary between calls as other touch points go down or are released.
	 * @param index the index of the touch pointer to get. Should be in the
	 *              range 0 .. (GetPointerCount() - 1)
	 * @return the corresponding touch pointer
	 */
	virtual const TouchPointer* GetPointer(uint index) const = 0;

	/**
	 * Gets a touch pointer by it's unique ID. This will always return the
	 * same pointer between calls as long as it's being held down.
	 * @param id the unique ID of the pointer to get
	 * @return the corresponding touch pointer, or NULL if the ID doesn't match any existing touch pointer
	 */
	virtual const TouchPointer* GetPointerById(int id) const = 0;

	/**
	 * Resets the current touch screen and all touch point states.
	 */
	virtual void Reset() = 0;

	/**
	 * Registers a touch screen event listener so that touch events will be
	 * sent to it.
	 * @param listener the listener object to register
	 */
	virtual void RegisterListener(TouchscreenListener *listener) = 0;

	/**
	 * Unregisters a touch screen event listener so that touch events will no
	 * longer be sent to it.
	 * @param listener the listener object to unregister
	 */
	virtual void UnregisterListener(TouchscreenListener *listener) = 0;
};

#endif

