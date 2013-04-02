#include "../framework/debug.h"

#include "gameprocess.h"

#include "processmanager.h"
#include "../gameapp.h"
#include "../states/gamestate.h"

GameProcess::GameProcess(GameState *gameState, ProcessManager *processManager)
	: EventListenerEx(gameState->GetGameApp()->GetEventManager())
{
	ASSERT(gameState != NULL);
	ASSERT(processManager != NULL);
	
	m_gameState = gameState;
	m_processManager = processManager;
	m_finished = FALSE;
}

GameProcess::~GameProcess()
{
}

void GameProcess::OnAdd()
{
}

void GameProcess::OnRemove()
{
}

void GameProcess::OnPause(BOOL dueToOverlay)
{
}

void GameProcess::OnResume(BOOL fromOverlay)
{
}

void GameProcess::OnAppGainFocus()
{
}

void GameProcess::OnAppLostFocus()
{
}

void GameProcess::OnAppPause()
{
}

void GameProcess::OnAppResume()
{
}

void GameProcess::OnLostContext()
{
}

void GameProcess::OnNewContext()
{
}

void GameProcess::OnRender(RenderContext *renderContext)
{
}

void GameProcess::OnResize()
{
}

void GameProcess::OnUpdate(float delta)
{
}

BOOL GameProcess::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	return TRUE;
}

BOOL GameProcess::Handle(const Event *event)
{
	return FALSE;
}

BOOL GameProcess::IsTransitioning() const
{
	return m_processManager->IsTransitioning(this);
}

void GameProcess::SetFinished()
{
	m_finished = TRUE;
}

