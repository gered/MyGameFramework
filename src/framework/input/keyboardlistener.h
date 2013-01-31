#ifndef __FRAMEWORK_INPUT_KEYBOARDLISTENER_H_INCLUDED__
#define __FRAMEWORK_INPUT_KEYBOARDLISTENER_H_INCLUDED__

#include "keysyms.h"

/**
 * Interface for objects that want to receive callbacks for keyboard
 * input events.
 */
class KeyboardListener
{
public:
	/**
	 * Callback for when a key is down (pressed).
	 * @param key the key that the event is for
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnKeyDown(KEYS key) = 0;

	/**
	 * Callback for when a key is up (released).
	 * @param key the key that the event is for
	 * @return TRUE if no further listener callbacks of this kind should
	 *              be invoked until the next event occurs
	 */
	virtual BOOL OnKeyUp(KEYS key) = 0;
};

#endif

