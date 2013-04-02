#include "../framework/debug.h"

#include "gwengameprocess.h"
#include "gwengameprocessuicontroller.h"

GwenGameProcess::GwenGameProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
	m_gwenController = NULL;
}

GwenGameProcess::~GwenGameProcess()
{
	SAFE_DELETE(m_gwenController);
}

void GwenGameProcess::OnAdd()
{
	ASSERT(m_gwenController != NULL);
	m_gwenController->OnAdd();
}

void GwenGameProcess::OnRemove()
{
	m_gwenController->OnRemove();
}

void GwenGameProcess::OnPause(bool dueToOverlay)
{
	m_gwenController->OnPause(dueToOverlay);
}

void GwenGameProcess::OnResume(bool fromOverlay)
{
	m_gwenController->OnResume(fromOverlay);
}

void GwenGameProcess::OnLostContext()
{
	m_gwenController->OnLostContext();
}

void GwenGameProcess::OnNewContext()
{
	m_gwenController->OnNewContext();
}

void GwenGameProcess::OnRender(RenderContext *renderContext)
{
	m_gwenController->OnRender(renderContext);
}

void GwenGameProcess::OnResize()
{
	m_gwenController->OnResize();
}

void GwenGameProcess::OnUpdate(float delta)
{
	m_gwenController->OnUpdate(delta);
}

bool GwenGameProcess::OnTransition(float delta, bool isTransitioningOut, bool started)
{
	return m_gwenController->OnTransition(delta, isTransitioningOut, started);
}

bool GwenGameProcess::Handle(const Event *event)
{
	// handle events...
	
	// any events we don't want to handle ourselves should be passed off to
	// GwenGameProcessUIController
	return m_gwenController->Handle(event);
}
