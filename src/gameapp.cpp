#include "framework/debug.h"
#include "framework/log.h"

#include "gameapp.h"

#include "contexts/contentcache.h"
#include "contexts/rendercontext.h"
#include "events/eventmanager.h"
#include "framework/sdlgamewindow.h"
#include "framework/marmaladegamewindow.h"
#include "framework/content/contentmanager.h"
#include "framework/content/imageloader.h"
#include "framework/content/keyframemeshloader.h"
#include "framework/content/skeletalmeshloader.h"
#include "framework/content/spritefontloader.h"
#include "framework/content/staticmeshloader.h"
#include "framework/content/textloader.h"
#include "framework/content/textureloader.h"
#include "framework/graphics/graphicsdevice.h"
#include "framework/graphics/textureparameters.h"
#include "framework/graphics/viewcontext.h"
#include "game/testingstate.h"
#include "states/statemanager.h"

GameApp::GameApp()
	: BaseGameApp()
{
	STACK_TRACE;
	m_contentCache = NULL;
	m_renderContext = NULL;
	m_stateManager = NULL;
	m_eventManager = NULL;
}

GameApp::~GameApp()
{
	STACK_TRACE;
	SAFE_DELETE(m_stateManager);
	SAFE_DELETE(m_eventManager);
	SAFE_DELETE(m_contentCache);
	SAFE_DELETE(m_renderContext);
}

void GameApp::OnAppGainFocus()
{
	STACK_TRACE;
	BaseGameApp::OnAppGainFocus();
	m_stateManager->OnAppGainFocus();
}

void GameApp::OnAppLostFocus()
{
	STACK_TRACE;
	BaseGameApp::OnAppLostFocus();
	m_stateManager->OnAppLostFocus();
}

void GameApp::OnAppPause()
{
	STACK_TRACE;
	BaseGameApp::OnAppPause();
	m_stateManager->OnAppPause();
}

void GameApp::OnAppResume()
{
	STACK_TRACE;
	BaseGameApp::OnAppResume();
	m_stateManager->OnAppResume();
}

BOOL GameApp::OnInit()
{
	STACK_TRACE;

#if defined(DESKTOP) && defined(SDL)
	SDLGameWindowParams windowParams;
	windowParams.title = "Engine Test";
	windowParams.width = 854;
	windowParams.height = 480;
	windowParams.bpp = 0;
	windowParams.windowed = TRUE;
	windowParams.resizable = TRUE;
#elif MOBILE
	GameWindowParams windowParams;
	windowParams.windowed = FALSE;
#else
#error Undefined platform.
#endif

	if (!Initialize(&windowParams))
		return FALSE;
	
	SeedRnd(GetOperatingSystem()->GetTicks());

	GetContentManager()->RegisterLoader(new ImageLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new KeyframeMeshLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new SkeletalMeshLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new SpriteFontLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new StaticMeshLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new TextLoader(GetContentManager()));
	GetContentManager()->RegisterLoader(new TextureLoader(GetContentManager()));

	m_eventManager = new EventManager();
	m_stateManager = new StateManager(this);
	m_renderContext = new RenderContext(GetGraphicsDevice(), GetContentManager());
	m_contentCache = new ContentCache(GetContentManager());

	return TRUE;
}

void GameApp::OnLoadGame()
{
	STACK_TRACE;
	GetGraphicsDevice()->SetTextureParameters(TEXPARAM_PIXELATED);

	m_renderContext->OnLoadGame();
	m_contentCache->OnLoadGame();

	m_stateManager->Push<TestingState>();
}

void GameApp::OnLostContext()
{
	STACK_TRACE;
	BaseGameApp::OnLostContext();
	m_stateManager->OnLostContext();
}

void GameApp::OnNewContext()
{
	STACK_TRACE;
	BaseGameApp::OnNewContext();
	m_stateManager->OnNewContext();
}

void GameApp::OnRender()
{
	STACK_TRACE;
	BaseGameApp::OnRender();
	m_renderContext->OnPreRender();
	m_stateManager->OnRender(m_renderContext);
	m_renderContext->OnPostRender();
}

void GameApp::OnResize()
{
	STACK_TRACE;
	BaseGameApp::OnResize();
	m_renderContext->OnResize();
	m_stateManager->OnResize();
}

void GameApp::OnUpdate(float delta)
{
	STACK_TRACE;
	BaseGameApp::OnUpdate(delta);

	m_eventManager->ProcessQueue();

	m_stateManager->OnUpdate(delta);

	if (m_stateManager->IsEmpty())
	{
		LOG_INFO(LOGCAT_GAMEAPP, "No states running, quitting.\n");
		Quit();
	}
}

uint32_t GameApp::GetScreenScale() const
{
	STACK_TRACE;
	return m_renderContext->GetScreenScale();
}
