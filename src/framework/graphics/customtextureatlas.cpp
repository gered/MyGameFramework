#include "../debug.h"

#include "customtextureatlas.h"

#include "texture.h"
#include "../math/rect.h"

CustomTextureAtlas::CustomTextureAtlas(uint textureWidth, uint textureHeight, float texCoordEdgeOffset)
	: TextureAtlas(textureWidth, textureHeight, texCoordEdgeOffset)
{
}

CustomTextureAtlas::CustomTextureAtlas(Texture *source, float texCoordEdgeOffset)
	: TextureAtlas(source, texCoordEdgeOffset)
{
}

CustomTextureAtlas::~CustomTextureAtlas()
{
}

uint CustomTextureAtlas::Add(const Rect &position)
{
	return Add(position.left, position.top, position.right, position.bottom);
}

uint CustomTextureAtlas::Add(uint left, uint top, uint right, uint bottom)
{
	ASSERT(right <= GetWidth());
	ASSERT(bottom <= GetHeight());
	ASSERT(left < right);
	ASSERT(top < bottom);

	TextureAtlasTile tile;

	// set pixel location/dimensions
	tile.dimensions.left = left;
	tile.dimensions.top = top;
	tile.dimensions.right = right;
	tile.dimensions.bottom = bottom;

	// set texture coordinates
	// HACK: subtract GetTexCoordEdgeOffset() from the bottom right edges to 
	//       get around floating point rounding errors (adjacent tiles will 
	//       slightly bleed in otherwise)
	tile.texCoords.left = ((float)left + GetTexCoordEdgeOffset()) / (float)GetWidth();
	tile.texCoords.top = ((float)top + GetTexCoordEdgeOffset()) / (float)GetHeight();
	tile.texCoords.right = ((float)right - GetTexCoordEdgeOffset()) / (float)GetWidth();
	tile.texCoords.bottom = ((float)bottom - GetTexCoordEdgeOffset()) / (float)GetHeight();
	
	m_tiles.push_back(tile);

	// index we just added
	return m_tiles.size() - 1;
}

void CustomTextureAtlas::Reset()
{
	m_tiles.clear();
}
