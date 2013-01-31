#include "../debug.h"

#include "textureatlas.h"

#include "texture.h"

TextureAtlas::TextureAtlas(uint16_t textureWidth, uint16_t textureHeight, float texCoordEdgeOffset)
{
	STACK_TRACE;
	ASSERT(textureWidth > 0);
	ASSERT(textureHeight > 0);

	m_source = NULL;
	m_textureWidth = textureWidth;
	m_textureHeight = textureHeight;
	m_texCoordEdgeOffset = texCoordEdgeOffset;
}

TextureAtlas::TextureAtlas(Texture *source, float texCoordEdgeOffset)
{
	STACK_TRACE;
	ASSERT(source != NULL);

	m_source = source;
	m_textureWidth = m_source->GetWidth();
	m_textureHeight = m_source->GetHeight();
	m_texCoordEdgeOffset = texCoordEdgeOffset;
}

TextureAtlas::~TextureAtlas()
{
	STACK_TRACE;
}

void TextureAtlas::SetTexture(Texture *source)
{
	STACK_TRACE;
	if (source == NULL)
		m_source = NULL;
	else
	{
		// must match the existing dimensions set for this atlas
		ASSERT(source->GetWidth() == m_textureWidth);
		ASSERT(source->GetHeight() == m_textureHeight);
		m_source = source;
	}
}
