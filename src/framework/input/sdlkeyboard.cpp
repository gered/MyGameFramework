#ifdef SDL
#include "../debug.h"

#include "sdlkeyboard.h"
#include "keyboardlistener.h"
#include "../sdlincludes.h"
#include "../sdlsystem.h"
#include <string.h>

SDLKeyboard::SDLKeyboard(SDLSystem *system, BOOL hasPhysicalKeysForGameControls)
{
	STACK_TRACE;
	m_system = system;
	m_hasPhysicalKeysForGameControls = hasPhysicalKeysForGameControls;

	m_keys = new BOOL[KSYM_LAST];
	ASSERT(m_keys != NULL);
	m_lockedKeys = new BOOL[KSYM_LAST];
	ASSERT(m_lockedKeys != NULL);

	Reset();
}

SDLKeyboard::~SDLKeyboard()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_keys);
	SAFE_DELETE_ARRAY(m_lockedKeys);
}

BOOL SDLKeyboard::OnKeyEvent(const SDL_KeyboardEvent *eventArgs)
{
	STACK_TRACE;
	int32_t keyCode = (int32_t)eventArgs->keysym.sym;

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

		m_keys[keyCode] = FALSE;
		m_lockedKeys[keyCode] = FALSE;
	}

	return TRUE;
}

BOOL SDLKeyboard::IsPressed(KEYS key)
{
	STACK_TRACE;
	if (m_keys[key] && !m_lockedKeys[key])
	{
		m_lockedKeys[key] = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

void SDLKeyboard::Reset()
{
	STACK_TRACE;
	memset(m_keys, FALSE, sizeof(BOOL) * KSYM_LAST);
	memset(m_lockedKeys, FALSE, sizeof(BOOL) * KSYM_LAST);
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

