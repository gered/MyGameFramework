#ifdef __S3E__
#include "common.h"

#include "debug.h"
#include "log.h"

#include "basegameapp.h"
#include "marmaladegamewindow.h"
#include "marmaladesystemevent.h"
#include "screenorientation.h"
#include "graphics/glincludes.h"

#include "s3e.h"

MarmaladeGameWindow::MarmaladeGameWindow(BaseGameApp *gameApp, MarmaladeSystem *system)
	: GameWindow(gameApp)
{
	m_system = system;
	m_active = FALSE;
	m_focused = FALSE;
	m_closing = FALSE;
	m_rect = Rect(0, 0, 0, 0);
	m_bpp = 0;
	m_fullscreen = TRUE;
	m_screenOrientation = SCREEN_ANGLE_0;
	m_eglContext = EGL_NO_CONTEXT;
	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglSurface = EGL_NO_SURFACE;
}

MarmaladeGameWindow::~MarmaladeGameWindow()
{
	LOG_INFO(LOGCAT_WINDOW, "Releasing.\n");
	if (!DestroyWindow())
		LOG_ERROR(LOGCAT_WINDOW, "Failed to destroy the EGL context.\n");
}

BOOL MarmaladeGameWindow::Create(GameWindowParams *params)
{
	LOG_INFO(LOGCAT_WINDOW, "Creating a window.\n");

	LOG_INFO(LOGCAT_WINDOW, "Received window creation parameters:\n");
	LOG_INFO(LOGCAT_WINDOW, "  windowed = %d\n", params->windowed);

	m_fullscreen = !params->windowed;

	// Set up the window and EGL context
	if (!SetUpWindow())
		return FALSE;

	LOG_INFO(LOGCAT_WINDOW, "Finished initialization.\n");

	LOG_INFO(LOGCAT_WINDOW, "Window marked active.\n");
	m_active = TRUE;
	m_focused = TRUE;

	return TRUE;
}

BOOL MarmaladeGameWindow::Resize(uint16_t width, uint16_t height)
{
	// note that the parameters are ignored completely because they don't really
	// make sense for our primary/only usage of Marmalade (mobile devices)
	// the parameters need to be there obviously because of the GameWindow parent 
	// class declaration.
	ASSERT(m_eglDisplay != EGL_NO_DISPLAY);

	LOG_INFO(LOGCAT_WINDOW, "Querying for new EGL display dimensions.\n");

	EGLint newWidth;
	EGLint newHeight;
	eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &newWidth);
	eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &newHeight);

	LOG_INFO(LOGCAT_WINDOW, "EGL display dimensions: %dx%d\n", newWidth, newHeight);

	int surfaceWidth = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	int surfaceHeight = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	int surfaceBpp = s3eSurfaceGetInt(S3E_SURFACE_PIXEL_TYPE) & S3E_SURFACE_PIXEL_SIZE_MASK;

	LOG_INFO(LOGCAT_WINDOW, "Surface dimensions resized to %dx%d with a bpp of %d.\n", surfaceWidth, surfaceHeight, surfaceBpp);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = surfaceWidth;
	m_rect.bottom = surfaceHeight;
	m_bpp = surfaceBpp;

	LOG_INFO(LOGCAT_WINDOW, "Window content area set to %d, %d, %d, %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

	SCREEN_ORIENTATION_ANGLE angle = GetCurrentScreenOrientationAngle();
	LOG_INFO(LOGCAT_WINDOW, "Device's current screen orientation angle: %d\n", angle);
	m_screenOrientation = angle;

	return TRUE;
}

BOOL MarmaladeGameWindow::ToggleFullscreen()
{
	ASSERT(!"Not implemented.");
	return FALSE;
}

