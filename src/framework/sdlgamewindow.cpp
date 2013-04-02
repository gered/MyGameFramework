#ifdef SDL
#include "debug.h"
#include "log.h"

#include "sdlgamewindow.h"

#include "basegameapp.h"
#include "gamewindow.h"
#include "screenorientation.h"
#include "sdlsystemevent.h"
#include "sdlincludes.h"

SDLGameWindow::SDLGameWindow(BaseGameApp *gameApp)
	: GameWindow(gameApp)
{
	m_active = false;
	m_focused = false;
	m_closing = false;
	m_hasCurrentGLContext = false;
	m_screenOrientation = SCREEN_ANGLE_0;
	m_SDLflags = 0;
	m_originalWidth = 0;
	m_originalHeight = 0;
	m_width = 0;
	m_height = 0;
	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = 0;
	m_rect.bottom = 0;
	m_bpp = 0;
	m_fullscreen = false;
	m_screen = NULL;
}

SDLGameWindow::~SDLGameWindow()
{
	LOG_INFO(LOGCAT_WINDOW, "Releasing.\n");
}

bool SDLGameWindow::Create(GameWindowParams *params)
{
	LOG_INFO(LOGCAT_WINDOW, "Creating a window.\n");

	SDLGameWindowParams *sdlParams = (SDLGameWindowParams*)params;

	LOG_INFO(LOGCAT_WINDOW, "Received window creation parameters:\n");
	LOG_INFO(LOGCAT_WINDOW, "  title = \"%s\"\n", sdlParams->title.c_str());
	LOG_INFO(LOGCAT_WINDOW, "  width = %d\n", sdlParams->width);
	LOG_INFO(LOGCAT_WINDOW, "  height = %d\n", sdlParams->height);
	LOG_INFO(LOGCAT_WINDOW, "  bpp = %d\n", sdlParams->bpp);
	LOG_INFO(LOGCAT_WINDOW, "  windowed = %d\n", sdlParams->windowed);
	LOG_INFO(LOGCAT_WINDOW, "  resizable = %d\n", sdlParams->resizable);

	uint flags = SDL_OPENGL;
	if (!sdlParams->windowed)
		flags |= SDL_FULLSCREEN;
	if (sdlParams->resizable)
		flags |= SDL_RESIZABLE;

	m_SDLflags = flags;
	m_bpp = sdlParams->bpp;
	m_fullscreen = !sdlParams->windowed;

	// Trust that the initial video resolution specified will work, and save it as the fallback
	m_originalWidth = sdlParams->width;
	m_originalHeight = sdlParams->height;

	DisplaySdlHardwareInfo();

	// Set the video mode
	if (!SetUpWindow(sdlParams->width, sdlParams->height))
		return false;

	SDL_WM_SetCaption(sdlParams->title.c_str(), sdlParams->title.c_str());
	LOG_INFO(LOGCAT_WINDOW, "Application window caption set: \"%s\"\n", sdlParams->title.c_str());

	LOG_INFO(LOGCAT_WINDOW, "Finished initialization.\n");

	LOG_INFO(LOGCAT_WINDOW, "Window marked active.\n");
	m_active = true;
	m_focused = true;

	return true;
}

bool SDLGameWindow::Resize(uint width, uint height)
{
	bool result = SetUpWindow(width, height);
	GetGameApp()->OnNewContext();
	GetGameApp()->OnResize();
	return result;
}

bool SDLGameWindow::ToggleFullscreen()
{
	bool screenToggleResult;
	if (m_fullscreen)
		LOG_INFO(LOGCAT_WINDOW, "Beginning switch to windowed mode...\n");
	else
		LOG_INFO(LOGCAT_WINDOW, "Beginning switch to fullscreen mode...\n");

	GetGameApp()->OnLostContext();
	m_hasCurrentGLContext = false;

	m_SDLflags ^= SDL_FULLSCREEN;
	if (m_SDLflags & SDL_FULLSCREEN)
		m_fullscreen = true;
	else
		m_fullscreen = false;

	screenToggleResult = Resize(m_width, m_height);
	if (!screenToggleResult)
	{
		LOG_ERROR(LOGCAT_WINDOW, "Failed to toggle fullscreen/windowed mode (%dx%dx%d, fullscreen = %d).\n", m_width, m_height, m_bpp, m_fullscreen);
		ASSERT(screenToggleResult == true);
		return false;
	}

	if (m_fullscreen)
		LOG_INFO(LOGCAT_WINDOW, "Finished switch to fullscreen mode.\n");
	else
		LOG_INFO(LOGCAT_WINDOW, "Finished switch to windowed mode.\n");

	return true;
}

