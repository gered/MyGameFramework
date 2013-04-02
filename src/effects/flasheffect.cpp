#include "flasheffect.h"
#include "../contexts/rendercontext.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/texture.h"
#include "../framework/graphics/viewcontext.h"

const float DEFAULT_FLASH_SPEED = 16.0f;
const float DEFAULT_MAX_INTENSITY = 1.0f;

FlashEffect::FlashEffect()
{
	m_flashingIn = true;
	m_flashInSpeed = DEFAULT_FLASH_SPEED;
	m_flashOutSpeed = DEFAULT_FLASH_SPEED;
	m_maximumIntensity = DEFAULT_MAX_INTENSITY;
	m_alpha = 0.0f;
	m_color = COLOR_BLACK;
}

FlashEffect::~FlashEffect()
{
}

void FlashEffect::OnRender(RenderContext *renderContext)
{
	uint width = renderContext->GetGraphicsDevice()->GetViewContext()->GetViewportWidth();
	uint height = renderContext->GetGraphicsDevice()->GetViewContext()->GetViewportHeight();
	
	m_color.a = m_alpha;
	renderContext->GetSpriteBatch()->Render(
		renderContext->GetGraphicsDevice()->GetSolidColorTexture(COLOR_WHITE), 
		0, 0, 
		width, height, 
		m_color
		);
}

void FlashEffect::OnUpdate(float delta)
{
	if (m_flashingIn)
	{
		m_alpha += (delta * m_flashInSpeed);
		if (m_alpha >= m_maximumIntensity)
		{
			m_alpha = m_maximumIntensity;
			m_flashingIn = false;
		}
	}
	else
	{
		m_alpha -= (delta * m_flashOutSpeed);
		if (m_alpha < 0.0f)
			m_alpha = 0.0f;
	}

	if (m_alpha == 0.0f && m_flashingIn == false)
		MarkInactive();
}

