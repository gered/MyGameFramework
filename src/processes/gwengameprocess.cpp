#include "../framework/debug.h"

#include "gwengameprocess.h"
#include "gwengameprocessuicontroller.h"

GwenGameProcess::GwenGameProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
	STACK_TRACE;
	m_gwenController = NULL;
}

GwenGameProcess::~GwenGameProcess()
{
	STACK_TRACE;
	SAFE_DELETE(m_gwenController);
}

void GwenGameProcess::OnAdd()
{
	STACK_TRACE;
	ASSERT(m_gwenController != NULL);
	m_gwenController->OnAdd();
}

void GwenGameProcess::OnRemove()
{
	STACK_TRACE;
	m_gwenController->OnRemove();
}

void GwenGameProcess::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	m_gwenController->OnPause(dueToOverlay);
}

void GwenGameProcess::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	m_gwenController->OnResume(fromOverlay);
}

void GwenGameProcess::OnLostContext()
{
	STACK_TRACE;
	m_gwenController->OnLostContext();
}

void GwenGameProcess::OnNewContext()
{
	STACK_TRACE;
	m_gwenController->OnNewContext();
}

void GwenGameProcess::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	m_gwenController->OnRender(renderContext);
}

void GwenGameProcess::OnResize()
{
	STACK_TRACE;
	m_gwenController->OnResize();
}

void GwenGameProcess::OnUpdate(float delta)
{
	STACK_TRACE;
	m_gwenController->OnUpdate(delta);
}

BOOL GwenGameProcess::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return m_gwenController->OnTransition(delta, isTransitioningOut, started);
}

BOOL GwenGameProcess::Handle(const Event *event)
{
	STACK_TRACE;
	// handle events...
	
	// any events we don't want to handle ourselves should be passed off to
	// GwenGameProcessUIController
	return m_gwenController->Handle(event);
}
