#include "dimeffect.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/viewcontext.h"

const float DEFAULT_DIM_ALPHA = 0.5f;
const Color DEFAULT_DIM_COLOR = COLOR_BLACK;

DimEffect::DimEffect()
{
	m_alpha = DEFAULT_DIM_ALPHA;
	m_color = DEFAULT_DIM_COLOR;
}

DimEffect::~DimEffect()
{
}

void DimEffect::OnRender(RenderContext *renderContext)
{
	uint16_t width = renderContext->GetGraphicsDevice()->GetViewContext()->GetViewportWidth();
	uint16_t height = renderContext->GetGraphicsDevice()->GetViewContext()->GetViewportHeight();
	
	m_color.a = m_alpha;
	renderContext->GetSpriteBatch()->Render(
		renderContext->GetGraphicsDevice()->GetSolidColorTexture(COLOR_WHITE), 
		0, 0, 
		width, height, 
		m_color
		);
}

