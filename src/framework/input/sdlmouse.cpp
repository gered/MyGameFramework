#ifdef SDL
#include "sdlmouse.h"
#include "mouselistener.h"
#include "../sdlincludes.h"
#include "../sdlsystem.h"
#include <string.h>

// TODO: query hardware for number of buttons?
const int NUM_BUTTONS = 5;

SDLMouse::SDLMouse(SDLSystem *system)
{
	m_system = system;

	m_buttons = new bool[NUM_BUTTONS];
	m_lockedButtons = new bool[NUM_BUTTONS];

	Reset();
}

SDLMouse::~SDLMouse()
{
	SAFE_DELETE_ARRAY(m_buttons);
	SAFE_DELETE_ARRAY(m_lockedButtons);
}

void SDLMouse::ResetDeltas()
{
	m_deltaX = 0;
	m_deltaY = 0;
}

bool SDLMouse::OnButtonEvent(const SDL_MouseButtonEvent *eventArgs)
{
	// translate from SDL's button values to our own MOUSE_BUTTONS enum
	int button = (int)eventArgs->button - 1;

	if (eventArgs->state == SDL_PRESSED)
	{
		// Pressed only if not locked
		m_buttons[button] = !(m_lockedButtons[button]);
		
		uint x = eventArgs->x;
		uint y = eventArgs->y;

		// always report button down events
		// NOTE: we're ignoring the "locked button" state because listeners
		//       don't have support for it (yet)
		for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnMouseButtonDown((MOUSE_BUTTONS)button, x, y))
				break;
		}
	}
	else
	{
		uint x = eventArgs->x;
		uint y = eventArgs->y;

		// if the button is just being released this tick, then trigger an event in all listeners
		if (m_buttons[button])
		{
			for (stl::set<MouseListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnMouseButtonUp((MOUSE_BUTTONS)button, x, y))
					break;
			}
		}

		m_buttons[button] = false;
		m_lockedButtons[button] = false;
	}

	return true;
}

bool SDLMouse::OnMotionEvent(const SDL_MouseMotionEvent *eventArgs)
{
	m_deltaX = eventArgs->x - m_x;
	m_deltaY = eventArgs->y - m_y;

	m_x = eventArgs->x;
	m_y = eventArgs->y;

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

bool SDLMouse::IsPressed(MOUSE_BUTTONS button)
{
	if (m_buttons[button] && !m_lockedButtons[button])
	{
		m_lockedButtons[button] = true;
		return true;
	}
	else
		return false;
}

void SDLMouse::Reset()
{
	memset(m_buttons, false, sizeof(bool) * NUM_BUTTONS);
	memset(m_lockedButtons, false, sizeof(bool) * NUM_BUTTONS);
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
}

void SDLMouse::RegisterListener(MouseListener *listener)
{
	m_listeners.insert(listener);
}

void SDLMouse::UnregisterListener(MouseListener *listener)
{
	m_listeners.erase(listener);
}

#endif
