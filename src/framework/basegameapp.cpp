#include "debug.h"
#include "log.h"

#include "basegameapp.h"

#include "operatingsystem.h"
#include "gamewindow.h"
#include "content/contentmanager.h"
#include "file/util.h"
#include "graphics/glincludes.h"
#include "graphics/graphicsdevice.h"
#include "input/keyboard.h"
#include "input/mouse.h"
#include "input/touchscreen.h"

const uint DEFAULT_UPDATE_FREQUENCY = 60;
const uint DEFAULT_MAX_FRAMESKIP = 10;

BaseGameApp::BaseGameApp()
{
	m_stop = false;
	m_isPaused = false;
	m_fps = 0;
	m_frameTime = 0.0f;
	m_numRenders = 0;
	m_rendersPerSecond = 0;
	m_numUpdates = 0;
	m_updatesPerSecond = 0;
	m_renderTime = 0;
	m_updateTime = 0;
	m_nextUpdateAt = 0;
	m_isRunningSlowly = false;
	m_isDirty = false;
	m_system = NULL;
	m_window = NULL;
	m_graphics = NULL;
	m_content = NULL;

	SetUpdateFrequency(DEFAULT_UPDATE_FREQUENCY);
	SetMaxFrameSkip(DEFAULT_MAX_FRAMESKIP);
}

BaseGameApp::~BaseGameApp()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Releasing.\n");
	if (m_graphics != NULL)
		OnLostContext();
	
	SAFE_DELETE(m_content);
	SAFE_DELETE(m_graphics);
}

bool BaseGameApp::Start(OperatingSystem *system)
{
	m_system = system;

	LOG_INFO(LOGCAT_GAMEAPP, "Starting.\n");
	if (!OnInit())
	{
		LOG_ERROR(LOGCAT_GAMEAPP, "Initialization failed.\n");
		return false;
	}
	LOG_INFO(LOGCAT_GAMEAPP, "Initialization succeeded.\n");
	OnNewContext();
	OnResize();

	return true;
}

bool BaseGameApp::Initialize(GameWindowParams *windowParams)
{
	LOG_INFO(LOGCAT_GAMEAPP, "Starting initialization.\n");

	if (!m_system->CreateGameWindow(this, windowParams))
		return false;

	LOG_INFO(LOGCAT_GAMEAPP, "Verifying shader support present.\n");
	if (!m_system->HasShaderSupport())
	{
		LOG_ERROR(LOGCAT_GAMEAPP, "No support for shaders detected. Graphics framework requires hardware shader support. Aborting.\n");
		return false;
	}

	m_window = m_system->GetWindow();
	if (m_window == NULL)
	{
		LOG_ERROR(LOGCAT_GAMEAPP, "Not able to get a GameWindow instance.\n");
		return false;
	}

	m_graphics = new GraphicsDevice();
	ASSERT(m_graphics != NULL);
	m_graphics->Initialize(m_window);

	m_content = new ContentManager(this);
	ASSERT(m_content != NULL);

	LOG_INFO(LOGCAT_GAMEAPP, "Initialization finished.\n");

	return true;
}

