#ifndef __FRAMEWORK_INPUT_TOUCHSCREENLISTENER_H_INCLUDED__
#define __FRAMEWORK_INPUT_TOUCHSCREENLISTENER_H_INCLUDED__

#include "../common.h"

/**
 * Interface for objects that want to receive callbacks for touch screen
 * input events.
 */
class TouchscreenListener
{
public:
	/**
	 * Callback for when a touch point is down (pressed).
	 * @param id the unique ID of the touch point
	 * @param x the X coordinate of the touch point
	 * @param y the Y coordinate of the touch point
	 * @param isPrimary TRUE if the touch screen input device class 
	 *                  considers this touch point to be the primary one
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnTouchDown(int id, uint x, uint y, BOOL isPrimary) = 0;

	/**
	 * Callback for when a touch point is up (released).
	 * @param id the unique ID of the touch point
	 * @param isPrimary TRUE if the touch screen input device class 
	 *                  considers this touch point to be the primary one
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnTouchUp(int id, BOOL isPrimary) = 0;

	/**
	 * Callback for when the touch point moves.
	 * @param id the unique ID of the touch point
	 * @param x the new X coordinate of the touch point
	 * @param y the new Y coordinate of the touch point
	 * @param deltaX amount the touch point moved since the last move event along the X axis
	 * @param deltaY amount the touch point moved since the last move event along the Y axis
	 * @param isPrimary TRUE if the touch screen input device class 
	 *                  considers this touch point to be the primary one
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnTouchMove(int id, uint x, uint y, int deltaX, int deltaY, BOOL isPrimary) = 0;
};

#endif

