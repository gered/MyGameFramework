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
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnMouseButtonDown(MOUSE_BUTTONS button, uint16_t x, uint16_t y) = 0;

	/**
	 * Callback for when a button is up (released).
	 * @param button the button that the event is for
	 * @param x current X coordinate of the mouse cursor
	 * @param y current Y coordinate of the mouse cursor
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnMouseButtonUp(MOUSE_BUTTONS button, uint16_t x, uint16_t y) = 0;

	/**
	 * Callback for when the mouse cursor moves.
	 * @param x new X coordinate of the mouse cursor
	 * @param y new Y coordinate of the mouse cursor
	 * @param deltaX amount the mouse cursor moved since the last move event along the X axis
	 * @param deltaY amount the mouse cursor moved since the last move event along the Y axis
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnMouseMove(uint16_t x, uint16_t y, int16_t deltaX, int16_t deltaY) = 0;
};

#endif

