#ifdef __S3E__
#ifndef __FRAMEWORK_INPUT_MARMALADEKEYBOARD_H_INCLUDED__
#define __FRAMEWORK_INPUT_MARMALADEKEYBOARD_H_INCLUDED__

#include "../common.h"
#include "keyboard.h"
#include "s3eKeyboard.h"
#include <stl/set.h>

class KeyboardListener;

class MarmaladeKeyboard : public Keyboard
{
public:
	MarmaladeKeyboard(BOOL hasPhysicalKeysForGameControls);
	virtual ~MarmaladeKeyboard();

	BOOL OnKeyEvent(const s3eKeyboardEvent *eventArgs);
	BOOL OnKeyCharEvent(const s3eKeyboardCharEvent *eventArgs);

	BOOL HasPhysicalKeysForGameControls() const            { return m_hasPhysicalKeysForGameControls; }

	BOOL IsDown(KEYS key)                                  { return m_keys[key] && !m_lockedKeys[key]; }
	BOOL IsPressed(KEYS key);
	void Lock(KEYS key)                                    { m_lockedKeys[key] = TRUE; }

	void Reset();

	void RegisterListener(KeyboardListener *listener);
	void UnregisterListener(KeyboardListener *listener);

private:
	stl::set<KeyboardListener*> m_listeners;
	BOOL *m_keys;
	BOOL *m_lockedKeys;
	BOOL m_hasPhysicalKeysForGameControls;
};

#endif
#endif
