#ifdef __S3E__
#include "../debug.h"

#include "marmaladekeyboard.h"
#include "keyboardlistener.h"
#include <string.h>

MarmaladeKeyboard::MarmaladeKeyboard(BOOL hasPhysicalKeysForGameControls)
	: Keyboard()
{
	STACK_TRACE;
	m_hasPhysicalKeysForGameControls = hasPhysicalKeysForGameControls;

	m_keys = new BOOL[KSYM_LAST];
	ASSERT(m_keys != NULL);
	m_lockedKeys = new BOOL[KSYM_LAST];
	ASSERT(m_lockedKeys != NULL);

	Reset();
}

MarmaladeKeyboard::~MarmaladeKeyboard()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_keys);
	SAFE_DELETE_ARRAY(m_lockedKeys);
}

BOOL MarmaladeKeyboard::OnKeyEvent(const s3eKeyboardEvent *eventArgs)
{
	STACK_TRACE;
	int32_t keyCode = (int32_t)eventArgs->m_Key;
	BOOL isDown = (BOOL)eventArgs->m_Pressed;

	if (isDown)
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

BOOL MarmaladeKeyboard::OnKeyCharEvent(const s3eKeyboardCharEvent *eventArgs)
{
	STACK_TRACE;
	// TODO: implementation
	return FALSE;
}

BOOL MarmaladeKeyboard::IsPressed(KEYS key)
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

void MarmaladeKeyboard::Reset()
{
	STACK_TRACE;
	memset(m_keys, FALSE, sizeof(BOOL) * KSYM_LAST);
	memset(m_lockedKeys, FALSE, sizeof(BOOL) * KSYM_LAST);
}

void MarmaladeKeyboard::RegisterListener(KeyboardListener *listener)
{
	STACK_TRACE;
	m_listeners.insert(listener);
}

void MarmaladeKeyboard::UnregisterListener(KeyboardListener *listener)
{
	STACK_TRACE;
	m_listeners.erase(listener);
}

#endif
