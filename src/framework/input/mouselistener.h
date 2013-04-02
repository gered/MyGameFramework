#ifndef __FRAMEWORK_INPUT_MOUSELISTENER_H_INCLUDED__
#define __FRAMEWORK_INPUT_MOUSELISTENER_H_INCLUDED__

#include "../common.h"
#include "mousebuttons.h"

/**
 * Interface for objects that want to receive callbacks for mouse
 * input events.
 */
class MouseListener
{
public:
	/**
	 * Callback for when a button is down (pressed).
	 * @param button the button that the event is for
	 * @param x current X coordinate of the mouse cursor
	 * @param y current Y coordinate of the mouse cursor
	 * @return true if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual bool OnMouseButtonDown(MOUSE_BUTTONS button, uint x, uint y) = 0;

	/**
	 * Callback for when a button is up (released).
	 * @param button the button that the event is for
	 * @param x current X coordinate of the mouse cursor
	 * @param y current Y coordinate of the mouse cursor
	 * @return true if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual bool OnMouseButtonUp(MOUSE_BUTTONS button, uint x, uint y) = 0;

	/**
	 * Callback for when the mouse cursor moves.
	 * @param x new X coordinate of the mouse cursor
	 * @param y new Y coordinate of the mouse cursor
	 * @param deltaX amount the mouse cursor moved since the last move event along the X axis
	 * @param deltaY amount the mouse cursor moved since the last move event along the Y axis
	 * @return true if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual bool OnMouseMove(uint x, uint y, int deltaX, int deltaY) = 0;
};

#endif

