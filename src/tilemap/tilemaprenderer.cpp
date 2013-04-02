#include "../framework/debug.h"

#include "tilemaprenderer.h"

#include "tilemap.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/renderstate.h"
#include "../framework/graphics/shader.h"
#include "../framework/graphics/simplecolortextureshader.h"
#include "../framework/graphics/viewcontext.h"
#include "../framework/math/camera.h"
#include "../framework/math/frustum.h"

TileMapRenderer::TileMapRenderer(GraphicsDevice *graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;

	m_chunkRenderer	= new ChunkRenderer(graphicsDevice);

	m_numChunksRendered = 0;
	m_numAlphaChunksRendered = 0;
	m_numVerticesRendered = 0;
	m_numAlphaVerticesRendered = 0;
}

TileMapRenderer::~TileMapRenderer()
{
	SAFE_DELETE(m_chunkRenderer);
}

void TileMapRenderer::Render(const TileMap *tileMap, Shader *shader)
{
	m_numChunksRendered = 0;
	m_numVerticesRendered = 0;

	if (shader == NULL)
	{
		m_graphicsDevice->BindShader(m_graphicsDevice->GetSimpleColorTextureShader());
		m_graphicsDevice->GetSimpleColorTextureShader()->SetModelViewMatrix(m_graphicsDevice->GetViewContext()->GetModelViewMatrix());
		m_graphicsDevice->GetSimpleColorTextureShader()->SetProjectionMatrix(m_graphicsDevice->GetViewContext()->GetProjectionMatrix());
	}
	else
	{
		ASSERT(shader->IsReadyForUse() == TRUE);
		m_graphicsDevice->BindShader(shader);
	}

	for (uint32_t y = 0; y < tileMap->GetHeightInChunks(); ++y)
	{
		for (uint32_t z = 0; z < tileMap->GetDepthInChunks(); ++z)
		{
			for (uint32_t x = 0; x < tileMap->GetWidthInChunks(); ++x)
			{
				TileChunk *chunk = tileMap->GetChunk(x, y, z);
				if (m_graphicsDevice->GetViewContext()->GetCamera()->GetFrustum()->Test(chunk->GetBounds()))
				{
					m_numVerticesRendered += m_chunkRenderer->Render(chunk);
					++m_numChunksRendered;
				}
			}
		}
	}

	m_graphicsDevice->UnbindShader();
}

void TileMapRenderer::RenderAlpha(const TileMap *tileMap, Shader *shader)
{
	m_numAlphaChunksRendered = 0;
	m_numAlphaVerticesRendered = 0;

	if (shader == NULL)
	{
		m_graphicsDevice->BindShader(m_graphicsDevice->GetSimpleColorTextureShader());
		m_graphicsDevice->GetSimpleColorTextureShader()->SetModelViewMatrix(m_graphicsDevice->GetViewContext()->GetModelViewMatrix());
		m_graphicsDevice->GetSimpleColorTextureShader()->SetProjectionMatrix(m_graphicsDevice->GetViewContext()->GetProjectionMatrix());
	}
	else
	{
		ASSERT(shader->IsReadyForUse() == TRUE);
		m_graphicsDevice->BindShader(shader);
	}

	for (uint32_t y = 0; y < tileMap->GetHeightInChunks(); ++y)
	{
		for (uint32_t z = 0; z < tileMap->GetDepthInChunks(); ++z)
		{
			for (uint32_t x = 0; x < tileMap->GetWidthInChunks(); ++x)
			{
				TileChunk *chunk = tileMap->GetChunk(x, y, z);
				if (chunk->IsAlphaEnabled() && m_graphicsDevice->GetViewContext()->GetCamera()->GetFrustum()->Test(chunk->GetBounds()))
				{
					m_numAlphaVerticesRendered += m_chunkRenderer->RenderAlpha(chunk);
					++m_numAlphaChunksRendered;
				}
			}
		}
	}

	m_graphicsDevice->UnbindShader();
}
