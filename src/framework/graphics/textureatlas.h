#ifndef __FRAMEWORK_GRAPHICS_TEXTUREATLAS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_TEXTUREATLAS_H_INCLUDED__

#include "../common.h"

#include "../math/rect.h"
#include "../math/rectf.h"

#include <stl/vector.h>

class Texture;

const float TEXCOORD_EDGE_BLEED_OFFSET = 0.02f;

/**
 * Contains information defining a sub-texture or image in a
 * texture atlas.
 */
struct TextureAtlasTile
{
	Rect dimensions;
	RectF texCoords;
};

/**
 * Base class for a sub-image texture manager allowing the use of a
 * single texture object to contain many individual smaller textures
 * to render with.
 */
class TextureAtlas
{
public:
	virtual ~TextureAtlas();

	/**
	 * @return the underlying texture object, or NULL if not set
	 */
	const Texture* GetTexture() const                      { return m_source; }
	
	/**
	 * @return the underlying texture object, or NULL if not set
	 */
	Texture* GetTexture()                                  { return m_source; }

	/**
	 * @return the width in pixels of the underlying texture object
	 */
	uint GetWidth() const                                  { return m_textureWidth; }

	/**
	 * @return the height in pixels of the underlying texture object
	 */
	uint GetHeight() const                                 { return m_textureHeight; }

	/**
	 * @return the number of sub-textures or tiles currently contained in this atlas
	 */
	uint GetNumTextures() const                            { return m_tiles.size(); }

	/**
	 * Gets position and dimension information about a specified
	 * sub-texture or tile.
	 * @param index the index of the sub-texture or tile to retrieve
	 * @return the sub-texture/tile's information
	 */
	const TextureAtlasTile& GetTile(uint index) const      { return m_tiles[index]; }

	/**
	 * Sets the underlying texture object for this atlas.
	 * @param source the texture to use for this atlas
	 */
	void SetTexture(Texture *source);

protected:
	/**
	 * Sets the base texture atlas object.
	 * @param textureWidth the width in pixels of the texture that will 
	 *                     be used for this atlas
	 * @param textureHeight the height in pixels of the texture that
	 *                       will be used for this atlas
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	TextureAtlas(uint textureWidth, uint textureHeight, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	/**
	 * Sets the base texture object.
	 * @param source the underlying texture to use for this atlas
	 * @param texCoordEdgeOffset an offset value that will be used to 
	 *                           help alleviate "texture bleeding" 
	 *                           issues while rendering
	 */
	TextureAtlas(Texture *source, float texCoordEdgeOffset = TEXCOORD_EDGE_BLEED_OFFSET);

	/**
	 * @return an offset value that can be used to help alleviate
	 *         "texture bleeding" when used in conjunction with sub-texture
	 *         UV coordinates
	 */
	float GetTexCoordEdgeOffset() const                    { return m_texCoordEdgeOffset; }

	stl::vector<TextureAtlasTile> m_tiles;

private:
	uint m_textureWidth;
	uint m_textureHeight;
	Texture *m_source;
	float m_texCoordEdgeOffset;
};

#endif

