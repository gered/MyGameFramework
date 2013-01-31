#ifndef __TILEMAP_TILEMAPRENDERER_H_INCLUDED__
#define __TILEMAP_TILEMAPRENDERER_H_INCLUDED__

#include "../framework/common.h"

#include "chunkrenderer.h"

class GraphicsDevice;
class TileMap;
class Shader;

class TileMapRenderer
{
public:
	TileMapRenderer(GraphicsDevice *graphicsDevice);
	virtual ~TileMapRenderer();

	void Render(const TileMap *tileMap, Shader *shader = NULL);
	void RenderAlpha(const TileMap *tileMap, Shader *shader = NULL);

	uint32_t GetNumVerticesRendered() const                { return m_numVerticesRendered; }
	uint32_t GetNumAlphaVerticesRendered() const           { return m_numAlphaVerticesRendered; }
	uint32_t GetNumChunksRendered() const                  { return m_numChunksRendered; }
	uint32_t GetNumAlphaChunksRendered() const             { return m_numAlphaChunksRendered; }

	uint32_t GetTotalVerticesRendered() const              { return m_numVerticesRendered + m_numAlphaVerticesRendered; }
	uint32_t GetTotalChunksRendered() const                { return m_numChunksRendered + m_numAlphaChunksRendered; }

private:
	GraphicsDevice *m_graphicsDevice;
	ChunkRenderer *m_chunkRenderer;

	uint32_t m_numVerticesRendered;
	uint32_t m_numAlphaVerticesRendered;
	uint32_t m_numChunksRendered;
	uint32_t m_numAlphaChunksRendered;
};

#endif

