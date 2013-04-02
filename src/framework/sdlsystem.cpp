#ifdef SDL
#include "debug.h"
#include "log.h"

#include "sdlsystem.h"

#include "basegameapp.h"
#include "gamewindow.h"
#include "osevent.h"
#include "sdlgamewindow.h"
#include "sdlsystemevent.h"
#include "file/filesystem.h"
#include "file/sdlfilesystem.h"
#include "graphics/glincludes.h"
#include "input/mouse.h"
#include "input/keyboard.h"
#include "input/sdlmouse.h"
#include "input/sdlkeyboard.h"
#include "sdlincludes.h"

SDLSystem::SDLSystem()
{
	m_isQuitting = false;
	m_hasShaderSupport = false;
	m_window = NULL;
	m_filesystem = NULL;
	m_mouse = NULL;
	m_keyboard = NULL;
}

SDLSystem::~SDLSystem()
{
	LOG_INFO(LOGCAT_SYSTEM, "Releasing.\n");

	if (!m_isQuitting)
	{
		LOG_WARN(LOGCAT_SYSTEM, "Was not signaled to quit. Calling Quit() now.");
		Quit();
	}

	SAFE_DELETE(m_window);

	SAFE_DELETE(m_filesystem);
	SAFE_DELETE(m_mouse);
	SAFE_DELETE(m_keyboard);

	LOG_INFO(LOGCAT_SYSTEM, "Shutting down SDL.\n");
	SDL_Quit();
}

bool SDLSystem::Initialize()
{
	LOG_INFO(LOGCAT_SYSTEM, "SDLSystem initialization starting.\n");

	const SDL_version *SDLversion = SDL_Linked_Version();
	LOG_INFO(LOGCAT_SYSTEM, "SDL Runtime Version: %u.%u.%u, Linked Version: %u.%u.%u\n", SDLversion->major, SDLversion->minor, SDLversion->patch, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_TIMER) == -1)
	{
		LOG_ERROR(LOGCAT_SYSTEM, "SDL_Init() failed: %s\n", SDL_GetError());
		return false;
	}
	LOG_INFO(LOGCAT_SYSTEM, "SDL_Init() passed.\n");

	m_mouse = new SDLMouse(this);
	ASSERT(m_mouse != NULL);
	LOG_INFO(LOGCAT_SYSTEM, "Mouse input device available.\n");

	m_keyboard = new SDLKeyboard(this, true);
	ASSERT(m_keyboard != NULL);
	LOG_INFO(LOGCAT_SYSTEM, "Keyboard input device available.\n");

	int numJoysticks = SDL_NumJoysticks();
	SDL_Joystick *joystick;

	LOG_INFO(LOGCAT_SYSTEM, "%d joystick input devices found\n", numJoysticks);

	for (int i = 0; i < numJoysticks; i++)
	{
		LOG_INFO(LOGCAT_SYSTEM, "Joystick #%d. %s:\n", (i + 1), SDL_JoystickName(i));

		joystick = SDL_JoystickOpen(i);
		if (joystick)
		{
			LOG_INFO(LOGCAT_SYSTEM, "\tAxes: %d\n", SDL_JoystickNumAxes(joystick));
			LOG_INFO(LOGCAT_SYSTEM, "\tBalls: %d\n", SDL_JoystickNumBalls(joystick));
			LOG_INFO(LOGCAT_SYSTEM, "\tHats: %d\n", SDL_JoystickNumHats(joystick));
			LOG_INFO(LOGCAT_SYSTEM, "\tButtons: %d\n", SDL_JoystickNumButtons(joystick));

			SDL_JoystickClose(joystick);
		}
		else
			LOG_WARN(LOGCAT_SYSTEM, "\tMore information could not be obtained.\n");
	}

	LOG_INFO(LOGCAT_SYSTEM, "Initializing file system access.\n");
	m_filesystem = new SDLFileSystem();
	ASSERT(m_filesystem != NULL);

	LOG_INFO(LOGCAT_SYSTEM, "Finished SDLSystem initialization.\n");

	return true;
}

