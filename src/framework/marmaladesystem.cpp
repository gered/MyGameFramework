#ifdef __S3E__
#include "common.h"

#include "debug.h"
#include "log.h"

#include "marmaladesystem.h"
#include "marmaladesystemevent.h"
#include "operatingsystem.h"
#include "basegameapp.h"
#include "gamewindow.h"
#include "marmaladegamewindow.h"
#include "file/marmaladefilesystem.h"
#include "graphics/glincludes.h"
#include "input/marmaladekeyboard.h"
#include "input/marmalademouse.h"
#include "input/marmaladetouchscreen.h"

#include "s3e.h"
#include "MarmaladeVersion.h"

#include <string.h>

int32 _MarmaladeEventCallback_Pause(void *systemData, void *userData);
int32 _MarmaladeEventCallback_Resume(void *systemData, void *userData);
int32 _MarmaladeEventCallback_Exit(void *systemData, void *userData);
int32 _MarmaladeEventCallback_GLSuspend(void *systemData, void *userData);
int32 _MarmaladeEventCallback_GLResume(void *systemData, void *userData);
int32 _MarmaladeEventCallback_ScreenResize(void *systemData, void *userData);
int32 _MarmaladeEventCallback_Key(void *systemData, void *userData);
int32 _MarmaladeEventCallback_KeyChar(void *systemData, void *userData);
int32 _MarmaladeEventCallback_PointerButton(void *systemData, void *userData);
int32 _MarmaladeEventCallback_PointerMotion(void *systemData, void *userData);
int32 _MarmaladeEventCallback_PointerMultitouchButton(void *systemData, void *userData);
int32 _MarmaladeEventCallback_PointerMultitouchMotion(void *systemData, void *userData);
int32_t _MarmaladeEvent_PassToSystem(MARMALADE_EVENT event, void *systemData, void *userData);

MarmaladeSystem::MarmaladeSystem()
	: OperatingSystem()
{
	STACK_TRACE;
	m_isQuitting = FALSE;
	m_window = NULL;
	m_filesystem = NULL;
	m_keyboard = NULL;
	m_mouse = NULL;
	m_touchscreen = NULL;
	m_hasShaderSupport = FALSE;
	m_supportedShaderVersion = 0.0f;
}

MarmaladeSystem::~MarmaladeSystem()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_SYSTEM, "Releasing.\n");

	if (!m_isQuitting)
	{
		LOG_WARN(LOGCAT_SYSTEM, "Was not signaled to quit. Calling Quit() now.\n");
		Quit();
	}

	SAFE_DELETE(m_window);

	SAFE_DELETE(m_filesystem);
	SAFE_DELETE(m_keyboard);
	SAFE_DELETE(m_mouse);
	SAFE_DELETE(m_touchscreen);
}