BOOL MarmaladeGameWindow::SetUpWindow()
{
	if (!SetUpEGL())
	{
		LOG_ERROR(LOGCAT_WINDOW, "EGL setup not completed successfully.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL MarmaladeGameWindow::DestroyWindow()
{
	if (!DestroyEGL())
	{
		LOG_ERROR(LOGCAT_WINDOW, "EGL destroy not completed successfully.\n");
		return FALSE;
	}

	return TRUE;
}

BOOL MarmaladeGameWindow::SetUpEGL()
{
	LOG_INFO(LOGCAT_WINDOW, "Connecting to EGL display server.\n");
	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (display == EGL_NO_DISPLAY)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglGetDisplay() failed.\n");
		return FALSE;
	}

	LOG_INFO(LOGCAT_WINDOW, "Initializing EGL.\n");
	if (eglInitialize(display, 0, 0) == EGL_FALSE)
	{
		EGLint initError = eglGetError();
		if (initError == EGL_BAD_DISPLAY)
			LOG_ERROR(LOGCAT_WINDOW, "eglInitialize() failed: EGL_BAD_DISPLAY\n");
		else if (initError == EGL_NOT_INITIALIZED)
			LOG_ERROR(LOGCAT_WINDOW, "eglInitialize() failed: EGL_NOT_INITIALIZED\n");
		else
			LOG_ERROR(LOGCAT_WINDOW, "eglInitialize() failed.\n");
		return FALSE;
	}

	LOG_INFO(LOGCAT_WINDOW, "Querying available EGL configs.\n");
	const EGLint MAX_EGL_CONFIGS = 30;   // Too much? Or good enough?
	EGLint numConfigsFound = 0;
	EGLConfig configs[MAX_EGL_CONFIGS];
	if (eglGetConfigs(display, configs, MAX_EGL_CONFIGS, &numConfigsFound) == EGL_FALSE)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglGetConfigs() failed.\n");
		return FALSE;
	}
	if (numConfigsFound == 0)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglGetConfigs() found 0 EGL configs. Aborting.\n");
		return FALSE;
	}

	// TODO: these should probably come from a config file or something
	EGLint desiredContextVersion = EGL_OPENGL_ES2_BIT;
	EGLint desiredSurfaceType = EGL_WINDOW_BIT;
	EGLint desiredBlueSize = 8;
	EGLint desiredGreenSize = 8;
	EGLint desiredRedSize = 8;
	EGLint desiredDepthSize = 24;

	LOG_INFO(LOGCAT_WINDOW, "Found %d configs. Searching for an exact match:\n", numConfigsFound);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_RENDERABLE_TYPE = 0x%x\n", desiredContextVersion);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_SURFACE_TYPE = 0x%x\n", desiredSurfaceType);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_BLUE_SIZE = %d\n", desiredBlueSize);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_GREEN_SIZE = %d\n", desiredGreenSize);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_RED_SIZE = %d\n", desiredRedSize);
	LOG_INFO(LOGCAT_WINDOW, "  EGL_DEPTH_SIZE = %d\n", desiredDepthSize);

	EGLint matchingConfigIndex = -1;
	EGLConfig config;

	// first try to find an exact match
	for (EGLint i = 0; i < numConfigsFound; ++i)
	{
		EGLint contextVersionSupport = 0;
		EGLint surfaceType = 0;
		EGLint blueSize = 0;
		EGLint greenSize = 0;
		EGLint redSize = 0;
		EGLint depthSize = 0;
		eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &contextVersionSupport);
		eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &surfaceType);
		eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &blueSize);
		eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &greenSize);
		eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &redSize);
		eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depthSize);
		LOG_INFO(LOGCAT_WINDOW, "Config %d: renderableType = 0x%x, type = 0x%x, b = %d, g = %d, r = %d, depth = %d\n", i, contextVersionSupport, surfaceType, blueSize, greenSize, redSize, depthSize);
		if (contextVersionSupport & desiredContextVersion && surfaceType & desiredSurfaceType && blueSize == desiredBlueSize && greenSize == desiredGreenSize && redSize == desiredRedSize && depthSize == desiredDepthSize)
		{
			LOG_INFO(LOGCAT_WINDOW, "Found matching config: %d.\n", i);
			matchingConfigIndex = i;
			break;
		}
	}

	// if we didn't find an exact match, lets now try and find the first config
	// where everything except the depth buffer size matches, but make sure
	// that the config at least has a non-zero depth buffer.
	// We do this as it seems like the depth buffer size is the most likely one
	// that won't match our initial search criteria.
	if (matchingConfigIndex == -1)
	{
		LOG_WARN(LOGCAT_WINDOW, "No exact config match found. Searching for first config matching:\n");
		LOG_INFO(LOGCAT_WINDOW, "  EGL_RENDERABLE_TYPE = 0x%x\n", desiredContextVersion);
		LOG_WARN(LOGCAT_WINDOW, "  EGL_SURFACE_TYPE = 0x%x\n", desiredSurfaceType);
		LOG_WARN(LOGCAT_WINDOW, "  EGL_BLUE_SIZE = %d\n", desiredBlueSize);
		LOG_WARN(LOGCAT_WINDOW, "  EGL_GREEN_SIZE = %d\n", desiredGreenSize);
		LOG_WARN(LOGCAT_WINDOW, "  EGL_RED_SIZE = %d\n", desiredRedSize);
		LOG_WARN(LOGCAT_WINDOW, "  EGL_DEPTH_SIZE > 0\n");

		for (EGLint i = 0; i < numConfigsFound; ++i)
		{
			EGLint contextVersionSupport = 0;
			EGLint surfaceType = 0;
			EGLint blueSize = 0;
			EGLint greenSize = 0;
			EGLint redSize = 0;
			EGLint depthSize = 0;
			eglGetConfigAttrib(display, configs[i], EGL_RENDERABLE_TYPE, &contextVersionSupport);
			eglGetConfigAttrib(display, configs[i], EGL_SURFACE_TYPE, &surfaceType);
			eglGetConfigAttrib(display, configs[i], EGL_BLUE_SIZE, &blueSize);
			eglGetConfigAttrib(display, configs[i], EGL_GREEN_SIZE, &greenSize);
			eglGetConfigAttrib(display, configs[i], EGL_RED_SIZE, &redSize);
			eglGetConfigAttrib(display, configs[i], EGL_DEPTH_SIZE, &depthSize);
			LOG_WARN(LOGCAT_WINDOW, "Config %d: renderableType = 0x%x, type = 0x%x, b = %d, g = %d, r = %d, depth = %d\n", i, contextVersionSupport, surfaceType, blueSize, greenSize, redSize, depthSize);
			if (contextVersionSupport & desiredContextVersion && surfaceType & desiredSurfaceType && blueSize == desiredBlueSize && greenSize == desiredGreenSize && redSize == desiredRedSize && depthSize > 0)
			{
				LOG_WARN(LOGCAT_WINDOW, "Found matching config: %d.\n", i);
				matchingConfigIndex = i;
				break;
			}
		}
	}

	// if we still didn't find a match, just use the first config from the list
	if (matchingConfigIndex == -1)
	{
		LOG_WARN(LOGCAT_WINDOW, "No matching config found. Using config 0 from the previous list as a fallback.\n");
		matchingConfigIndex = 0;
	}

	config = configs[matchingConfigIndex];

	LOG_INFO(LOGCAT_WINDOW, "Creating EGL window.\n");
	void *nativeWindowHandle = s3eGLGetNativeWindow();
	EGLSurface surface = eglCreateWindowSurface(display, config, nativeWindowHandle, NULL);
	if (surface == EGL_NO_SURFACE)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglCreateWindowSurface() failed.\n");
		return FALSE;
	}

	LOG_INFO(LOGCAT_WINDOW, "Creating rendering context.\n");
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, };
	EGLContext context = eglCreateContext(display, config, NULL, contextAttribs);
	if (context == EGL_NO_CONTEXT)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglCreateContext() failed.\n");
		return FALSE;
	}

	LOG_INFO(LOGCAT_WINDOW, "Making this rendering context current.\n");
	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		LOG_ERROR(LOGCAT_WINDOW, "eglMakeCurrent() failed.\n");
		return FALSE;
	}

	LOG_INFO(LOGCAT_WINDOW, "EGL window set up successfully.\n");

	EGLint width;
	EGLint height;
	eglQuerySurface(display, surface, EGL_WIDTH, &width);
	eglQuerySurface(display, surface, EGL_HEIGHT, &height);

	LOG_INFO(LOGCAT_WINDOW, "EGL display dimensions: %dx%d\n", width, height);

	m_eglDisplay = display;
	m_eglSurface = surface;
	m_eglContext = context;

	int surfaceWidth = s3eSurfaceGetInt(S3E_SURFACE_WIDTH);
	int surfaceHeight = s3eSurfaceGetInt(S3E_SURFACE_HEIGHT);
	int surfaceBpp = s3eSurfaceGetInt(S3E_SURFACE_PIXEL_TYPE) & S3E_SURFACE_PIXEL_SIZE_MASK;

	LOG_INFO(LOGCAT_WINDOW, "Surface dimensions of %dx%d with a bpp of %d.\n", surfaceWidth, surfaceHeight, surfaceBpp);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = surfaceWidth;
	m_rect.bottom = surfaceHeight;
	m_bpp = surfaceBpp;

	LOG_INFO(LOGCAT_WINDOW, "Window content area set to %d, %d, %d, %d\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

	SCREEN_ORIENTATION_ANGLE angle = GetCurrentScreenOrientationAngle();
	LOG_INFO(LOGCAT_WINDOW, "Device's current screen orientation angle: %d\n", angle);
	m_screenOrientation = angle;

	return TRUE;
}

