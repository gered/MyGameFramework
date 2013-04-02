#include "../framework/debug.h"

#include "gwengamestate.h"
#include "gwengamestateuicontroller.h"

GwenGameState::GwenGameState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	m_gwenController = NULL;
}

GwenGameState::~GwenGameState()
{
	SAFE_DELETE(m_gwenController);
}

void GwenGameState::OnPush()
{
	ASSERT(m_gwenController != NULL);
	m_gwenController->OnPush();
}

void GwenGameState::OnPop()
{
	m_gwenController->OnPop();
}

void GwenGameState::OnPause(bool dueToOverlay)
{
	GameState::OnPause(dueToOverlay);
	m_gwenController->OnPause(dueToOverlay);
}

void GwenGameState::OnResume(bool fromOverlay)
{
	GameState::OnResume(fromOverlay);
	m_gwenController->OnResume(fromOverlay);
}

void GwenGameState::OnLostContext()
{
	GameState::OnLostContext();
	m_gwenController->OnLostContext();
}

void GwenGameState::OnNewContext()
{
	GameState::OnNewContext();
	m_gwenController->OnNewContext();
}

void GwenGameState::OnRender(RenderContext *renderContext)
{
	m_gwenController->OnRender(renderContext);
	GameState::OnRender(renderContext);
}

void GwenGameState::OnResize()
{
	GameState::OnResize();
	m_gwenController->OnResize();
}

void GwenGameState::OnUpdate(float delta)
{
	GameState::OnUpdate(delta);
	m_gwenController->OnUpdate(delta);
}

bool GwenGameState::OnTransition(float delta, bool isTransitioningOut, bool started)
{
	return m_gwenController->OnTransition(delta, isTransitioningOut, started);
}

bool GwenGameState::Handle(const Event *event)
{
	// handle events...
	
	// any events we don't want to handle ourselves should be passed off to
	// GwenGameStateUIController
	return m_gwenController->Handle(event);
}
