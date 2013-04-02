#ifdef SDL
#ifndef __FRAMEWORK_SDLGAMEWINDOW_H_INCLUDED__
#define __FRAMEWORK_SDLGAMEWINDOW_H_INCLUDED__

#include "common.h"
#include "gamewindow.h"
#include "screenorientation.h"
#include "sdlincludes.h"
#include <stl/string.h>

#include "math/rect.h"

class BaseGameApp;

struct SDLGameWindowParams : GameWindowParams
{
	stl::string title;
	uint width;
	uint height;
	uint bpp;
	bool resizable;
};

class SDLGameWindow : public GameWindow
{
public:
	SDLGameWindow(BaseGameApp *gameApp);
	virtual ~SDLGameWindow();

	bool Create(GameWindowParams *params);
	bool Resize(uint width, uint height);
	bool ToggleFullscreen();
	void Close();

	uint GetWidth() const                                  { return m_width; }
	uint GetHeight() const                                 { return m_height; }
	const Rect& GetRect() const                            { return m_rect; }
	uint GetBPP() const                                    { return m_bpp; }
	bool IsWindowed() const                                { return m_fullscreen; }
	SCREEN_ORIENTATION_ANGLE GetScreenOrientation() const  { return m_screenOrientation; }

	bool IsActive() const                                  { return m_active; }
	bool IsFocused() const                                 { return m_focused; }
	bool IsClosing() const                                 { return m_closing; }
	bool HasGLContext() const                              { return m_hasCurrentGLContext; }

	void ProcessEvent(const OSEvent *event);
	void Flip()                                            { SDL_GL_SwapBuffers(); }

private:
	void DisplaySdlHardwareInfo();
	bool SetUpWindow(uint width, uint height);

	bool m_active;
	bool m_focused;
	bool m_closing;
	bool m_hasCurrentGLContext;

	SCREEN_ORIENTATION_ANGLE m_screenOrientation;
	uint m_SDLflags;
	uint m_originalWidth;
	uint m_originalHeight;
	uint m_width;
	uint m_height;
	Rect m_rect;
	uint m_bpp;
	bool m_fullscreen;
	SDL_Surface *m_screen;
};

#endif
#endif
