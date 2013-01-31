#include "../debug.h"

#include "customtextureatlas.h"

#include "texture.h"
#include "../math/rect.h"

CustomTextureAtlas::CustomTextureAtlas(uint16_t textureWidth, uint16_t textureHeight, float texCoordEdgeOffset)
	: TextureAtlas(textureWidth, textureHeight, texCoordEdgeOffset)
{
	STACK_TRACE;
}

CustomTextureAtlas::CustomTextureAtlas(Texture *source, float texCoordEdgeOffset)
	: TextureAtlas(source, texCoordEdgeOffset)
{
	STACK_TRACE;
}

CustomTextureAtlas::~CustomTextureAtlas()
{
	STACK_TRACE;
}

uint32_t CustomTextureAtlas::Add(const Rect &position)
{
	STACK_TRACE;
	return Add(position.left, position.top, position.right, position.bottom);
}

uint32_t CustomTextureAtlas::Add(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
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
	STACK_TRACE;
	m_tiles.clear();
}
