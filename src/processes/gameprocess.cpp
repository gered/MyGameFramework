#include "../framework/debug.h"

#include "gameprocess.h"

#include "processmanager.h"
#include "../gameapp.h"
#include "../states/gamestate.h"

GameProcess::GameProcess(GameState *gameState, ProcessManager *processManager)
	: EventListenerEx(gameState->GetGameApp()->GetEventManager())
{
	STACK_TRACE;
	ASSERT(gameState != NULL);
	ASSERT(processManager != NULL);
	
	m_gameState = gameState;
	m_processManager = processManager;
	m_finished = FALSE;
}

GameProcess::~GameProcess()
{
	STACK_TRACE;
}

void GameProcess::OnAdd()
{
	STACK_TRACE;
}

void GameProcess::OnRemove()
{
	STACK_TRACE;
}

void GameProcess::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
}

void GameProcess::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
}

void GameProcess::OnAppGainFocus()
{
	STACK_TRACE;
}

void GameProcess::OnAppLostFocus()
{
	STACK_TRACE;
}

void GameProcess::OnAppPause()
{
	STACK_TRACE;
}

void GameProcess::OnAppResume()
{
	STACK_TRACE;
}

void GameProcess::OnLostContext()
{
	STACK_TRACE;
}

void GameProcess::OnNewContext()
{
	STACK_TRACE;
}

void GameProcess::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
}

void GameProcess::OnResize()
{
	STACK_TRACE;
}

void GameProcess::OnUpdate(float delta)
{
	STACK_TRACE;
}

BOOL GameProcess::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return TRUE;
}

BOOL GameProcess::Handle(const Event *event)
{
	STACK_TRACE;
	return FALSE;
}

BOOL GameProcess::IsTransitioning() const
{
	STACK_TRACE;
	return m_processManager->IsTransitioning(this);
}

void GameProcess::SetFinished()
{
	STACK_TRACE;
	m_finished = TRUE;
}

