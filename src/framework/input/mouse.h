#ifndef __FRAMEWORK_INPUT_MOUSE_H_INCLUDED__
#define __FRAMEWORK_INPUT_MOUSE_H_INCLUDED__

#include "../common.h"
#include "mousebuttons.h"

class MouseListener;

/**
 * Interface for a mouse input device class.
 */
class Mouse
{
public:
	Mouse()                                                {}
	virtual ~Mouse()                                       {}

	/**
	 * Checks if the button is currently down, but not locked.
	 * @param button the button to check
	 * @return TRUE if down and not locked, FALSE if not down or locked
	 */
	virtual BOOL IsDown(MOUSE_BUTTONS button) = 0;

	/**
	 * Checks if the button is currently down, but not locked. If so, the 
	 * button is locked until released so that duplicate "down" events will 
	 * not be registered for this button until it is released and pressed 
	 * down again.
	 * @param button the button to check
	 * @return TRUE if down and not locked, FALSE if not down or locked
	 */
	virtual BOOL IsPressed(MOUSE_BUTTONS button) = 0;

	/**
	 * Locks the button so that it will not be reported as down until it is
	 * released and then pressed down again.
	 * @param button the button to lock
	 */
	virtual void Lock(MOUSE_BUTTONS button) = 0;

	/**
	 * @return current X coordinate of the mouse cursor
	 */
	virtual uint GetX() const = 0;

	/**
	 * @return current Y coordinate of the mouse cursor
	 */
	virtual uint GetY() const = 0;

	/**
	 * @return amount the mouse cursor moved since the last update along the X axis
	 */
	virtual int GetDeltaX() const = 0;

	/**
	 * @return amount the mouse cursor moved since the last update along the Y axis
	 */
	virtual int GetDeltaY() const = 0;

	/**
	 * Resets the current mouse coordinates, button states and locked button states.
	 */
	virtual void Reset() = 0;

	/**
	 * Registers a mouse event listener so that mouse events will be
	 * sent to it.
	 * @param listener the listener object to register
	 */
	virtual void RegisterListener(MouseListener *listener) = 0;

	/**
	 * Unregisters a mouse event listener so that mouse events will no
	 * longer be sent to it.
	 * @param listener the listener object to unregister
	 */
	virtual void UnregisterListener(MouseListener *listener) = 0;
};

#endif
