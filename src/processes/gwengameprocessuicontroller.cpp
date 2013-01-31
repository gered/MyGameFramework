#include "../framework/debug.h"

#include "gwengameprocessuicontroller.h"
#include "gwengameprocess.h"
#include "../gameapp.h"
#include "../contexts/rendercontext.h"
#include "../framework/gwen/gwen_inputprocessor.h"
#include "../framework/gwen/gwen_spritebatchrenderer.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/viewcontext.h"
#include <controls/gwen_canvas.h>
#include <skins/gwen_texturedbase.h>

GwenGameProcessUIController::GwenGameProcessUIController(GwenGameProcess *gameProcess)
{
	STACK_TRACE;
	m_gameProcess = gameProcess;
	m_canvas = NULL;
	m_inputProcessor = NULL;
	m_renderer = NULL;
	m_skin = NULL;
	m_alpha = 1.0f;
	m_scale = 1.0f;
}

GwenGameProcessUIController::~GwenGameProcessUIController()
{
	STACK_TRACE;
	SAFE_DELETE(m_inputProcessor);
	SAFE_DELETE(m_canvas);
	SAFE_DELETE(m_skin);
	SAFE_DELETE(m_renderer);
}

void GwenGameProcessUIController::OnAdd()
{
	STACK_TRACE;
}

void GwenGameProcessUIController::OnRemove()
{
	STACK_TRACE;
}

void GwenGameProcessUIController::OnPause(BOOL dueToOverlay)
{
	STACK_TRACE;
	EnableGwenInput(FALSE);
}

void GwenGameProcessUIController::OnResume(BOOL fromOverlay)
{
	STACK_TRACE;
	EnableGwenInput(TRUE);
}

void GwenGameProcessUIController::OnLostContext()
{
	STACK_TRACE;
}

void GwenGameProcessUIController::OnNewContext()
{
	STACK_TRACE;
}

void GwenGameProcessUIController::OnRender(RenderContext *renderContext)
{
	STACK_TRACE;
	ASSERT(m_renderer != NULL);
	ASSERT(m_canvas != NULL);
	m_renderer->PreRender(renderContext->GetSpriteBatch());
	m_canvas->RenderCanvas();
	m_renderer->PostRender();
}

void GwenGameProcessUIController::OnResize()
{
	STACK_TRACE;
	ResizeAndScaleCanvas();
}

void GwenGameProcessUIController::OnUpdate(float delta)
{
	STACK_TRACE;
	ASSERT(m_canvas != NULL);
	m_canvas->DoThink();
}

BOOL GwenGameProcessUIController::OnTransition(float delta, BOOL isTransitioningOut, BOOL started)
{
	STACK_TRACE;
	return TRUE;
}

BOOL GwenGameProcessUIController::Handle(const Event *event)
{
	STACK_TRACE;
	return FALSE;
}

Gwen::Controls::Canvas* GwenGameProcessUIController::InitializeGwen(const stl::string &skinFilename, const stl::string &fontFilename, uint8_t fontSize)
{
	STACK_TRACE;
	ASSERT(m_renderer == NULL);
	ASSERT(m_skin == NULL);
	ASSERT(m_canvas == NULL);
	
	m_renderer = new Gwen::Renderer::SpriteBatchRenderer(GetGameProcess()->GetGameApp()->GetContentManager(), GetGameProcess()->GetGameApp()->GetGraphicsDevice());
	m_skin = new Gwen::Skin::TexturedBase(m_renderer);
	
	m_skin->Init(skinFilename);
	m_skin->SetDefaultFont(fontFilename, (float)fontSize);
	
	m_canvas = new Gwen::Controls::Canvas(m_skin);
	SetAlpha(1.0f);
	m_scale = 1.0f;
	ResizeAndScaleCanvas();
	
	m_inputProcessor = new Gwen::Input::InputProcessor(GetGameProcess()->GetGameApp(), m_canvas);
	
	return m_canvas;
}

void GwenGameProcessUIController::ResizeAndScaleCanvas()
{
	STACK_TRACE;
	if (m_canvas != NULL)
	{
		// make sure that the control is using the most up-to-date scale (which
		// takes into account any recent viewport resizing, etc.)
		SetScale(m_scale);
		
		uint16_t viewportWidth = GetGameProcess()->GetGameApp()->GetGraphicsDevice()->GetViewContext()->GetViewportWidth();
		uint16_t viewportHeight = GetGameProcess()->GetGameApp()->GetGraphicsDevice()->GetViewContext()->GetViewportHeight();
		uint16_t scaledViewportWidth = (float)viewportWidth / m_canvas->Scale();
		uint16_t scaledViewportHeight = (float)viewportHeight / m_canvas->Scale();
		
		m_canvas->SetBounds(0, 0, scaledViewportWidth, scaledViewportHeight);
	}
}

void GwenGameProcessUIController::EnableGwenInput(BOOL enable)
{
	STACK_TRACE;
	if (m_inputProcessor != NULL)
		m_inputProcessor->Enable(enable);
}

void GwenGameProcessUIController::SetAlpha(float alpha)
{
	STACK_TRACE;
	ASSERT(m_renderer != NULL);
	m_alpha = alpha;
	m_renderer->SetAlpha(alpha);
}

void GwenGameProcessUIController::SetScale(float scale)
{
	STACK_TRACE;
	ASSERT(m_canvas != NULL);
	m_scale = scale;
	
	float baseScale = (float)GetGameProcess()->GetGameApp()->GetScreenScale();
	
	m_canvas->SetScale(scale * baseScale);
}

ProcessManager* GwenGameProcessUIController::GetProcessManager() const
{
	return GetGameProcess()->GetProcessManager();
}

GameState* GwenGameProcessUIController::GetGameState() const
{
	return GetGameProcess()->GetGameState();
}

void GwenGameProcessUIController::SetProcessFinished()
{
	GetGameProcess()->SetFinished();
}
