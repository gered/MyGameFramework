#ifdef SDL
#ifndef __FRAMEWORK_INPUT_SDLKEYBOARD_H_INCLUDED__
#define __FRAMEWORK_INPUT_SDLKEYBOARD_H_INCLUDED__

#include "../common.h"
#include "keyboard.h"
#include "../sdlincludes.h"
#include <stl/set.h>

class KeyboardListener;
class SDLSystem;

class SDLKeyboard : public Keyboard
{
public:
	SDLKeyboard(SDLSystem *system, BOOL hasPhysicalKeysForGameControls);
	virtual ~SDLKeyboard();

	BOOL OnKeyEvent(const SDL_KeyboardEvent *eventArgs);

	BOOL HasPhysicalKeysForGameControls() const            { return m_hasPhysicalKeysForGameControls; }

	BOOL IsDown(KEYS key)                                  { return m_keys[key] && !m_lockedKeys[key]; }
	BOOL IsPressed(KEYS key);
	void Lock(KEYS key)                                    { m_lockedKeys[key] = TRUE; }

	void Reset();

	void RegisterListener(KeyboardListener *listener);
	void UnregisterListener(KeyboardListener *listener);

private:
	stl::set<KeyboardListener*> m_listeners;
	SDLSystem *m_system;

	BOOL *m_keys;
	BOOL *m_lockedKeys;
	BOOL m_hasPhysicalKeysForGameControls;
};

#endif
#endif

