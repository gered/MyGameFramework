#ifdef SDL
#include "sdlkeyboard.h"
#include "keyboardlistener.h"
#include "../sdlincludes.h"
#include "../sdlsystem.h"
#include <string.h>

SDLKeyboard::SDLKeyboard(SDLSystem *system, bool hasPhysicalKeysForGameControls)
{
	m_system = system;
	m_hasPhysicalKeysForGameControls = hasPhysicalKeysForGameControls;

	m_keys = new bool[KSYM_LAST];
	m_lockedKeys = new bool[KSYM_LAST];

	Reset();
}

SDLKeyboard::~SDLKeyboard()
{
	SAFE_DELETE_ARRAY(m_keys);
	SAFE_DELETE_ARRAY(m_lockedKeys);
}

bool SDLKeyboard::OnKeyEvent(const SDL_KeyboardEvent *eventArgs)
{
	int keyCode = (int)eventArgs->keysym.sym;

	if (eventArgs->state == SDL_PRESSED)
	{
		m_keys[keyCode] = !(m_lockedKeys[keyCode]);            // pressed only if not locked

		// always report keydown events
		// NOTE: we're ignoring the "locked key" state because listeners
		//       don't have support for it (yet)
		for (stl::set<KeyboardListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnKeyDown((KEYS)keyCode))
				break;
		}
	}
	else
	{
		// if the key is just being released this tick, then trigger an event in all listeners
		if (m_keys[keyCode])
		{
			for (stl::set<KeyboardListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnKeyUp((KEYS)keyCode))
					break;
			}
		}

		m_keys[keyCode] = false;
		m_lockedKeys[keyCode] = false;
	}

	return true;
}

bool SDLKeyboard::IsPressed(KEYS key)
{
	if (m_keys[key] && !m_lockedKeys[key])
	{
		m_lockedKeys[key] = true;
		return true;
	}
	else
		return false;
}

void SDLKeyboard::Reset()
{
	memset(m_keys, false, sizeof(bool) * KSYM_LAST);
	memset(m_lockedKeys, false, sizeof(bool) * KSYM_LAST);
}

void SDLKeyboard::RegisterListener(KeyboardListener *listener)
{
	m_listeners.insert(listener);
}

void SDLKeyboard::UnregisterListener(KeyboardListener *listener)
{
	m_listeners.erase(listener);
}

#endif

