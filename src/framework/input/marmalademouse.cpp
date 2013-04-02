#ifdef __S3E__
#include "../debug.h"

#include "marmalademouse.h"
#include "mouselistener.h"
#include <string.h>

const int NUM_BUTTONS = S3E_POINTER_BUTTON_MAX;

MarmaladeMouse::MarmaladeMouse()
{
	m_buttons = new bool[NUM_BUTTONS];
	m_lockedButtons = new bool[NUM_BUTTONS];

	Reset();
}

MarmaladeMouse::~MarmaladeMouse()
{
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
}

void MarmaladeMouse::ResetDeltas()
{
	m_deltaX = 0;
	m_deltaY = 0;
}

bool MarmaladeMouse::OnButtonEvent(const s3ePointerEvent *eventArgs)
{
	int button = (int)eventArgs->m_Button;
	bool isDown = (bool)eventArgs->m_Pressed;
	int x = eventArgs->m_x;
	int y = eventArgs->m_y;

	if (isDown)
	{
		// Pressed only if not locked
		m_buttons[button] = !(m_lockedButtons[button]);

		// always report button down events
		// NOTE: we're ignoring the "locked button" state because listeners
		//       don't have support for it (yet)
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseButtonDown((MOUSE_BUTTONS)button, (uint)x, (uint)y))
				break;
		}
	}
	else
	{
		// if the button is just being released this tick, then trigger an event in all listeners
		if (m_buttons[button])
		{
			for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnMouseButtonUp((MOUSE_BUTTONS)button, (uint)x, (uint)y))
					break;
			}
		}

		m_buttons[button] = false;
		m_lockedButtons[button] = false;
	}

	return true;
}

bool MarmaladeMouse::OnMotionEvent(const s3ePointerMotionEvent *eventArgs)
{
	m_deltaX = eventArgs->m_x - m_x;
	m_deltaY = eventArgs->m_y - m_y;

	m_x = eventArgs->m_x;
	m_y = eventArgs->m_y;

	// raise listener events for the mouse position only if it's moved this tick
	if (m_deltaX != 0 || m_deltaY != 0)
	{
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseMove(m_x, m_y, m_deltaX, m_deltaY))
				break;
		}
	}

	return true;
}

bool MarmaladeMouse::IsPressed(MOUSE_BUTTONS button)
{
	if (m_buttons[button] && !m_lockedButtons[button])
	{
		m_lockedButtons[button] = true;
		return true;
	}
	else
		return false;
}

void MarmaladeMouse::Reset()
{
	memset(m_buttons, false, sizeof(bool) * NUM_BUTTONS);
	memset(m_lockedButtons, false, sizeof(bool) * NUM_BUTTONS);
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
}

void MarmaladeMouse::RegisterListener(MouseListener *listener)
{
	m_listeners.insert(listener);
}

void MarmaladeMouse::UnregisterListener(MouseListener *listener)
{
	m_listeners.erase(listener);
}

#endif
