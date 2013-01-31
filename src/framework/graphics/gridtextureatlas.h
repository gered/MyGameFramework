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
	GridTextureAtlas(uint16_t textureWidth, uint16_t textureHeight, uint16_t tileWidth, uint16_t tileHeight, uint16_t tileBorder = 0, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

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
	GridTextureAtlas(Texture *source, uint16_t tileWidth, uint16_t tileHeight, uint16_t tileBorder = 0, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	virtual ~GridTextureAtlas();

	/**
	 * @return width in pixels of each sub-texture / tile
	 */
	uint16_t GetTileWidth() const                          { return m_tileWidth; }

	/**
	 * @return height in pixels of each sub-texture / tile
	 */
	uint16_t GetTileHeight() const                         { return m_tileHeight; }

private:
	void GenerateGrid(uint16_t tileWidth, uint16_t tileHeight, uint16_t tileBorder);

	uint16_t m_tileWidth;
	uint16_t m_tileHeight;
};

#endif
