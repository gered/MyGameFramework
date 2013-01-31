#ifndef __TILEMAP_CHUNKRENDERER_H_INCLUDED__
#define __TILEMAP_CHUNKRENDERER_H_INCLUDED__

#include "../framework/common.h"

class BlendState;
class GraphicsDevice;
class RenderState;
class TileChunk;

class ChunkRenderer
{
public:
	ChunkRenderer(GraphicsDevice *graphicsDevice);
	virtual ~ChunkRenderer();

	uint32_t Render(const TileChunk *chunk);
	uint32_t RenderAlpha(const TileChunk *chunk);

private:
	GraphicsDevice *m_graphicsDevice;
	RenderState *m_renderState;
	BlendState *m_defaultBlendState;
	BlendState *m_alphaBlendState;
};

#endif

