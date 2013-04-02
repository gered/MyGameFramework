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

	uint GetNumVerticesRendered() const                    { return m_numVerticesRendered; }
	uint GetNumAlphaVerticesRendered() const               { return m_numAlphaVerticesRendered; }
	uint GetNumChunksRendered() const                      { return m_numChunksRendered; }
	uint GetNumAlphaChunksRendered() const                 { return m_numAlphaChunksRendered; }

	uint GetTotalVerticesRendered() const                  { return m_numVerticesRendered + m_numAlphaVerticesRendered; }
	uint GetTotalChunksRendered() const                    { return m_numChunksRendered + m_numAlphaChunksRendered; }

private:
	GraphicsDevice *m_graphicsDevice;
	ChunkRenderer *m_chunkRenderer;

	uint m_numVerticesRendered;
	uint m_numAlphaVerticesRendered;
	uint m_numChunksRendered;
	uint m_numAlphaChunksRendered;
};

#endif

