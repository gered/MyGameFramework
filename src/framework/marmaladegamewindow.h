#ifdef __S3E__
#ifndef __FRAMEWORK_MARMALADEGAMEWINDOW_H_INCLUDED__
#define __FRAMEWORK_MARMALADEGAMEWINDOW_H_INCLUDED__

#include "common.h"

#include "gamewindow.h"
#include "screenorientation.h"
#include "graphics/glincludes.h"
#include "math/rect.h"

class MarmaladeSystem;
class BaseGameApp;

#define IGNORE_DIMENSIONS 0

class MarmaladeGameWindow : public GameWindow
{
public:
	MarmaladeGameWindow(BaseGameApp *gameApp, MarmaladeSystem *system);
	virtual ~MarmaladeGameWindow();

	bool Create(GameWindowParams *params);
	bool Resize(uint width = IGNORE_DIMENSIONS, uint height = IGNORE_DIMENSIONS);
	bool ToggleFullscreen();
	void Close();

	uint GetWidth() const                                  { return m_rect.GetWidth(); }
	uint GetHeight() const                                 { return m_rect.GetHeight(); }
	const Rect& GetRect() const                            { return m_rect; }
	uint GetBPP() const                                    { return m_bpp; }
	bool IsWindowed() const                                { return !m_fullscreen; }
	SCREEN_ORIENTATION_ANGLE GetScreenOrientation() const  { return m_screenOrientation; }

	bool IsActive() const                                  { return m_active; }
	bool IsFocused() const                                 { return m_focused; }
	bool IsClosing() const                                 { return m_closing; }
	bool HasGLContext() const                              { return eglGetCurrentContext() != EGL_NO_CONTEXT; }

	void ProcessEvent(const OSEvent *event);
	void Flip()                                            { eglSwapBuffers(m_eglDisplay, m_eglSurface); }

private:
	bool SetUpWindow();
	bool DestroyWindow();

	bool SetUpEGL();
	bool DestroyEGL();

	SCREEN_ORIENTATION_ANGLE GetCurrentScreenOrientationAngle();

	MarmaladeSystem *m_system;

	bool m_active;
	bool m_focused;
	bool m_closing;

	Rect m_rect;
	uint m_bpp;
	bool m_fullscreen;
	SCREEN_ORIENTATION_ANGLE m_screenOrientation;

	EGLDisplay m_eglDisplay;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;
};

#endif
#endif