BOOL MarmaladeGameWindow::DestroyEGL()
{
	if (m_eglDisplay != EGL_NO_DISPLAY)
	{
		LOG_INFO(LOGCAT_WINDOW, "Destroying EGL context.\n");

		// Used to check the return value from this call, but it seems that every code sample,
		// article, book, or whatever doesn't do that ever. And this seems to always fail in
		// the Marmalade simulator (return EGL_FALSE).
		eglMakeCurrent(EGL_NO_DISPLAY, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

		if (m_eglContext != EGL_NO_CONTEXT)
		{
			if (eglDestroyContext(m_eglDisplay, m_eglContext) == EGL_FALSE)
			{
				LOG_ERROR(LOGCAT_WINDOW, "Error destroying EGL context.\n");
				return FALSE;
			}
		}
		if (m_eglSurface != EGL_NO_SURFACE)
		{
			if (eglDestroySurface(m_eglDisplay, m_eglSurface) == EGL_FALSE)
			{
				LOG_ERROR(LOGCAT_WINDOW, "Error destroying EGL surface.\n");
				return FALSE;
			}
		}
		if (eglTerminate(m_eglDisplay) == EGL_FALSE)
		{
			LOG_ERROR(LOGCAT_WINDOW, "Error terminating EGL display.\n");
			return FALSE;
		}
	}
	m_eglDisplay = EGL_NO_DISPLAY;
	m_eglContext = EGL_NO_CONTEXT;
	m_eglSurface = EGL_NO_SURFACE;

	return TRUE;
}

SCREEN_ORIENTATION_ANGLE MarmaladeGameWindow::GetCurrentScreenOrientationAngle()
{
	s3eSurfaceBlitDirection direction = (s3eSurfaceBlitDirection)s3eSurfaceGetInt(S3E_SURFACE_DEVICE_BLIT_DIRECTION);

	SCREEN_ORIENTATION_ANGLE angle = SCREEN_ANGLE_0;
	switch (direction)
	{
	case S3E_SURFACE_BLIT_DIR_NORMAL:
	case S3E_SURFACE_BLIT_DIR_NATIVE:
		angle = SCREEN_ANGLE_0;
		break;
	case S3E_SURFACE_BLIT_DIR_ROT90:
		angle = SCREEN_ANGLE_90;
		break;
	case S3E_SURFACE_BLIT_DIR_ROT180:
		angle = SCREEN_ANGLE_180;
		break;
	case S3E_SURFACE_BLIT_DIR_ROT270:
		angle = SCREEN_ANGLE_270;
		break;
	}

	return angle;
}

void MarmaladeGameWindow::Close()
{
	if (m_active)
	{
		LOG_INFO(LOGCAT_WINDOW, "Window marked inactive.\n");
		m_active = FALSE;
		m_focused = FALSE;
	}
	else
		LOG_INFO(LOGCAT_WINDOW, "Window was already marked inactive.\n");

	LOG_INFO(LOGCAT_WINDOW, "Closing.\n");
	m_closing = TRUE;
}

void MarmaladeGameWindow::ProcessEvent(const OSEvent *event)
{
	if (IsClosing())
		return;

	MarmaladeSystemEvent *eventArgs = (MarmaladeSystemEvent*)event;

	switch (eventArgs->event)
	{
	case MARMALADE_EVENT_PAUSE:
		LOG_INFO(LOGCAT_WINDOW, "Event: MARMALADE_EVENT_PAUSE\n");
		if (m_active)
		{
			LOG_INFO(LOGCAT_WINDOW, "Window marked inactive.\n");
			m_active = FALSE;
			m_focused = FALSE;
			LOG_INFO(LOGCAT_WINDOW, "Lost input device focus.\n");
			GetGameApp()->OnAppLostFocus();
			LOG_INFO(LOGCAT_WINDOW, "Pausing app.\n");
			GetGameApp()->OnAppPause();
		}
		else
			LOG_INFO(LOGCAT_WINDOW, "Window was already marked inactive.\n");
		break;

	case MARMALADE_EVENT_RESUME:
		LOG_INFO(LOGCAT_WINDOW, "Event: MARMALADE_EVENT_RESUME\n");
		if (!m_active)
		{
			LOG_INFO(LOGCAT_WINDOW, "Window marked active.\n");
			m_active = TRUE;
			m_focused = TRUE;
			LOG_INFO(LOGCAT_WINDOW, "Resuming app.\n");
			GetGameApp()->OnAppResume();
			LOG_INFO(LOGCAT_WINDOW, "Gained input device focus.\n");
			GetGameApp()->OnAppGainFocus();
		}
		else
			LOG_INFO(LOGCAT_WINDOW, "Window was already marked active.\n");
		break;

	case MARMALADE_EVENT_GLSUSPEND:
		LOG_INFO(LOGCAT_WINDOW, "Event: MARMALADE_EVENT_GLSUSPEND\n");
		GetGameApp()->OnLostContext();
		if (!DestroyWindow())
		{
			LOG_ERROR(LOGCAT_WINDOW, "Error destroying EGL context.\n");
			Close();
			return;
		}
		break;

	case MARMALADE_EVENT_GLRESUME:
		LOG_INFO(LOGCAT_WINDOW, "Event: MARMALADE_EVENT_GLRESUME\n");
		if (!SetUpWindow())
		{
			LOG_ERROR(LOGCAT_WINDOW, "Error creating EGL context.\n");
			Close();
			return;
		}
		GetGameApp()->OnNewContext();

		// Not sure if there will ever be a case where a screen resize
		// event will be raised immediately after (or before?) a GL
		// resume event. If that's the case, we could end up calling
		// BaseGameApp's OnResize() event twice...
		GetGameApp()->OnResize();
		break;

	case MARMALADE_EVENT_SCREENRESIZE:
		LOG_INFO(LOGCAT_WINDOW, "Event: MARMALADE_EVENT_SCREENRESIZE\n");
		if (!Resize())
		{
			LOG_ERROR(LOGCAT_WINDOW, "Window resize failed.\n");
			Close();
			return;
		}
		GetGameApp()->OnResize();
		break;
	}
}

#endif