bool SDLSystem::CreateGameWindow(BaseGameApp *gameApp, GameWindowParams *params)
{
	ASSERT(m_window == NULL);

	SDLGameWindow *window = new SDLGameWindow(gameApp);
	ASSERT(window != NULL);

	if (!window->Create(params))
	{
		LOG_ERROR(LOGCAT_SYSTEM, "Failed to create a GameWindow.\n");
		return false;
	}

	m_window = window;

	// now that we have a window (and a GL context), set up OpenGL related stuff

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		LOG_ERROR(LOGCAT_SYSTEM, "GLEW failed to initialize: %s\n", glewGetErrorString(err));
		return false;
	}
	LOG_INFO(LOGCAT_SYSTEM, "GLEW %s initialized. OpenGL extensions loaded.\n", glewGetString(GLEW_VERSION));

	// Determine if shaders are supported by the video card
	if (GLEW_ARB_vertex_shader || GLEW_ARB_vertex_program)
	{
		m_hasShaderSupport = true;

		// Since shaders are supported, try to determine what the exact shader version that
		// is supported is by the presence of the different extensions
		// (Ref: http://ogltotd.blogspot.com/2006/12/checking-for-d3d-shader-version.html )
		m_supportedShaderVersion = 1.1f;
		if (GLEW_NV_texture_shader && GLEW_NV_register_combiners)
			m_supportedShaderVersion = 1.1f;
		if (GLEW_ATI_fragment_shader || GLEW_ATI_text_fragment_shader)
			m_supportedShaderVersion = 1.4f;
		if (GLEW_ARB_fragment_program || GLEW_ARB_fragment_shader)
			m_supportedShaderVersion = 2.0f;
		if (GLEW_NV_vertex_program3)
			m_supportedShaderVersion = 3.0f;
		if (GLEW_NV_gpu_program4 || GLEW_EXT_gpu_shader4)
			m_supportedShaderVersion = 4.0f;
	}
	else
		m_hasShaderSupport = false;

	// display opengl hardware/support information
	// TODO: probably should just look at the GL_VERSION string since that's
	//       what GLEW is actually doing under the hood...
	if (GLEW_VERSION_4_2)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 4.2 support present.\n");
	else if (GLEW_VERSION_4_1)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 4.1 support present.\n");
	else if (GLEW_VERSION_4_0)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 4.0 support present.\n");
	else if (GLEW_VERSION_3_3)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 3.3 support present.\n");
	else if (GLEW_VERSION_3_2)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 3.2 support present.\n");
	else if (GLEW_VERSION_3_1)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 3.1 support present.\n");
	else if (GLEW_VERSION_3_0)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 3.0 support present.\n");
	else if (GLEW_VERSION_2_1)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 2.1 support present.\n");
	else if (GLEW_VERSION_2_0)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 2.0 support present.\n");
	else if (GLEW_VERSION_1_5)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 1.5 support present.\n");
	else if (GLEW_VERSION_1_4)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 1.4 support present.\n");
	else if (GLEW_VERSION_1_3)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 1.3 support present.\n");
	else if (GLEW_VERSION_1_2)
		LOG_INFO(LOGCAT_SYSTEM, "OpenGL 1.2 support present.\n");
	else
		LOG_WARN(LOGCAT_SYSTEM, "Unknown version of OpenGL.\n");

	LOG_INFO(LOGCAT_SYSTEM, "OpenGL renderer using the following video hardware:\n");
	LOG_INFO(LOGCAT_SYSTEM, "GL_VENDOR: %s\n", glGetString(GL_VENDOR));
	LOG_INFO(LOGCAT_SYSTEM, "GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	LOG_INFO(LOGCAT_SYSTEM, "GL_VERSION: %s\n", glGetString(GL_VERSION));
	LOG_INFO(LOGCAT_SYSTEM, "GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));

	if (m_hasShaderSupport)
	{
		LOG_INFO(LOGCAT_SYSTEM, "Video card supports Shader Model %.1f.\n", m_supportedShaderVersion);
		LOG_INFO(LOGCAT_SYSTEM, "GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	else
		LOG_INFO(LOGCAT_SYSTEM, "Video card does not support shaders.\n");

	LOG_INFO(LOGCAT_SYSTEM, "GameWindow instance is ready.\n");

	return true;
}

void SDLSystem::ProcessEvents()
{
	// little bit of housekeeping
	m_mouse->ResetDeltas();

	SDL_Event event;
	SDLSystemEvent evt;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_ACTIVEEVENT:
			LOG_INFO(LOGCAT_SYSTEM, "Event: SDL_ACTIVEEVENT. Passing to GameWindow.\n");
			evt.event = &event;
			m_window->ProcessEvent(&evt);
			break;

		case SDL_VIDEORESIZE:
			LOG_INFO(LOGCAT_SYSTEM, "Event: SDL_VIDEORESIZE. Passing to GameWindow.\n");
			evt.event = &event;
			m_window->ProcessEvent(&evt);
			break;

		case SDL_QUIT:
			LOG_INFO(LOGCAT_SYSTEM, "Event: SDL_QUIT\n");
			Quit();
			break;

		case SDL_KEYDOWN:
			m_keyboard->OnKeyEvent(&event.key);
			break;

		case SDL_KEYUP:
			m_keyboard->OnKeyEvent(&event.key);
			break;

		case SDL_MOUSEMOTION:
			m_mouse->OnMotionEvent(&event.motion);
			break;

		case SDL_MOUSEBUTTONDOWN:
			m_mouse->OnButtonEvent(&event.button);
			break;

		case SDL_MOUSEBUTTONUP:
			m_mouse->OnButtonEvent(&event.button);
			break;
		}
	}
}

void SDLSystem::Quit()
{
	LOG_INFO(LOGCAT_SYSTEM, "Quit requested.\n");
	m_isQuitting = true;

	if (m_window != NULL && !m_window->IsClosing())
	{
		LOG_INFO(LOGCAT_SYSTEM, "Signaling window to close.\n");
		m_window->Close();
	}

}

#endif
