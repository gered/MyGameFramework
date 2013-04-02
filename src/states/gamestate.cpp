#include "../framework/debug.h"

#include "gamestate.h"
#include "statemanager.h"
#include "../gameapp.h"
#include "../effects/effectmanager.h"
#include "../events/eventlistenerex.h"
#include "../processes/processmanager.h"

GameState::GameState(GameApp *gameApp, StateManager *stateManager)
	: EventListenerEx(gameApp->GetEventManager())
{
	ASSERT(gameApp != NULL);
	ASSERT(stateManager != NULL);
	
	m_gameApp = gameApp;
	m_stateManager = stateManager;
	
	m_effectManager = new EffectManager();
	m_processManager = new ProcessManager(this);

	m_isFinished = FALSE;
	m_returnValue = 0;
	m_hasReturnValue = FALSE;
}

GameState::~GameState()
{
	SAFE_DELETE(m_effectManager);
	SAFE_DELETE(m_processManager);
}

void GameState::OnPush()
{
}

void GameState::OnPop()
{
}

void GameState::OnPause(BOOL dueToOverlay)
{
	m_processManager->OnPause(dueToOverlay);
}

void GameState::OnResume(BOOL fromOverlay)
{
	m_processManager->OnResume(fromOverlay);
}

void GameState::OnAppGainFocus()
{
	m_processManager->OnAppGainFocus();
	m_effectManager->OnAppGainFocus();
}

void GameState::OnAppLostFocus()
{
	m_processManager->OnAppLostFocus();
	m_effectManager->OnAppLostFocus();
}

void GameState::OnAppPause()
{
	m_processManager->OnAppPause();
	m_effectManager->OnAppPause();
}

void GameState::OnAppResume()
{
	m_processManager->OnAppResume();
	m_effectManager->OnAppResume();
}

void GameState::OnLostContext()
{
	m_processManager->OnLostContext();
	m_effectManager->OnLostContext();
}

void GameState::OnNewContext()
{
	m_processManager->OnNewContext();
	m_effectManager->OnNewContext();
}

void GameState::OnRender(RenderContext *renderContext)
{
	// switch it up and do effects before processes here so that processes
	// (which would commonly be used for UI overlay elements) don't get
	// overwritten by local effects (e.g. flashes, etc.)
	m_effectManager->OnRenderLocal(renderContext);
	m_processManager->OnRender(renderContext);
}

void GameState::OnResize()
{
	m_processManager->OnResize();
	m_effectManager->OnResize();
}

void GameState::OnUpdate(float delta)
{
	m_processManager->OnUpdate(delta);
	m_effectManager->OnUpdate(delta);
}

BOOL GameState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	return TRUE;
}

BOOL GameState::Handle(const Event *event)
{
	return FALSE;
}

BOOL GameState::IsTransitioning() const
{
	return m_stateManager->IsTransitioning(this);
}

BOOL GameState::IsTopState() const
{
	return m_stateManager->IsTop(this);
}

void GameState::SetFinished()
{
	m_isFinished = TRUE;
	m_returnValue = 0;
	m_hasReturnValue = FALSE;
}

void GameState::SetFinished(int returnValue)
{
	m_isFinished = TRUE;
	m_returnValue = returnValue;
	m_hasReturnValue = TRUE;
}
