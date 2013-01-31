#ifndef __FRAMEWORK_GRAPHICS_CUSTOMTEXTUREATLAS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMTEXTUREATLAS_H_INCLUDED__

#include "../common.h"
#include "textureatlas.h"

class Texture;
struct Rect;

/**
 * Texture atlas manager allowing arbitrary sub-textures to be defined.
 */
class CustomTextureAtlas : public TextureAtlas
{
public:
	/**
	 * Creates a texture atlas object with the specified properties.
	 * @param textureWidth the width in pixels of the texture that will 
	 *                     be used for this atlas
	 * @param textureHeight the height in pixels of the texture that
	 *                       will be used for this atlas
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	CustomTextureAtlas(uint16_t textureWidth, uint16_t textureHeight, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	/**
	 * Creates a texture atlas object with the specified properties.
	 * @param source the underlying texture to use for this atlas
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	CustomTextureAtlas(Texture *source, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	virtual ~CustomTextureAtlas();

	/**
	 * Adds a new definition for a sub-texture / tile.
	 * @param position the pixel coordinates of the sub-texture / tile
	 * @return the index of the new definition
	 */
	uint32_t Add(const Rect &position);

	/**
	 * Adds a new definition for a sub-texture / tile.
	 * @param left left X coordinate of the sub-texture / tile
	 * @param top top Y coordinate of the sub-texture / tile
	 * @param right right X coordinate of the sub-texture / tile
	 * @param bottom bottom Y coordinate of the sub-texture / tile
	 * @return the index of the new definition
	 */
	uint32_t Add(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);

	/**
	 * Clears all sub-texture / tile definitions.
	 */
	void Reset();
};

#endif
