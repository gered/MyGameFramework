#ifdef SDL
#ifndef __FRAMEWORK_INPUT_SDLMOUSE_H_INCLUDED__
#define __FRAMEWORK_INPUT_SDLMOUSE_H_INCLUDED__

#include "../common.h"
#include "mouse.h"
#include "../sdlincludes.h"
#include <stl/set.h>

class MouseListener;
class SDLSystem;

class SDLMouse : public Mouse
{
public:
	SDLMouse(SDLSystem *system);
	virtual ~SDLMouse();

	void ResetDeltas();

	bool OnButtonEvent(const SDL_MouseButtonEvent *eventArgs);
	bool OnMotionEvent(const SDL_MouseMotionEvent *eventArgs);

	bool IsDown(MOUSE_BUTTONS button)                      { return m_buttons[button] && !m_lockedButtons[button]; }
	bool IsPressed(MOUSE_BUTTONS button);
	void Lock(MOUSE_BUTTONS button)                        { m_lockedButtons[button] = true; }

	uint GetX() const                                      { return m_x; }
	uint GetY() const                                      { return m_y; }
	int GetDeltaX() const                                  { return m_deltaX; }
	int GetDeltaY() const                                  { return m_deltaY; }

	void Reset();

	void RegisterListener(MouseListener *listener);
	void UnregisterListener(MouseListener *listener);

private:
	stl::set<MouseListener*> m_listeners;
	SDLSystem *m_system;
	
	bool *m_buttons;
	bool *m_lockedButtons;

	uint m_x;
	uint m_y;
	int m_deltaX;
	int m_deltaY;
};

#endif
#endif

