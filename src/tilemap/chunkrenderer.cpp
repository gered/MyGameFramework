#include "chunkrenderer.h"

#include "../framework/graphics/blendstate.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/renderstate.h"
#include "../framework/graphics/textureatlas.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/matrix4x4.h"
#include "tilechunk.h"
#include "tilemap.h"
#include "tilemeshcollection.h"

ChunkRenderer::ChunkRenderer(GraphicsDevice *graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;

	m_renderState = new RENDERSTATE_DEFAULT;
	m_defaultBlendState = new BLENDSTATE_DEFAULT;
	m_alphaBlendState = new BLENDSTATE_ALPHABLEND;
}

ChunkRenderer::~ChunkRenderer()
{
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_defaultBlendState);
	SAFE_DELETE(m_alphaBlendState);
}

uint ChunkRenderer::Render(const TileChunk *chunk)
{
	const Texture *texture = chunk->GetTileMap()->GetMeshes()->GetTextureAtlas()->GetTexture();

	uint numVertices = chunk->GetNumVertices();

	m_renderState->Apply();
	m_defaultBlendState->Apply();
	m_graphicsDevice->BindTexture(texture);
	m_graphicsDevice->BindVertexBuffer(chunk->GetVertices());
	m_graphicsDevice->RenderTriangles(0, numVertices / 3);
	m_graphicsDevice->UnbindVertexBuffer();

	return numVertices;
}

uint ChunkRenderer::RenderAlpha(const TileChunk *chunk)
{
	uint numVertices = 0;

	if (chunk->IsAlphaEnabled())
	{
		const Texture *texture = chunk->GetTileMap()->GetMeshes()->GetTextureAtlas()->GetTexture();

		numVertices = chunk->GetNumAlphaVertices();

		m_renderState->Apply();
		m_alphaBlendState->Apply();
		m_graphicsDevice->BindTexture(texture);
		m_graphicsDevice->BindVertexBuffer(chunk->GetAlphaVertices());
		m_graphicsDevice->RenderTriangles(0, numVertices / 3);
		m_graphicsDevice->UnbindVertexBuffer();
	}

	return numVertices;
}