BOOL MarmaladeSystem::Initialize()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_SYSTEM, "MarmaladeSystem initialization starting.\n");

	const char* marmaladeRuntimeVersion = s3eDeviceGetString(S3E_DEVICE_SDK_VERSION);
	LOG_INFO(LOGCAT_SYSTEM, "Marmalade Runtime Version %s, Linked Version %s\n", marmaladeRuntimeVersion, MARMALADE_VERSION_STRING);

	const char* s3eRuntimeVersion = s3eDeviceGetString(S3E_DEVICE_S3E_VERSION);
	LOG_INFO(LOGCAT_SYSTEM, "S3E Runtime Version: %s, Linked Version: %s\n", s3eRuntimeVersion, S3E_VERSION_STRING);

	const char* deviceID = s3eDeviceGetString(S3E_DEVICE_ID);
	const char* deviceOS = s3eDeviceGetString(S3E_DEVICE_OS);
	int32 deviceOsVersion = s3eDeviceGetInt(S3E_DEVICE_OS_VERSION);
	int osVersionMajor = (deviceOsVersion >> 16);
	int osVersionMinor =  deviceOsVersion & 0xffff;
	const char* deviceClass = s3eDeviceGetString(S3E_DEVICE_CLASS);
	const char* deviceArch = s3eDeviceGetString(S3E_DEVICE_ARCHITECTURE);
	const char* deviceChipset = s3eDeviceGetString(S3E_DEVICE_CHIPSET);
	int32 deviceTotalMemKB = s3eDeviceGetInt(S3E_DEVICE_MEM_TOTAL);
	int32 deviceFreeMemKB = s3eDeviceGetInt(S3E_DEVICE_MEM_FREE);
	int32 heapSize = s3eMemoryGetInt(S3E_MEMORY_SIZE);

	LOG_INFO(LOGCAT_SYSTEM, "Device ID: %s\n", deviceID);
	LOG_INFO(LOGCAT_SYSTEM, "Device OS: %s (%d.%d)\n", deviceOS, osVersionMajor, osVersionMinor);
	LOG_INFO(LOGCAT_SYSTEM, "Device Class: %s\n", deviceClass);
	LOG_INFO(LOGCAT_SYSTEM, "Device Architecture: %s\n", deviceArch);
	LOG_INFO(LOGCAT_SYSTEM, "Device Chipset: %s\n", deviceChipset);
	LOG_INFO(LOGCAT_SYSTEM, "Device Memory: %dKB free, %dKB total\n", deviceFreeMemKB, deviceTotalMemKB);
	LOG_INFO(LOGCAT_SYSTEM, "S3E Memory Heap Size: %d bytes\n", heapSize);

	BOOL keyboardHasAlpha = FALSE;
	BOOL keyboardHasDirection = FALSE;
	if (s3eKeyboardGetInt(S3E_KEYBOARD_HAS_ALPHA))
	{
		keyboardHasAlpha = TRUE;
		LOG_INFO(LOGCAT_SYSTEM, "Keyboard property: S3E_KEYBOARD_HAS_ALPHA\n");
	}
	if (s3eKeyboardGetInt(S3E_KEYBOARD_HAS_NUMPAD))
		LOG_INFO(LOGCAT_SYSTEM, "Keyboard property: S3E_KEYBOARD_HAS_NUMPAD\n");
	if (s3eKeyboardGetInt(S3E_KEYBOARD_HAS_DIRECTION))
	{
		keyboardHasDirection = TRUE;
		LOG_INFO(LOGCAT_SYSTEM, "Keyboard property: S3E_KEYBOARD_HAS_DIRECTION\n");
	}

	// Android Xperia Play device detection
	// TODO: any other device ID's we need to worry about?
	BOOL isXperiaPlay = FALSE;
	if (s3eDeviceGetInt(S3E_DEVICE_OS) == S3E_OS_ID_ANDROID)
	{
		LOG_INFO(LOGCAT_SYSTEM, "Detected Android as host OS.\n");
		if (strlen(deviceID) >= 4)
		{
			// possible device ID's I currently know of:
			//   R800i, R800a, R800x
			if (strncmp(deviceID, "R800", 4) == 0)
			{
				LOG_INFO(LOGCAT_SYSTEM, "Device is an Xperia Play.\n");
				isXperiaPlay = TRUE;
			}
			else
				LOG_INFO(LOGCAT_SYSTEM, "Device is not an Xperia Play.\n");
		}
	}

	BOOL keyboardHasPhysicalGameControls = FALSE;
	if ((keyboardHasAlpha && keyboardHasDirection) || isXperiaPlay)
		keyboardHasPhysicalGameControls = TRUE;

	if (keyboardHasPhysicalGameControls)
		LOG_INFO(LOGCAT_SYSTEM, "Keyboard device has enough physical keys for full game controls.\n");
	else
		LOG_INFO(LOGCAT_SYSTEM, "Keyboard device does not have enough physical keys for full game controls.\n");

	m_keyboard = new MarmaladeKeyboard(keyboardHasPhysicalGameControls);
	ASSERT(m_keyboard != NULL);
	LOG_INFO(LOGCAT_SYSTEM, "Keyboard input device ready.\n");

	BOOL isMultitouchAvailable = FALSE;
	if (s3ePointerGetInt(S3E_POINTER_AVAILABLE))
	{
		s3ePointerType pointerType = (s3ePointerType)s3ePointerGetInt(S3E_POINTER_TYPE);
		if (pointerType == S3E_POINTER_TYPE_INVALID)
			LOG_ERROR(LOGCAT_SYSTEM, "Pointer type = S3E_POINTER_TYPE_INVALID\n");
		else if (pointerType == S3E_POINTER_TYPE_MOUSE)
		{
			LOG_INFO(LOGCAT_SYSTEM, "Pointer device is a mouse.\n");

			m_mouse = new MarmaladeMouse();
			ASSERT(m_mouse != NULL);
			LOG_INFO(LOGCAT_SYSTEM, "Mouse input device ready.\n");
		}
		else if (pointerType == S3E_POINTER_TYPE_STYLUS)
		{
			s3eStylusType stylusType = (s3eStylusType)s3ePointerGetInt(S3E_POINTER_STYLUS_TYPE);
			if (stylusType == S3E_STYLUS_TYPE_INVALID)
				LOG_ERROR(LOGCAT_SYSTEM, "Stylus type = S3E_STYLUS_TYPE_INVALID\n");
			else
			{
				if (stylusType == S3E_STYLUS_TYPE_STYLUS)
					LOG_INFO(LOGCAT_SYSTEM, "Pointer device is a touchscreen using a stylus.\n");
				else if (stylusType == S3E_STYLUS_TYPE_FINGER)
					LOG_INFO(LOGCAT_SYSTEM, "Pointer device is a touchscreen.\n");

				if (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE))
				{
					LOG_INFO(LOGCAT_SYSTEM, "Pointer device supports multitouch.\n");
					isMultitouchAvailable = TRUE;
				}
				else
					LOG_INFO(LOGCAT_SYSTEM, "Pointer device does not support multitouch.\n");

				m_touchscreen = new MarmaladeTouchscreen(this, isMultitouchAvailable);
				ASSERT(m_touchscreen != NULL);
				LOG_INFO(LOGCAT_SYSTEM, "Touchscreen input device ready.\n");
			}
		}
	}
	else
		LOG_WARN(LOGCAT_SYSTEM, "No pointer device is available.\n");

	s3eDeviceRegister(S3E_DEVICE_PAUSE, _MarmaladeEventCallback_Pause, this);
	s3eDeviceRegister(S3E_DEVICE_UNPAUSE, _MarmaladeEventCallback_Resume, this);
	s3eDeviceRegister(S3E_DEVICE_EXIT, _MarmaladeEventCallback_Exit, this);
	s3eGLRegister(S3E_GL_SUSPEND, _MarmaladeEventCallback_GLSuspend, this);
	s3eGLRegister(S3E_GL_RESUME, _MarmaladeEventCallback_GLResume, this);
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, _MarmaladeEventCallback_ScreenResize, this);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, _MarmaladeEventCallback_Key, this);
	s3eKeyboardRegister(S3E_KEYBOARD_CHAR_EVENT, _MarmaladeEventCallback_KeyChar, this);
	if (m_mouse != NULL || m_touchscreen != NULL)
	{
		if (isMultitouchAvailable)
		{
			s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, _MarmaladeEventCallback_PointerMultitouchButton, this);
			s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, _MarmaladeEventCallback_PointerMultitouchMotion, this);
		}
		else
		{
			s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, _MarmaladeEventCallback_PointerButton, this);
			s3ePointerRegister(S3E_POINTER_MOTION_EVENT, _MarmaladeEventCallback_PointerMotion, this);
		}
	}
	else
		LOG_WARN(LOGCAT_SYSTEM, "No mouse or touchscreen device was initialized.\n");

	LOG_INFO(LOGCAT_SYSTEM, "Registered S3E event callbacks.\n");

	LOG_INFO(LOGCAT_SYSTEM, "Initializing file system access.\n");
	m_filesystem = new MarmaladeFileSystem();
	ASSERT(m_filesystem != NULL);

	LOG_INFO(LOGCAT_SYSTEM, "Finished initialization.\n");

	return TRUE;
}

