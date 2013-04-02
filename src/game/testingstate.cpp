#include "../framework/log.h"
#include "../framework/common.h"

#include "testingstate.h"

#include "debuginfoprocess.h"
#include "../gameapp.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/simplecolorshader.h"
#include "../framework/graphics/viewcontext.h"
#include "../framework/input/keyboard.h"
#include "../framework/math/camera.h"
#include "../framework/math/common.h"
#include "../framework/support/freecamera.h"
#include "../framework/support/grid.h"
#include "../processes/processmanager.h"

TestingState::TestingState(GameApp *gameApp, StateManager *stateManager)
	: GameState(gameApp, stateManager)
{
	m_grid = NULL;
	m_camera = NULL;
}

TestingState::~TestingState()
{
}

void TestingState::OnPush()
{
	m_camera = new FreeCamera(GetGameApp()->GetGraphicsDevice()->GetViewContext(), GetGameApp());
	m_camera->GetPosition().Set(0.0f, 1.0f, 0.0f);
	GetGameApp()->GetGraphicsDevice()->GetViewContext()->SetCamera(m_camera);

	m_grid = new Grid(GetGameApp()->GetGraphicsDevice(), 32, 32);

	GetProcessManager()->Add<DebugInfoProcess>();
}

void TestingState::OnPop()
{
	GetGameApp()->GetGraphicsDevice()->GetViewContext()->SetCamera(NULL);
	
	SAFE_DELETE(m_grid);
	SAFE_DELETE(m_camera);
}

void TestingState::OnPause(bool dueToOverlay)
{
	GameState::OnPause(dueToOverlay);
}

void TestingState::OnResume(bool fromOverlay)
{
	GameState::OnResume(fromOverlay);
}

void TestingState::OnAppGainFocus()
{
	GameState::OnAppGainFocus();
}

void TestingState::OnAppLostFocus()
{
	GameState::OnAppLostFocus();
}

void TestingState::OnLostContext()
{
	GameState::OnLostContext();
}

void TestingState::OnNewContext()
{
	GameState::OnNewContext();
}

void TestingState::OnRender(RenderContext *renderContext)
{
	SimpleColorShader *colorShader = renderContext->GetGraphicsDevice()->GetSimpleColorShader();

	renderContext->GetGraphicsDevice()->Clear(0.25f, 0.5f, 1.0f, 1.0f);	

	renderContext->GetGraphicsDevice()->BindShader(colorShader);
	colorShader->SetModelViewMatrix(renderContext->GetGraphicsDevice()->GetViewContext()->GetModelViewMatrix());
	colorShader->SetProjectionMatrix(renderContext->GetGraphicsDevice()->GetViewContext()->GetProjectionMatrix());
	m_grid->OnRender();
	renderContext->GetGraphicsDevice()->UnbindShader();

	
	GameState::OnRender(renderContext);
}

void TestingState::OnResize()
{
	GameState::OnResize();
}

void TestingState::OnUpdate(float delta)
{
	GameState::OnUpdate(delta);

	if (GetGameApp()->GetKeyboard()->IsPressed(KSYM_ESCAPE))
		SetFinished();
	
	GetGameApp()->GetGraphicsDevice()->GetViewContext()->GetCamera()->OnUpdate(delta);
}

bool TestingState::OnTransition(float delta, bool isTransitioningOut, bool started)
{
	return true;
}
