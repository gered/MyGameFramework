#ifdef __S3E__
#include "marmaladekeyboard.h"
#include "keyboardlistener.h"
#include <string.h>

MarmaladeKeyboard::MarmaladeKeyboard(bool hasPhysicalKeysForGameControls)
{
	m_hasPhysicalKeysForGameControls = hasPhysicalKeysForGameControls;

	m_keys = new bool[KSYM_LAST];
	m_lockedKeys = new bool[KSYM_LAST];

	Reset();
}

MarmaladeKeyboard::~MarmaladeKeyboard()
{
	SAFE_DELETE_ARRAY(m_keys);
	SAFE_DELETE_ARRAY(m_lockedKeys);
}

bool MarmaladeKeyboard::OnKeyEvent(const s3eKeyboardEvent *eventArgs)
{
	int keyCode = (int)eventArgs->m_Key;
	bool isDown = (bool)eventArgs->m_Pressed;

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

		m_keys[keyCode] = false;
		m_lockedKeys[keyCode] = false;
	}

	return true;
}

bool MarmaladeKeyboard::OnKeyCharEvent(const s3eKeyboardCharEvent *eventArgs)
{
	// TODO: implementation
	return false;
}

bool MarmaladeKeyboard::IsPressed(KEYS key)
{
	if (m_keys[key] && !m_lockedKeys[key])
	{
		m_lockedKeys[key] = true;
		return true;
	}
	else
		return false;
}

void MarmaladeKeyboard::Reset()
{
	memset(m_keys, false, sizeof(bool) * KSYM_LAST);
	memset(m_lockedKeys, false, sizeof(bool) * KSYM_LAST);
}

void MarmaladeKeyboard::RegisterListener(KeyboardListener *listener)
{
	m_listeners.insert(listener);
}

void MarmaladeKeyboard::UnregisterListener(KeyboardListener *listener)
{
	m_listeners.erase(listener);
}

#endif