BOOL MarmaladeSystem::CreateGameWindow(BaseGameApp *gameApp, GameWindowParams *params)
{
	STACK_TRACE;
	ASSERT(m_window == NULL);

	MarmaladeGameWindow *window = new MarmaladeGameWindow(gameApp, this);
	ASSERT(window != NULL);

	if (!window->Create(params))
	{
		LOG_ERROR(LOGCAT_SYSTEM, "Failed to create a GameWindow.\n");
		return FALSE;
	}

	m_window = window;

	int32 glVersion = s3eGLGetInt(S3E_GL_VERSION);
	int glVersionMajor = (glVersion >> 8) & 0xff;
	int glVersionMinor = glVersion & 0xff;
	int32 eglVersion = s3eGLGetInt(S3E_EGL_VERSION);
	int eglVersionMajor = (eglVersion >> 8) & 0xff;
	int eglVersionMinor = eglVersion & 0xff;
	int32 glMustSuspend = s3eGLGetInt(S3E_GL_MUST_SUSPEND);

	LOG_INFO(LOGCAT_SYSTEM, "GL Version: %d.%d\n", glVersionMajor, glVersionMinor);
	LOG_INFO(LOGCAT_SYSTEM, "EGL Version: %d.%d\n", eglVersionMajor, eglVersionMinor);
	if (glMustSuspend)
		LOG_INFO(LOGCAT_SYSTEM, "Platform may require GL context to be suspended/resumed during execution.\n");
	else
		LOG_INFO(LOGCAT_SYSTEM, "Platform will not require GL context to be suspended/resumed during execution.\n");

	LOG_INFO(LOGCAT_SYSTEM, "OpenGL renderer using the following video hardware:\n");
	LOG_INFO(LOGCAT_SYSTEM, "GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	LOG_INFO(LOGCAT_SYSTEM, "GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	LOG_INFO(LOGCAT_SYSTEM, "GL_VERSION: %s\n", glGetString(GL_VERSION));
	LOG_INFO(LOGCAT_SYSTEM, "GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));

#ifdef GL_ES_VERSION_2_0
	if (glVersionMajor >= 2)
	{
		m_hasShaderSupport = TRUE;

		// TOD: shader version detection? (who cares?)
		m_supportedShaderVersion = 0.0f;

		LOG_INFO(LOGCAT_SYSTEM, "Version of OpenGL supports using shaders.\n");
		LOG_INFO(LOGCAT_SYSTEM, "GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	else
	{
		m_hasShaderSupport = FALSE;
		m_supportedShaderVersion = 0.0f;

		LOG_INFO(LOGCAT_SYSTEM, "Version of OpenGL supports fixed function pipeline only.\n");
	}
#endif

	LOG_INFO(LOGCAT_SYSTEM, "GameWindow instance is ready.\n");

	return TRUE;
}

void MarmaladeSystem::ProcessEvents()
{
	STACK_TRACE;
	// need to manually reset deltas for the Mouse/Touchscreen device objects
	// since we use callbacks to get state updates. as a result of using
	// callbacks, the deltas won't ever get reset to zero when motion of these
	// devices stops (because callbacks are only fired for *motion* events).
	// also need to make sure we do this before the s3eDeviceYield call as
	// that will trigger this ticks callbacks (99% sure anyway, from testing).
	if (m_mouse != NULL)
		m_mouse->ResetDeltas();
	if (m_touchscreen != NULL)
	{
		m_touchscreen->ResetDeltas();
		// also need to re-supply the touchscreen with a new view bounds rect so it can clamp x/y coords
		m_touchscreen->ResetViewBounds(m_window->GetRect());
	}

	s3eDeviceYield(0);
}

int32_t MarmaladeSystem::OnEvent(const MarmaladeSystemEvent *eventArgs)
{
	STACK_TRACE;
	switch (eventArgs->event)
	{
	case MARMALADE_EVENT_PAUSE:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_PAUSE. Passing to GameWindow.\n");
		ASSERT(m_window != NULL);
		m_window->ProcessEvent(eventArgs);
		break;

	case MARMALADE_EVENT_RESUME:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_RESUME. Passing to GameWindow.\n");
		ASSERT(m_window != NULL);
		m_window->ProcessEvent(eventArgs);
		break;

	case MARMALADE_EVENT_EXIT:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_EXIT.\n");
		ASSERT(m_window != NULL);
		m_window->Close();
		break;

	case MARMALADE_EVENT_GLSUSPEND:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_GLSUSPEND. Passing to GameWindow.\n");
		ASSERT(m_window != NULL);
		m_window->ProcessEvent(eventArgs);
		break;

	case MARMALADE_EVENT_GLRESUME:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_GLRESUME. Passing to GameWindow.\n");
		ASSERT(m_window != NULL);
		m_window->ProcessEvent(eventArgs);
		break;

	case MARMALADE_EVENT_SCREENRESIZE:
		LOG_INFO(LOGCAT_SYSTEM, "Event: MARMALADE_EVENT_SCREENRESIZE. Passing to GameWindow.\n");
		ASSERT(m_window != NULL);
		m_window->ProcessEvent(eventArgs);
		break;

	case MARMALADE_EVENT_KEY:
		ASSERT(m_keyboard != NULL);
		m_keyboard->OnKeyEvent((s3eKeyboardEvent*)eventArgs->data);
		break;

	case MARMALADE_EVENT_KEYCHAR:
		ASSERT(m_keyboard != NULL);
		m_keyboard->OnKeyCharEvent((s3eKeyboardCharEvent*)eventArgs->data);
		break;

	case MARMALADE_EVENT_POINTER_BUTTON:
		ASSERT(m_mouse != NULL || m_touchscreen != NULL);
		if (m_mouse != NULL)
			m_mouse->OnButtonEvent((s3ePointerEvent*)eventArgs->data);
		else if (m_touchscreen != NULL)
			m_touchscreen->OnSingleTouchTapEvent((s3ePointerEvent*)eventArgs->data);
		break;

	case MARMALADE_EVENT_POINTER_MOTION:
		ASSERT(m_mouse != NULL || m_touchscreen != NULL);
		if (m_mouse != NULL)
			m_mouse->OnMotionEvent((s3ePointerMotionEvent*)eventArgs->data);
		else if (m_touchscreen != NULL)
			m_touchscreen->OnSingleTouchMotionEvent((s3ePointerMotionEvent*)eventArgs->data);
		break;

	case MARMALADE_EVENT_POINTER_MULTITOUCH_BUTTON:
		ASSERT(m_touchscreen != NULL);
		m_touchscreen->OnMultiTouchTapEvent((s3ePointerTouchEvent*)eventArgs->data);
		break;

	case MARMALADE_EVENT_POINTER_MULTITOUCH_MOTION:
		ASSERT(m_touchscreen != NULL);
		m_touchscreen->OnMultiTouchMotionEvent((s3ePointerTouchMotionEvent*)eventArgs->data);
		break;
	}

	return 0;
}

void MarmaladeSystem::Quit()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_SYSTEM, "Quit requested.\n");
	m_isQuitting = TRUE;

	if (m_window != NULL && !m_window->IsClosing())
	{
		LOG_INFO(LOGCAT_SYSTEM, "Signaling window to close.\n");
		m_window->Close();
	}
}

void MarmaladeSystem::Delay(uint32_t milliseconds) const
{
	STACK_TRACE;
	unsigned int start = GetTicks();
	unsigned int elapsed = 0;

	do
	{
		elapsed = GetTicks() - start;
	}
	while (milliseconds > elapsed);
}

int32 _MarmaladeEventCallback_Pause(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_PAUSE, systemData, userData);
}

int32 _MarmaladeEventCallback_Resume(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_RESUME, systemData, userData);
}

