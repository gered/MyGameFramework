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
	STACK_TRACE;
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
	STACK_TRACE;
	SAFE_DELETE(m_effectManager);
	SAFE_DELETE(m_processManager);
}

void GameState::OnPush()
{
	STACK_TRACE;
}

void GameState::OnPop()
{
	STACK_TRACE;
}

void GameState::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	m_processManager->OnPause(dueToOverlay);
}

void GameState::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	m_processManager->OnResume(fromOverlay);
}

void GameState::OnAppGainFocus()
{
	STACK_TRACE;
	m_processManager->OnAppGainFocus();
	m_effectManager->OnAppGainFocus();
}

void GameState::OnAppLostFocus()
{
	STACK_TRACE;
	m_processManager->OnAppLostFocus();
	m_effectManager->OnAppLostFocus();
}

void GameState::OnAppPause()
{
	STACK_TRACE;
	m_processManager->OnAppPause();
	m_effectManager->OnAppPause();
}

void GameState::OnAppResume()
{
	STACK_TRACE;
	m_processManager->OnAppResume();
	m_effectManager->OnAppResume();
}

void GameState::OnLostContext()
{
	STACK_TRACE;
	m_processManager->OnLostContext();
	m_effectManager->OnLostContext();
}

void GameState::OnNewContext()
{
	STACK_TRACE;
	m_processManager->OnNewContext();
	m_effectManager->OnNewContext();
}

void GameState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	// switch it up and do effects before processes here so that processes
	// (which would commonly be used for UI overlay elements) don't get
	// overwritten by local effects (e.g. flashes, etc.)
	m_effectManager->OnRenderLocal(renderContext);
	m_processManager->OnRender(renderContext);
}

void GameState::OnResize()
{
	STACK_TRACE;
	m_processManager->OnResize();
	m_effectManager->OnResize();
}

void GameState::OnUpdate(float delta)
{
	STACK_TRACE;
	m_processManager->OnUpdate(delta);
	m_effectManager->OnUpdate(delta);
}

BOOL GameState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return TRUE;
}

BOOL GameState::Handle(const Event *event)
{
	STACK_TRACE;
	return FALSE;
}

BOOL GameState::IsTransitioning() const
{
	STACK_TRACE;
	return m_stateManager->IsTransitioning(this);
}

BOOL GameState::IsTopState() const
{
	STACK_TRACE;
	return m_stateManager->IsTop(this);
}

void GameState::SetFinished()
{
	STACK_TRACE;
	m_isFinished = TRUE;
	m_returnValue = 0;
	m_hasReturnValue = FALSE;
}

void GameState::SetFinished(uint32_t returnValue)
{
	m_isFinished = TRUE;
	m_returnValue = returnValue;
	m_hasReturnValue = TRUE;
}
