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
	MarmaladeKeyboard(bool hasPhysicalKeysForGameControls);
	virtual ~MarmaladeKeyboard();

	bool OnKeyEvent(const s3eKeyboardEvent *eventArgs);
	bool OnKeyCharEvent(const s3eKeyboardCharEvent *eventArgs);

	bool HasPhysicalKeysForGameControls() const            { return m_hasPhysicalKeysForGameControls; }

	bool IsDown(KEYS key)                                  { return m_keys[key] && !m_lockedKeys[key]; }
	bool IsPressed(KEYS key);
	void Lock(KEYS key)                                    { m_lockedKeys[key] = true; }

	void Reset();

	void RegisterListener(KeyboardListener *listener);
	void UnregisterListener(KeyboardListener *listener);

private:
	stl::set<KeyboardListener*> m_listeners;
	bool *m_keys;
	bool *m_lockedKeys;
	bool m_hasPhysicalKeysForGameControls;
};

#endif
#endif
