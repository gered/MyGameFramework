#include "../debug.h"

#include "spritefont.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "graphicsdevice.h"
#include "texture.h"
#include "textureatlas.h"

SpriteFont::SpriteFont()
	: Content()
{
	m_size = 0;
	m_texture = NULL;
	m_glyphs = NULL;
	m_letterHeight = 0;
}

SpriteFont::~SpriteFont()
{
	STACK_TRACE;
	SAFE_DELETE(m_glyphs);
	SAFE_DELETE(m_texture);
}

void SpriteFont::Load(Texture *texture, TextureAtlas *glyphs, uint8_t size)
{
	STACK_TRACE;
	m_texture = texture;
	m_glyphs = glyphs;
	m_size = size;
	m_letterHeight = GetGlyph(' ').dimensions.GetHeight();
}

void SpriteFont::OnLostContext()
{
	STACK_TRACE;
	SAFE_DELETE(m_glyphs);
	m_letterHeight = 0;
	SAFE_DELETE(m_texture);
}

const TextureAtlasTile& SpriteFont::GetGlyph(unsigned char c) const
{
	STACK_TRACE;
	if (c < LOW_GLYPH || c > HIGH_GLYPH)
		return m_glyphs->GetTile(HIGH_GLYPH - LOW_GLYPH);
	else
		return m_glyphs->GetTile((int)c - LOW_GLYPH);
}

void SpriteFont::MeasureString(uint16_t *width, uint16_t *height, const char *format, ...) const
{
	STACK_TRACE;
	ASSERT(width != NULL || height != NULL);
	static char buffer[8096];     // probably way more then adequate

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 8095, format, args);
	va_end(args);

	size_t textLength = strlen(buffer);

	uint16_t currentMaxWidth = 0;
	uint16_t left = 0;
	int numLines = 1;

	for (size_t i = 0; i < textLength; ++i)
	{
		char c = buffer[i];
		if (c == '\n')
		{
			// new line
			left = 0;
			++numLines;
		}
		else
		{
			const TextureAtlasTile &glyph = GetGlyph(c);
			left += glyph.dimensions.GetWidth();
		}

		currentMaxWidth = Max(left, currentMaxWidth);
	}

	if (width != NULL)
		*width = currentMaxWidth;
	if (height != NULL)
		*height = (uint16_t)(numLines * GetLetterHeight());
}

void SpriteFont::MeasureString(uint16_t *width, uint16_t *height, float scale, const char *format, ...) const
{
	STACK_TRACE;
	ASSERT(width != NULL || height != NULL);
	static char buffer[8096];     // probably way more then adequate

	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 8095, format, args);
	va_end(args);

	size_t textLength = strlen(buffer);

	float scaledLetterHeight = (float)GetLetterHeight() * scale;

	float currentMaxWidth = 0;
	float x = 0;
	int numLines = 1;

	for (size_t i = 0; i < textLength; ++i)
	{
		char c = buffer[i];
		if (c == '\n')
		{
			// new line
			x = 0.0f;
			++numLines;
		}
		else
		{
			const TextureAtlasTile &glyph = GetGlyph(c);
			x += ((float)glyph.dimensions.GetWidth() * scale);
		}

		currentMaxWidth = Max(x, currentMaxWidth);
	}

	if (width != NULL)
		*width = (uint16_t)ceil(currentMaxWidth);
	if (height != NULL)
		*height = (uint16_t)(numLines * scaledLetterHeight);
}