int32 _MarmaladeEventCallback_Exit(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_EXIT, systemData, userData);
}

int32 _MarmaladeEventCallback_GLSuspend(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_GLSUSPEND, systemData, userData);
}

int32 _MarmaladeEventCallback_GLResume(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_GLRESUME, systemData, userData);
}

int32 _MarmaladeEventCallback_ScreenResize(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_SCREENRESIZE, systemData, userData);
}

int32 _MarmaladeEventCallback_Key(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_KEY, systemData, userData);
}

int32 _MarmaladeEventCallback_KeyChar(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_KEYCHAR, systemData, userData);
}

int32 _MarmaladeEventCallback_PointerButton(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_POINTER_BUTTON, systemData, userData);
}

int32 _MarmaladeEventCallback_PointerMotion(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_POINTER_MOTION, systemData, userData);
}

int32 _MarmaladeEventCallback_PointerMultitouchButton(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_POINTER_MULTITOUCH_BUTTON, systemData, userData);
}

int32 _MarmaladeEventCallback_PointerMultitouchMotion(void *systemData, void *userData)
{
	return _MarmaladeEvent_PassToSystem(MARMALADE_EVENT_POINTER_MULTITOUCH_MOTION, systemData, userData);
}

int32_t _MarmaladeEvent_PassToSystem(MARMALADE_EVENT event, void *systemData, void *userData)
{
	STACK_TRACE;
	MarmaladeSystemEvent eventArgs;
	eventArgs.event = event;
	eventArgs.data = systemData;

	MarmaladeSystem *system = (MarmaladeSystem*)userData;
	return system->OnEvent(&eventArgs);
}

#endif
