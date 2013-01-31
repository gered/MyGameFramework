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
	uint16_t width;
	uint16_t height;
	uint8_t bpp;
	BOOL resizable;
};

class SDLGameWindow : public GameWindow
{
public:
	SDLGameWindow(BaseGameApp *gameApp);
	virtual ~SDLGameWindow();

	BOOL Create(GameWindowParams *params);
	BOOL Resize(uint16_t width, uint16_t height);
	BOOL ToggleFullscreen();
	void Close();

	uint16_t GetWidth() const                              { return m_width; }
	uint16_t GetHeight() const                             { return m_height; }
	const Rect& GetRect() const                            { return m_rect; }
	uint8_t GetBPP() const                                 { return m_bpp; }
	BOOL IsWindowed() const                                { return m_fullscreen; }
	SCREEN_ORIENTATION_ANGLE GetScreenOrientation() const  { return m_screenOrientation; }

	BOOL IsActive() const                                  { return m_active; }
	BOOL IsFocused() const                                 { return m_focused; }
	BOOL IsClosing() const                                 { return m_closing; }
	BOOL HasGLContext() const                              { return m_hasCurrentGLContext; }

	void ProcessEvent(const OSEvent *event);
	void Flip()                                            { SDL_GL_SwapBuffers(); }

private:
	void DisplaySdlHardwareInfo();
	BOOL SetUpWindow(uint16_t width, uint16_t height);

	BOOL m_active;
	BOOL m_focused;
	BOOL m_closing;
	BOOL m_hasCurrentGLContext;

	SCREEN_ORIENTATION_ANGLE m_screenOrientation;
	uint32_t m_SDLflags;
	uint16_t m_originalWidth;
	uint16_t m_originalHeight;
	uint16_t m_width;
	uint16_t m_height;
	Rect m_rect;
	uint8_t m_bpp;
	BOOL m_fullscreen;
	SDL_Surface *m_screen;
};

#endif
#endif
