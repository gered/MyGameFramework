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
	m_finished = false;
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

void GameProcess::OnPause(bool dueToOverlay)
{
}

void GameProcess::OnResume(bool fromOverlay)
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

bool GameProcess::OnTransition(float delta, bool isTransitioningOut, bool started)
{
	return true;
}

bool GameProcess::Handle(const Event *event)
{
	return false;
}

bool GameProcess::IsTransitioning() const
{
	return m_processManager->IsTransitioning(this);
}

void GameProcess::SetFinished()
{
	m_finished = true;
}