void BaseGameApp::Loop()
{
	LOG_INFO(LOGCAT_GAMEAPP, "OnLoadGame event.\n");
	m_content->OnLoadGame();
	OnLoadGame();

	LOG_INFO(LOGCAT_GAMEAPP, "Main loop started.\n");

	uint numUpdatesThisFrame;
	uint numLoops = 0;
	uint timeElapsed = 0;

	uint updateTime = 0;
	uint renderTime = 0;

	m_nextUpdateAt = GetTicks();
	uint currentTime = GetTicks();

	while (!m_stop)
	{
		if (m_isPaused)
		{
			// make sure we *always* do our own internal processing all the time
			// that this loop is being executed. this makes sure OS events are
			// being processed.
			// NOTE: the underlying framework may or may not keep this loop running
			// when the app has been paused however.
			InternalUpdate();
		}
		else
		{
			uint newTime = GetTicks();
			uint frameTime = newTime - currentTime;
			currentTime = newTime;
			timeElapsed += frameTime;

			// Every second, recalculate the FPS
			if (timeElapsed >= 1000)
			{
				m_fps = numLoops;
				m_frameTime = 1000.0f / m_fps;

				m_rendersPerSecond = m_numRenders;
				m_updatesPerSecond = m_numUpdates;
				m_renderTime = renderTime;
				m_updateTime = updateTime;

				m_numUpdates = 0;
				m_numRenders = 0;
				renderTime = 0;
				updateTime = 0;

				numLoops = 0;
				timeElapsed = 0;
			}

			// we're "running slowly" if we're more than one update behind
			if (currentTime > m_nextUpdateAt + m_ticksPerUpdate)
				m_isRunningSlowly = true;
			else
				m_isRunningSlowly = false;

			numUpdatesThisFrame = 0;
			while (GetTicks() >= m_nextUpdateAt && numUpdatesThisFrame < m_maxFrameSkip)
			{
				if (numUpdatesThisFrame > 0)
					m_isRunningSlowly = true;

				uint before = GetTicks();
				OnUpdate(m_fixedUpdateInterval);
				updateTime += GetTicks() - before;

				++numUpdatesThisFrame;
				m_nextUpdateAt += m_ticksPerUpdate;

				++m_numUpdates;

				// just updated, so we need to render the new game state
				m_isDirty = true;
			}

			if (m_isDirty && m_window->IsActive() && m_window->HasGLContext())
			{
				uint before = GetTicks();
				OnRender();
				m_window->Flip();
				renderTime += GetTicks() - before;

				++m_numRenders;

				// don't render again until we have something new to show
				m_isDirty = false;
			}

			++numLoops;
		}
	}

	LOG_INFO(LOGCAT_GAMEAPP, "Main loop exiting.\n");
}

void BaseGameApp::Quit()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Quit requested.\n");
	if (!m_system->IsQuitting())
		m_system->Quit();
	m_stop = true;
}

void BaseGameApp::OnAppGainFocus()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Gained focus.\n");
}

void BaseGameApp::OnAppLostFocus()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Lost focus.\n");
}

void BaseGameApp::OnAppPause()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Paused.\n");
	m_isPaused = true;
}

void BaseGameApp::OnAppResume()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Resumed.\n");
	m_isPaused = false;

	// this makes it so that the main loop won't try to "catch up" thinking that
	// it's really far behind with updates since m_nextUpdateAt won't have been
	// updated since before the app was paused.
	m_nextUpdateAt = GetTicks();
}

void BaseGameApp::OnLostContext()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Lost OpenGL context.\n");

	if (m_graphics != NULL)
		m_graphics->OnLostContext();
	if (m_content != NULL)
		m_content->OnLostContext();
}

void BaseGameApp::OnNewContext()
{
	LOG_INFO(LOGCAT_GAMEAPP, "New OpenGL context.\n");
	m_graphics->OnNewContext();
	m_content->OnNewContext();

	// ensure we render an initial view for the new context
	Invalidate();
}

void BaseGameApp::OnRender()
{
	m_graphics->OnRender();
}

void BaseGameApp::OnResize()
{
	LOG_INFO(LOGCAT_GAMEAPP, "Window resize complete.\n");
	m_graphics->OnResize(m_window->GetRect());

	// ensure we render to initially fit the new window size
	Invalidate();
}

void BaseGameApp::InternalUpdate()
{
	m_system->ProcessEvents();

	if (m_window->IsClosing() && !m_stop)
		Quit();
}

void BaseGameApp::OnUpdate(float delta)
{
	InternalUpdate();
}

void BaseGameApp::SetUpdateFrequency(uint targetFrequency)
{
	m_targetUpdatesPerSecond = targetFrequency;
	m_ticksPerUpdate = 1000 / m_targetUpdatesPerSecond;
	m_fixedUpdateInterval = m_ticksPerUpdate / 1000.0f;
}

bool BaseGameApp::IsAppFocused() const
{
	if (m_window != NULL)
		return m_window->IsFocused();
	else
		return false;  // if there's no window, it can't be focused!
}
