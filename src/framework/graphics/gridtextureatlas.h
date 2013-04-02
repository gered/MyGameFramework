#ifndef __FRAMEWORK_GRAPHICS_GRIDTEXTUREATLAS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GRIDTEXTUREATLAS_H_INCLUDED__

#include "../common.h"

#include "textureatlas.h"

class Texture;

/**
 * Texture atlas manager which divides the texture evenly into a grid
 * of similarly sized sub-textures / tiles.
 */
class GridTextureAtlas : public TextureAtlas
{
public:
	/**
	 * Creates a texture atlas object with the specified properties.
	 * @param textureWidth the width in pixels of the texture that will 
	 *                     be used for this atlas
	 * @param textureHeight the height in pixels of the texture that
	 *                       will be used for this atlas
	 * @param tileWidth the width in pixels of each sub-texture / tile
	 * @param tileHeight the height in pixels of each sub-texture / tile
	 * @param tileBorder the number of pixels between each sub-texture / tile
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	GridTextureAtlas(uint textureWidth, uint textureHeight, uint tileWidth, uint tileHeight, uint tileBorder = 0, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	/**
	 * Creates a texture atlas object with the specified properties.
	 * @param source the underlying texture to use for this atlas
	 * @param tileWidth the width in pixels of each sub-texture / tile
	 * @param tileHeight the height in pixels of each sub-texture / tile
	 * @param tileBorder the number of pixels between each sub-texture / tile
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	GridTextureAtlas(Texture *source, uint tileWidth, uint tileHeight, uint tileBorder = 0, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	virtual ~GridTextureAtlas();

	/**
	 * @return width in pixels of each sub-texture / tile
	 */
	uint GetTileWidth() const                              { return m_tileWidth; }

	/**
	 * @return height in pixels of each sub-texture / tile
	 */
	uint GetTileHeight() const                             { return m_tileHeight; }

private:
	void GenerateGrid(uint tileWidth, uint tileHeight, uint tileBorder);

	uint m_tileWidth;
	uint m_tileHeight;
};

#endif
