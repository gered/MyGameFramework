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

	BOOL Create(GameWindowParams *params);
	BOOL Resize(uint width = IGNORE_DIMENSIONS, uint height = IGNORE_DIMENSIONS);
	BOOL ToggleFullscreen();
	void Close();

	uint GetWidth() const                                  { return m_rect.GetWidth(); }
	uint GetHeight() const                                 { return m_rect.GetHeight(); }
	const Rect& GetRect() const                            { return m_rect; }
	uint GetBPP() const                                    { return m_bpp; }
	BOOL IsWindowed() const                                { return !m_fullscreen; }
	SCREEN_ORIENTATION_ANGLE GetScreenOrientation() const  { return m_screenOrientation; }

	BOOL IsActive() const                                  { return m_active; }
	BOOL IsFocused() const                                 { return m_focused; }
	BOOL IsClosing() const                                 { return m_closing; }
	BOOL HasGLContext() const                              { return eglGetCurrentContext() != EGL_NO_CONTEXT; }

	void ProcessEvent(const OSEvent *event);
	void Flip()                                            { eglSwapBuffers(m_eglDisplay, m_eglSurface); }

private:
	BOOL SetUpWindow();
	BOOL DestroyWindow();

	BOOL SetUpEGL();
	BOOL DestroyEGL();

	SCREEN_ORIENTATION_ANGLE GetCurrentScreenOrientationAngle();

	MarmaladeSystem *m_system;

	BOOL m_active;
	BOOL m_focused;
	BOOL m_closing;

	Rect m_rect;
	uint m_bpp;
	BOOL m_fullscreen;
	SCREEN_ORIENTATION_ANGLE m_screenOrientation;

	EGLDisplay m_eglDisplay;
	EGLSurface m_eglSurface;
	EGLContext m_eglContext;
};

#endif
#endif
