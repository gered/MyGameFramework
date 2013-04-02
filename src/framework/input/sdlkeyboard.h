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
	SDLKeyboard(SDLSystem *system, bool hasPhysicalKeysForGameControls);
	virtual ~SDLKeyboard();

	bool OnKeyEvent(const SDL_KeyboardEvent *eventArgs);

	bool HasPhysicalKeysForGameControls() const            { return m_hasPhysicalKeysForGameControls; }

	bool IsDown(KEYS key)                                  { return m_keys[key] && !m_lockedKeys[key]; }
	bool IsPressed(KEYS key);
	void Lock(KEYS key)                                    { m_lockedKeys[key] = true; }

	void Reset();

	void RegisterListener(KeyboardListener *listener);
	void UnregisterListener(KeyboardListener *listener);

private:
	stl::set<KeyboardListener*> m_listeners;
	SDLSystem *m_system;

	bool *m_keys;
	bool *m_lockedKeys;
	bool m_hasPhysicalKeysForGameControls;
};

#endif
#endif