void SDLGameWindow::DisplaySdlHardwareInfo()
{
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	char *videoDriver = new char[100];
	videoDriver = SDL_VideoDriverName(videoDriver, 100);
	ASSERT(videoDriver != NULL);

	LOG_INFO(LOGCAT_WINDOW, "SDL renderer using the following video hardware/implementation:\n");
	LOG_INFO(LOGCAT_WINDOW, "  Video driver: %s\n", videoDriver);
	LOG_INFO(LOGCAT_WINDOW, "  hw_available: %ld\n", videoInfo->hw_available);
	LOG_INFO(LOGCAT_WINDOW, "  wm_available: %ld\n", videoInfo->wm_available);
	LOG_INFO(LOGCAT_WINDOW, "  blit_hw: %ld\n", videoInfo->blit_hw);
	LOG_INFO(LOGCAT_WINDOW, "  blit_hw_CC: %ld\n", videoInfo->blit_hw_CC);
	LOG_INFO(LOGCAT_WINDOW, "  blit_hw_A: %ld\n", videoInfo->blit_hw_A);
	LOG_INFO(LOGCAT_WINDOW, "  blit_sw: %ld\n", videoInfo->blit_sw);
	LOG_INFO(LOGCAT_WINDOW, "  blit_sw_CC: %ld\n", videoInfo->blit_sw_CC);
	LOG_INFO(LOGCAT_WINDOW, "  blit_sw_A: %ld\n", videoInfo->blit_sw_A);
	LOG_INFO(LOGCAT_WINDOW, "  blit_fill: %ld\n", videoInfo->blit_fill);
	LOG_INFO(LOGCAT_WINDOW, "  video_mem: %ld\n", videoInfo->video_mem);
	LOG_INFO(LOGCAT_WINDOW, "  vfmt: %ld (best)\n", videoInfo->vfmt->BitsPerPixel);
	LOG_INFO(LOGCAT_WINDOW, "  current_w: %ld\n", videoInfo->current_w);
	LOG_INFO(LOGCAT_WINDOW, "  current_h: %ld\n", videoInfo->current_h);

	delete videoDriver;

	LOG_INFO(LOGCAT_WINDOW, "SDL Video Flags selected: SDL_OPENGL%s%s\n", (m_SDLflags & SDL_FULLSCREEN) ? ", SDL_FULLSCREEN" : "", (m_SDLflags & SDL_RESIZABLE) ? ", SDL_RESIZABLE" : "");

	uint tempFlags = m_SDLflags;
	if (tempFlags & SDL_RESIZABLE)
		tempFlags ^= SDL_RESIZABLE;

	SDL_Rect **windowedModes = SDL_ListModes(NULL, tempFlags);
	SDL_Rect **fullscreenModes = SDL_ListModes(NULL, tempFlags | SDL_FULLSCREEN);

	ASSERT(windowedModes != NULL);
	ASSERT(fullscreenModes != NULL);

	// Windowed display resolutions available
	LOG_INFO(LOGCAT_WINDOW, "Supported windowed video modes with selected flags:\n");
	if (windowedModes == (SDL_Rect**)-1)
		LOG_INFO(LOGCAT_WINDOW, "  All video modes are available.\n");
	else
	{
		for (int i = 0; windowedModes[i]; i++)
			LOG_INFO(LOGCAT_WINDOW, "  %d. %d x %d\n", i + 1, windowedModes[i]->w, windowedModes[i]->h);
	}

	// Fullscreen display resolutions available
	LOG_INFO(LOGCAT_WINDOW, "Supported fullscreen video modes with selected flags:\n");
	if (fullscreenModes == (SDL_Rect**)-1)
		LOG_INFO(LOGCAT_WINDOW, "  All video modes are available.\n");
	else
	{
		for (int i = 0; fullscreenModes[i]; i++)
			LOG_INFO(LOGCAT_WINDOW, "  %d. %d x %d\n", i + 1, fullscreenModes[i]->w, fullscreenModes[i]->h);
	}
}

