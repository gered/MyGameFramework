#ifdef __S3E__
#include "../debug.h"

#include "marmalademouse.h"
#include "mouselistener.h"
#include <string.h>

const int32_t NUM_BUTTONS = S3E_POINTER_BUTTON_MAX;

MarmaladeMouse::MarmaladeMouse()
	: Mouse()
{
	STACK_TRACE;
	m_buttons = new BOOL[NUM_BUTTONS];
	ASSERT(m_buttons != NULL);
	m_lockedButtons = new BOOL[NUM_BUTTONS];
	ASSERT(m_lockedButtons != NULL);

	Reset();
}

MarmaladeMouse::~MarmaladeMouse()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
}

void MarmaladeMouse::ResetDeltas()
{
	STACK_TRACE;
	m_deltaX = 0;
	m_deltaY = 0;
}

BOOL MarmaladeMouse::OnButtonEvent(const s3ePointerEvent *eventArgs)
{
	STACK_TRACE;
	int32_t button = (int32_t)eventArgs->m_Button;
	BOOL isDown = (BOOL)eventArgs->m_Pressed;
	int32_t x = eventArgs->m_x;
	int32_t y = eventArgs->m_y;

	if (isDown)
	{
		// Pressed only if not locked
		m_buttons[button] = !(m_lockedButtons[button]);

		// always report button down events
		// NOTE: we're ignoring the "locked button" state because listeners
		//       don't have support for it (yet)
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseButtonDown((MOUSE_BUTTONS)button, (uint16_t)x, (uint16_t)y))
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
				if ((*i)->OnMouseButtonUp((MOUSE_BUTTONS)button, (uint16_t)x, (uint16_t)y))
					break;
			}
		}

		m_buttons[button] = FALSE;
		m_lockedButtons[button] = FALSE;
	}

	return TRUE;
}

BOOL MarmaladeMouse::OnMotionEvent(const s3ePointerMotionEvent *eventArgs)
{
	STACK_TRACE;
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

	return TRUE;
}

BOOL MarmaladeMouse::IsPressed(MOUSE_BUTTONS button)
{
	STACK_TRACE;
	if (m_buttons[button] && !m_lockedButtons[button])
	{
		m_lockedButtons[button] = TRUE;
		return TRUE;
	}
	else
		return FALSE;
}

void MarmaladeMouse::Reset()
{
	STACK_TRACE;
	memset(m_buttons, FALSE, sizeof(BOOL) * NUM_BUTTONS);
	memset(m_lockedButtons, FALSE, sizeof(BOOL) * NUM_BUTTONS);
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
}

void MarmaladeMouse::RegisterListener(MouseListener *listener)
{
	STACK_TRACE;
	m_listeners.insert(listener);
}

void MarmaladeMouse::UnregisterListener(MouseListener *listener)
{
	STACK_TRACE;
	m_listeners.erase(listener);
}

#endif
