#include "../framework/debug.h"

#include "rendercontext.h"

#include "../framework/assets/animation/keyframemeshrenderer.h"
#include "../framework/assets/animation/skeletalmeshrenderer.h"
#include "../framework/assets/static/staticmeshrenderer.h"
#include "../framework/content/contentmanager.h"
#include "../framework/graphics/billboardspritebatch.h"
#include "../framework/graphics/geometrydebugrenderer.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/renderstate.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/viewcontext.h"
#include "../framework/support/text.h"

RenderContext::RenderContext(GraphicsDevice *graphicsDevice, ContentManager *contentManager)
{
	ASSERT(graphicsDevice != NULL);
	ASSERT(contentManager != NULL);

	m_graphicsDevice = graphicsDevice;
	m_contentManager = contentManager;

	m_spriteBatch = new SpriteBatch(m_graphicsDevice);
	m_billboardSpriteBatch = new BillboardSpriteBatch(m_graphicsDevice);
	m_keyframeMeshRenderer = new KeyframeMeshRenderer();
	m_skeletalMeshRenderer = new SkeletalMeshRenderer();
	m_staticMeshRenderer = new StaticMeshRenderer();

	CalculateScreenScale();
}

RenderContext::~RenderContext()
{
	SAFE_DELETE(m_spriteBatch);
	SAFE_DELETE(m_billboardSpriteBatch);
	SAFE_DELETE(m_keyframeMeshRenderer);
	SAFE_DELETE(m_skeletalMeshRenderer);
	SAFE_DELETE(m_staticMeshRenderer);
}

void RenderContext::OnLoadGame()
{
}

void RenderContext::OnPreRender()
{
	RENDERSTATE_DEFAULT.Apply();
	BLENDSTATE_DEFAULT.Apply();

	m_graphicsDevice->GetDebugRenderer()->Begin();
	m_spriteBatch->Begin();
	m_billboardSpriteBatch->Begin();
}

void RenderContext::OnPostRender()
{
	m_billboardSpriteBatch->End();
	m_spriteBatch->End();
	m_graphicsDevice->GetDebugRenderer()->End();
}

void RenderContext::OnResize()
{
	CalculateScreenScale();
}

void RenderContext::CalculateScreenScale()
{
	uint width = m_graphicsDevice->GetViewContext()->GetViewportWidth();
	uint height = m_graphicsDevice->GetViewContext()->GetViewportHeight();

	// TODO: these values may need adjusting

	m_screenScale = 1;

	if (width < 640 || height < 480)
		return;

	m_screenScale = 2;

	if (width < 960 || height < 720)
		return;

	m_screenScale = 3;

	if (width < 1280 || height < 960)
		return;

	m_screenScale = 4;

	if (width < 1920 || height < 1080)
		return;

	m_screenScale = 5;
}