bool SDLGameWindow::SetUpWindow(uint width, uint height)
{
	int ret;
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();

	LOG_INFO(LOGCAT_WINDOW, "Attempting to set up new window: %dx%d @ %d bpp, %s.\n", width, height, m_bpp, (m_SDLflags & SDL_FULLSCREEN ? "fullscreen" : "windowed"));

	// Check for requested video mode
	ret = SDL_VideoModeOK(width, height, (m_bpp != 0 ? m_bpp : videoInfo->vfmt->BitsPerPixel), m_SDLflags);
	if (!ret)
	{
		ret = SDL_VideoModeOK(m_originalWidth, m_originalHeight, (m_bpp != 0 ? m_bpp : videoInfo->vfmt->BitsPerPixel), m_SDLflags);
		if (!ret)
		{
			LOG_WARN(LOGCAT_WINDOW, "Video mode %dx%d @ %d bpp not supported.\n", width, height, m_bpp);
			return false;
		}

		width = m_originalWidth;
		height = m_originalHeight;
		LOG_WARN(LOGCAT_WINDOW, "Fall back to application's original display resolution %dx%d.\n", m_originalWidth, m_originalHeight);
	}

	LOG_INFO(LOGCAT_WINDOW, "SDL_VideoModeOK() returned a BPP of %d.\n", ret);

	// Request minimum settings
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if ((m_screen = SDL_SetVideoMode(width, height, m_bpp, m_SDLflags)) == NULL)
	{
		LOG_ERROR(LOGCAT_WINDOW, "Could not set video mode %dx%dx%d (flags = %ld): %s\n", width, height, m_bpp, m_SDLflags, SDL_GetError());
		return false;
	}
	LOG_INFO(LOGCAT_WINDOW, "Video mode %dx%d @ %d bpp set.\n", width, height, m_bpp);

	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
	LOG_INFO(LOGCAT_WINDOW, "SDL_GL_RED_SIZE: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
	LOG_INFO(LOGCAT_WINDOW, "SDL_GL_GREEN_SIZE: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
	LOG_INFO(LOGCAT_WINDOW, "SDL_GL_BLUE_SIZE: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
	LOG_INFO(LOGCAT_WINDOW, "SDL_GL_DEPTH_SIZE: %d\n", value);
	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
	LOG_INFO(LOGCAT_WINDOW, "SDL_GL_DOUBLEBUFFER: %d\n", value);

	if (GetScreenOrientation() == SCREEN_ANGLE_90 || GetScreenOrientation() == SCREEN_ANGLE_270)
	{
		m_width = height;
		m_height = width;
		m_rect.left = 0;
		m_rect.top = 0;
		m_rect.right = m_rect.top + height;
		m_rect.bottom = m_rect.left + width;
	}
	else
	{
		m_width = width;
		m_height = height;
		m_rect.left = 0;
		m_rect.top = 0;
		m_rect.right = m_rect.left + width;
		m_rect.bottom = m_rect.top + height;
	}

	m_hasCurrentGLContext = true;

	return true;
}

void SDLGameWindow::ProcessEvent(const OSEvent *event)
{
	if (IsClosing())
		return;

	const SDLSystemEvent *evt = (SDLSystemEvent*)event;
	bool resizeResult;

	switch (evt->event->type)
	{
	case SDL_ACTIVEEVENT:
		LOG_INFO(LOGCAT_WINDOW, "Event: SDL_ACTIVEEVENT\n");
		if (evt->event->active.state & SDL_APPACTIVE)
		{
			// Disable rendering if the application was minimized
			if (evt->event->active.gain)
			{
				LOG_INFO(LOGCAT_WINDOW, "Window focus gained.\n");
				LOG_INFO(LOGCAT_WINDOW, "Window marked active.\n");
				m_active = true;

				// ensure we render again now that rendering has been resumed
				// (the window might have been minimized or covered up)
				GetGameApp()->Invalidate();
			}
			else
			{
				LOG_INFO(LOGCAT_WINDOW, "Window focus lost.\n");
				LOG_INFO(LOGCAT_WINDOW, "Window marked inactive.\n");
				m_active = false;
			}
		}
		if (evt->event->active.state & SDL_APPMOUSEFOCUS)
		{
			if (evt->event->active.gain)
				LOG_INFO(LOGCAT_WINDOW, "Gained mouse focus.\n");
			else
				LOG_INFO(LOGCAT_WINDOW, "Lost mouse focus.\n");
		}
		if (evt->event->active.state & SDL_APPINPUTFOCUS)
		{
			if (evt->event->active.gain)
			{
				LOG_INFO(LOGCAT_WINDOW, "Gained input device focus.\n");
				m_focused = true;
				GetGameApp()->OnAppGainFocus();
			}
			else
			{
				LOG_INFO(LOGCAT_WINDOW, "Lost input device focus.\n");
				m_focused = false;
				GetGameApp()->OnAppLostFocus();
			}
		}
		break;

	case SDL_VIDEORESIZE:
		LOG_INFO(LOGCAT_WINDOW, "Event: SDL_VIDEORESIZE\n");
		// Window resizing involves resetting the SDL video mode, which therefore also requires
		// re-initializing OpenGL
		LOG_INFO(LOGCAT_WINDOW, "Window resized to %dx%d.\n", evt->event->resize.w, evt->event->resize.h);
		GetGameApp()->OnLostContext();
		m_hasCurrentGLContext = false;
		resizeResult = SetUpWindow(evt->event->resize.w, evt->event->resize.h);
		GetGameApp()->OnNewContext();
		GetGameApp()->OnResize();
		if (!resizeResult)
		{
			LOG_ERROR(LOGCAT_WINDOW, "Window resize to %dx%d failed!\n", evt->event->resize.w, evt->event->resize.h);
			ASSERT(resizeResult == true);
			Close();
			return;
		}
		break;
	}
}

void SDLGameWindow::Close()
{
	if (m_active)
	{
		LOG_INFO(LOGCAT_WINDOW, "Window marked inactive.\n");
		m_active = false;
		m_focused = false;
	}
	else
		LOG_INFO(LOGCAT_WINDOW, "Window was already marked inactive.\n");

	LOG_INFO(LOGCAT_WINDOW, "Closing.\n");
	m_closing = true;
}

#endif
