#ifdef __S3E__
#ifndef __FRAMEWORK_INPUT_MARMALADEMOUSE_H_INCLUDED__
#define __FRAMEWORK_INPUT_MARMALADEMOUSE_H_INCLUDED__

#include "../common.h"
#include "mouse.h"
#include "s3ePointer.h"
#include <stl/set.h>

class MouseListener;

class MarmaladeMouse : public Mouse
{
public:
	MarmaladeMouse();
	virtual ~MarmaladeMouse();

	void ResetDeltas();

	bool OnButtonEvent(const s3ePointerEvent *eventArgs);
	bool OnMotionEvent(const s3ePointerMotionEvent *eventArgs);

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
	bool *m_buttons;
	bool *m_lockedButtons;

	uint m_x;
	uint m_y;
	int m_deltaX;
	int m_deltaY;
};

#endif
#endif
