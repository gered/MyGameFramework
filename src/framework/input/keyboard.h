#ifndef __FRAMEWORK_INPUT_KEYBOARD_H_INCLUDED__
#define __FRAMEWORK_INPUT_KEYBOARD_H_INCLUDED__

#include "../common.h"
#include "keysyms.h"

class KeyboardListener;

/**
 * Interface for a keyboard input device class.
 */
class Keyboard
{
public:
	Keyboard()                                             {}
	virtual ~Keyboard()                                    {}

	/**
	 * @return true if the underlying keyboard device was detected to have
	 *         enough physical keys for full game control
	 */
	virtual bool HasPhysicalKeysForGameControls() const = 0;

	/**
	 * Checks if the key is currently down, but not locked.
	 * @param key the key to check
	 * @return true if down and not locked, false if not down or locked
	 */
	virtual bool IsDown(KEYS key) = 0;

	/**
	 * Checks if the key is currently down, but not locked. If so, the key is
	 * locked until released so that duplicate "down" events will not be
	 * registered for this key until it is released and pressed down again.
	 * @param key the key to check
	 * @return true if down and not locked, false if not down or locked
	 */
	virtual bool IsPressed(KEYS key) = 0;

	/**
	 * Locks the key so that it will not be reported as down until it is
	 * released and then pressed down again.
	 * @param key the key to lock
	 */
	virtual void Lock(KEYS key) = 0;

	/**
	 * Resets the current key states and locked key states.
	 */
	virtual void Reset() = 0;

	/**
	 * Registers a keyboard event listener so that keyboard events will be
	 * sent to it.
	 * @param listener the listener object to register
	 */
	virtual void RegisterListener(KeyboardListener *listener) = 0;

	/**
	 * Unregisters a keyboard event listener so that keyboard events will no
	 * longer be sent to it.
	 * @param listener the listener object to unregister
	 */
	virtual void UnregisterListener(KeyboardListener *listener) = 0;
};

#endif
