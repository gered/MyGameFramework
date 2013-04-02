#include "../framework/debug.h"

#include "gwengamestate.h"
#include "gwengamestateuicontroller.h"

GwenGameState::GwenGameState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	STACK_TRACE;
	m_gwenController = NULL;
}

GwenGameState::~GwenGameState()
{
	STACK_TRACE;
	SAFE_DELETE(m_gwenController);
}

void GwenGameState::OnPush()
{
	STACK_TRACE;
	ASSERT(m_gwenController != NULL);
	m_gwenController->OnPush();
}

void GwenGameState::OnPop()
{
	STACK_TRACE;
	m_gwenController->OnPop();
}

void GwenGameState::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	GameState::OnPause(dueToOverlay);
	m_gwenController->OnPause(dueToOverlay);
}

void GwenGameState::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	GameState::OnResume(fromOverlay);
	m_gwenController->OnResume(fromOverlay);
}

void GwenGameState::OnLostContext()
{
	STACK_TRACE;
	GameState::OnLostContext();
	m_gwenController->OnLostContext();
}

void GwenGameState::OnNewContext()
{
	STACK_TRACE;
	GameState::OnNewContext();
	m_gwenController->OnNewContext();
}

void GwenGameState::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	m_gwenController->OnRender(renderContext);
	GameState::OnRender(renderContext);
}

void GwenGameState::OnResize()
{
	STACK_TRACE;
	GameState::OnResize();
	m_gwenController->OnResize();
}

void GwenGameState::OnUpdate(float delta)
{
	STACK_TRACE;
	GameState::OnUpdate(delta);
	m_gwenController->OnUpdate(delta);
}

BOOL GwenGameState::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return m_gwenController->OnTransition(delta, isTransitioningOut, started);
}

BOOL GwenGameState::Handle(const Event *event)
{
	STACK_TRACE;
	// handle events...
	
	// any events we don't want to handle ourselves should be passed off to
	// GwenGameStateUIController
	return m_gwenController->Handle(event);
}
