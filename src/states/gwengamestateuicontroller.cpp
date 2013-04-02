#include "../framework/debug.h"

#include "gwengamestateuicontroller.h"
#include "gwengamestate.h"
#include "../gameapp.h"
#include "../contexts/rendercontext.h"
#include "../framework/gwen/gwen_inputprocessor.h"
#include "../framework/gwen/gwen_spritebatchrenderer.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/viewcontext.h"
#include <controls/gwen_canvas.h>
#include <skins/gwen_texturedbase.h>

GwenGameStateUIController::GwenGameStateUIController(GwenGameState *gameState)
{
	ASSERT(gameState != NULL);
	
	m_gameState = gameState;
	m_canvas = NULL;
	m_inputProcessor = NULL;
	m_renderer = NULL;
	m_skin = NULL;
	m_alpha = 1.0f;
	m_scale = 1.0f;
}

GwenGameStateUIController::~GwenGameStateUIController()
{
	SAFE_DELETE(m_inputProcessor);
	SAFE_DELETE(m_canvas);
	SAFE_DELETE(m_skin);
	SAFE_DELETE(m_renderer);
}

void GwenGameStateUIController::OnPush()
{
}

void GwenGameStateUIController::OnPop()
{
}

void GwenGameStateUIController::OnPause(BOOL dueToOverlay)
{
	EnableGwenInput(FALSE);
}

void GwenGameStateUIController::OnResume(BOOL fromOverlay)
{
	EnableGwenInput(TRUE);
}

void GwenGameStateUIController::OnLostContext()
{
}

void GwenGameStateUIController::OnNewContext()
{
}

void GwenGameStateUIController::OnRender(RenderContext *renderContext)
{
	ASSERT(m_renderer != NULL);
	ASSERT(m_canvas != NULL);
	m_renderer->PreRender(renderContext->GetSpriteBatch());
	m_canvas->RenderCanvas();
	m_renderer->PostRender();
}

void GwenGameStateUIController::OnResize()
{
	ResizeAndScaleCanvas();
}

void GwenGameStateUIController::OnUpdate(float delta)
{
	ASSERT(m_canvas != NULL);
	m_canvas->DoThink();
}

BOOL GwenGameStateUIController::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	return TRUE;
}

BOOL GwenGameStateUIController::Handle(const Event *event)
{
	return FALSE;
}

Gwen::Controls::Canvas* GwenGameStateUIController::InitializeGwen(const stl::string &skinFilename, const stl::string &fontFilename, uint fontSize)
{
	ASSERT(m_renderer == NULL);
	ASSERT(m_skin == NULL);
	ASSERT(m_canvas == NULL);
	
	m_renderer = new Gwen::Renderer::SpriteBatchRenderer(GetGameState()->GetGameApp()->GetContentManager(), GetGameState()->GetGameApp()->GetGraphicsDevice());
	m_skin = new Gwen::Skin::TexturedBase(m_renderer);
	
	m_skin->Init(skinFilename);
	m_skin->SetDefaultFont(fontFilename, (float)fontSize);
	
	m_canvas = new Gwen::Controls::Canvas(m_skin);
	SetAlpha(1.0f);
	m_scale = 1.0f;
	ResizeAndScaleCanvas();
	
	m_inputProcessor = new Gwen::Input::InputProcessor(GetGameState()->GetGameApp(), m_canvas);
	
	return m_canvas;
}

void GwenGameStateUIController::ResizeAndScaleCanvas()
{
	if (m_canvas != NULL)
	{
		// make sure that the control is using the most up-to-date scale (which
		// takes into account any recent viewport resizing, etc.)
		SetScale(m_scale);
		
		uint viewportWidth = GetGameState()->GetGameApp()->GetGraphicsDevice()->GetViewContext()->GetViewportWidth();
		uint viewportHeight = GetGameState()->GetGameApp()->GetGraphicsDevice()->GetViewContext()->GetViewportHeight();
		
		uint scaledViewportWidth = (float)viewportWidth / m_canvas->Scale();
		uint scaledViewportHeight = (float)viewportHeight / m_canvas->Scale();
		
		m_canvas->SetBounds(0, 0, scaledViewportWidth, scaledViewportHeight);
	}
}

void GwenGameStateUIController::EnableGwenInput(BOOL enable)
{
	if (m_inputProcessor != NULL)
		m_inputProcessor->Enable(enable);
}

void GwenGameStateUIController::SetAlpha(float alpha)
{
	ASSERT(m_renderer != NULL);
	m_alpha = alpha;
	m_renderer->SetAlpha(alpha);
}

void GwenGameStateUIController::SetScale(float scale)
{
	ASSERT(m_canvas != NULL);
	m_scale = scale;
	
	float baseScale = (float)GetGameState()->GetGameApp()->GetScreenScale();
	
	m_canvas->SetScale(scale * baseScale);
}

StateManager* GwenGameStateUIController::GetStateManager() const
{
	return GetGameState()->GetStateManager();
}

void GwenGameStateUIController::SetStateFinished()
{
	GetGameState()->SetFinished();
}

void GwenGameStateUIController::SetStateFinished(int returnValue)
{
	GetGameState()->SetFinished(returnValue);
}
