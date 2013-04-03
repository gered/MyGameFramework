#include "../debug.h"

#include "spritebatch.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "blendstate.h"
#include "graphicsdevice.h"
#include "renderstate.h"
#include "shader.h"
#include "spritefont.h"
#include "spriteshader.h"
#include "sprite2dshader.h"
#include "texture.h"
#include "textureatlas.h"
#include "vertexbuffer.h"
#include "viewcontext.h"
#include "../math/camera.h"
#include "../math/mathhelpers.h"
#include "../math/point2.h"
#include "../math/rect.h"
#include "../math/vector3.h"

const uint DEFAULT_SPRITE_COUNT = 128;
const uint RESIZE_SPRITE_INCREMENT = 16;
const uint VERTICES_PER_SPRITE = 6;

const size_t PRINTF_BUFFER_SIZE = 8192;
char __spriteBatch_printfBuffer[PRINTF_BUFFER_SIZE + 1];

SpriteBatch::SpriteBatch(GraphicsDevice *graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;
	m_shader = NULL;

	// TODO: default size of 1 is best?
	m_currentSpritePointer = 0;
	
	VERTEX_ATTRIBS attribs[] = {
		VERTEX_POS_2D,
		VERTEX_COLOR,
		VERTEX_TEXCOORD
	};

	// size vertices and texture storage to match m_currentSpriteCapacity
	// HACK: we initialize the buffer to have a size of "1" simply because we
	//       can't use a size of 0 with the current VertexBuffer implementation
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(attribs, 3, 1, BUFFEROBJECT_USAGE_STREAM);

	m_textures.reserve(1);

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);
	m_renderState->SetDepthTesting(false);

	m_blendState = new BLENDSTATE_ALPHABLEND;
	ASSERT(m_blendState != NULL);

	m_begunRendering = false;

	m_isRenderStateOverridden = false;
	m_isBlendStateOverridden = false;

	m_isClipping = false;
}

SpriteBatch::~SpriteBatch()
{
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_blendState);
}

void SpriteBatch::InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader)
{
	ASSERT(m_begunRendering == false);

	// keep these around for any 3d -> 2d coordinate projection we may need to do
	// (since we're switching to a 2d orthographic projection mode next)
	m_previousProjection = m_graphicsDevice->GetViewContext()->GetProjectionMatrix();
	m_previousModelview = m_graphicsDevice->GetViewContext()->GetModelViewMatrix();

	if (shader == NULL)
		m_shader = m_graphicsDevice->GetSprite2DShader();
	else
	{
		ASSERT(shader->IsReadyForUse() == true);
		m_shader = shader;
	}

	if (renderState != NULL)
	{
		m_isRenderStateOverridden = true;
		m_overrideRenderState = *renderState;
	}
	else
		m_isRenderStateOverridden = false;

	if (blendState != NULL)
	{
		m_isBlendStateOverridden = true;
		m_overrideBlendState = *blendState;
	}
	else
		m_isBlendStateOverridden = false;

	m_currentSpritePointer = 0;
	m_begunRendering = true;
	m_vertices->MoveToStart();
}

void SpriteBatch::Begin(SpriteShader *shader)
{
	InternalBegin(NULL, NULL, shader);
}

void SpriteBatch::Begin(const RenderState &renderState, SpriteShader *shader)
{
	InternalBegin(&renderState, NULL, shader);
}

void SpriteBatch::Begin(const BlendState &blendState, SpriteShader *shader)
{
	InternalBegin(NULL, &blendState, shader);
}

void SpriteBatch::Begin(const RenderState &renderState, const BlendState &blendState, SpriteShader *shader)
{
	InternalBegin(&renderState, &blendState, shader);
}

void SpriteBatch::Render(const Texture *texture, int x, int y, const Color &color)
{
	y = FixYCoord(y, texture->GetHeight());
	AddSprite(texture, x, y, x + texture->GetWidth(), y + texture->GetHeight(), 0, 0, texture->GetWidth(), texture->GetHeight(), color);
}

void SpriteBatch::Render(const Texture *texture, int x, int y, uint width, uint height, const Color &color)
{
	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, 0, 0, width, height, color);
}

void SpriteBatch::Render(const Texture *texture, int x, int y, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	y = FixYCoord(y, texture->GetHeight());
	AddSprite(texture, x, y, x + texture->GetWidth(), y + texture->GetHeight(), texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, int x, int y, uint width, uint height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= texture->GetWidth() / 2;
	screenCoordinates.y -= texture->GetHeight() / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, uint width, uint height, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, width, height, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= texture->GetWidth() / 2;
	screenCoordinates.y -= texture->GetHeight() / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, uint width, uint height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, width, height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint index, int x, int y, const Color &color)
{
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Rect *tileSize = &atlas->GetTile(index).dimensions;
	const Texture *texture = atlas->GetTexture();

	y = FixYCoord(y, (uint)tileSize->GetHeight());
	AddSprite(texture, x, y, x + tileSize->GetWidth(), y + tileSize->GetHeight(), texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint index, int x, int y, uint width, uint height, const Color &color)
{
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Texture *texture = atlas->GetTexture();

	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint index, const Vector3 &worldPosition, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	const TextureAtlasTile &tile = atlas->GetTile(index);
	screenCoordinates.x -= tile.dimensions.GetWidth() / 2;
	screenCoordinates.y -= tile.dimensions.GetHeight() / 2;

	Render(atlas, index, screenCoordinates.x, screenCoordinates.y, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint index, const Vector3 &worldPosition, uint width, uint height, const Color &color)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(atlas, index, screenCoordinates.x, screenCoordinates.y, width, height, color);
}

void SpriteBatch::Render(const SpriteFont *font, int x, int y, const Color &color, const char *text)
{
	size_t textLength = strlen(text);

	y = FixYCoord(y, (uint)font->GetLetterHeight());

	int drawX = x;
	int drawY = y;

	for (size_t i = 0; i < textLength; ++i)
	{
		char c = text[i];
		if (c == '\n')
		{
			// new line
			drawX = x;
			drawY -= font->GetLetterHeight();
		}
		else
		{
			const TextureAtlasTile &glyph = font->GetGlyph(c);

			AddSprite(
				font->GetTexture(), 
				drawX, drawY, drawX + glyph.dimensions.GetWidth(), drawY + glyph.dimensions.GetHeight(), 
				glyph.texCoords.left, glyph.texCoords.top, glyph.texCoords.right, glyph.texCoords.bottom, 
				color
				);

			drawX += glyph.dimensions.GetWidth();
		}
	}
}

void SpriteBatch::Render(const SpriteFont *font, int x, int y, const Color &color, float scale, const char *text)
{
	size_t textLength = strlen(text);

	float scaledLetterHeight = (float)font->GetLetterHeight() * scale;

	y = (int)FixYCoord(y, scaledLetterHeight);

	float drawX = (float)x;
	float drawY = (float)y;

	for (size_t i = 0; i < textLength; ++i)
	{
		char c = text[i];
		if (c == '\n')
		{
			// new line
			drawX = x;
			drawY -= scaledLetterHeight;
		}
		else
		{
			const TextureAtlasTile &glyph = font->GetGlyph(c);

			float scaledGlyphWidth = (float)glyph.dimensions.GetWidth() * scale;
			float scaledGlyphHeight = (float)glyph.dimensions.GetHeight() * scale;

			AddSprite(
				font->GetTexture(), 
				drawX, drawY, drawX + scaledGlyphWidth, drawY + scaledGlyphHeight, 
				glyph.texCoords.left, glyph.texCoords.top, glyph.texCoords.right, glyph.texCoords.bottom, 
				color
				);

			drawX += scaledGlyphWidth;
		}
	}
}

void SpriteBatch::Render(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, const char *text)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	uint textWidth = 0;
	uint textHeight = 0;
	font->MeasureString(&textWidth, &textHeight, text);

	screenCoordinates.x -= textWidth / 2;
	screenCoordinates.y -= textHeight / 2;

	Render(font, screenCoordinates.x, screenCoordinates.y, color, text);
}

void SpriteBatch::Render(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *text)
{
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	uint textWidth = 0;
	uint textHeight = 0;
	font->MeasureString(&textWidth, &textHeight, scale, text);

	screenCoordinates.x -= textWidth / 2;
	screenCoordinates.y -= textHeight / 2;

	Render(font, screenCoordinates.x, screenCoordinates.y, color, scale, text);
}

void SpriteBatch::Printf(const SpriteFont *font, int x, int y, const Color &color, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, x, y, color, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, int x, int y, const Color &color, float scale, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, x, y, color, scale, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, worldPosition, color, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, worldPosition, color, scale, __spriteBatch_printfBuffer);
}

void SpriteBatch::AddSprite(const Texture *texture, int destLeft, int destTop, int destRight, int destBottom, uint sourceLeft, uint sourceTop, uint sourceRight, uint sourceBottom, const Color &color)
{
	ASSERT(m_begunRendering == true);

	uint width = sourceRight - sourceLeft;
	ASSERT(width > 0);
	uint height = sourceBottom - sourceTop;
	ASSERT(height > 0);
	float texLeft = sourceLeft / (float)width;
	float texTop = sourceTop / (float)height;
	float texRight = sourceRight / (float)width;
	float texBottom = sourceBottom / (float)height;
	float destLeftF = (float)destLeft;
	float destTopF = (float)destTop;
	float destRightF = (float)destRight;
	float destBottomF = (float)destBottom;

	if (m_isClipping)
	{
		if (!ClipSpriteCoords(destLeftF, destTopF, destRightF, destBottomF, texLeft, texTop, texRight, texBottom))
			return;
	}

	if (GetRemainingSpriteSpaces() == 0)
		AddMoreSpriteSpace(RESIZE_SPRITE_INCREMENT);
	
	SetSpriteInfo(m_currentSpritePointer, texture, destLeftF, destTopF, destRightF, destBottomF, texLeft, texTop, texRight, texBottom, color);
	++m_currentSpritePointer;
}

void SpriteBatch::AddSprite(const Texture *texture, int destLeft, int destTop, int destRight, int destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	ASSERT(m_begunRendering == true);

	float destLeftF = (float)destLeft;
	float destTopF = (float)destTop;
	float destRightF = (float)destRight;
	float destBottomF = (float)destBottom;

	if (m_isClipping)
	{
		if (!ClipSpriteCoords(destLeftF, destTopF, destRightF, destBottomF, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom))
			return;
	}

	if (GetRemainingSpriteSpaces() == 0)
		AddMoreSpriteSpace(RESIZE_SPRITE_INCREMENT);
	
	SetSpriteInfo(m_currentSpritePointer, texture, destLeftF, destTopF, destRightF, destBottomF, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
	++m_currentSpritePointer;
}

void SpriteBatch::AddSprite(const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	ASSERT(m_begunRendering == true);

	if (m_isClipping)
	{
		if (!ClipSpriteCoords(destLeft, destTop, destRight, destBottom, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom))
			return;
	}
	
	if (GetRemainingSpriteSpaces() == 0)
		AddMoreSpriteSpace(RESIZE_SPRITE_INCREMENT);

	SetSpriteInfo(m_currentSpritePointer, texture, destLeft, destTop, destRight, destBottom, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
	++m_currentSpritePointer;
}

bool SpriteBatch::ClipSpriteCoords(float &left, float &top, float &right, float &bottom, float &texCoordLeft, float &texCoordTop, float &texCoordRight, float &texCoordBottom)
{
	// check for completely out of bounds scenarios first
	if (left >= m_clipRegion.right)
		return false;
	if (right < m_clipRegion.left)
		return false;
	if (top >= m_clipRegion.bottom)
		return false;
	if (bottom < m_clipRegion.top)
		return false;

	float clippedLeft = left;
	float clippedTop = top;
	float clippedRight = right;
	float clippedBottom = bottom;
	float clippedTexCoordLeft = texCoordLeft;
	float clippedTexCoordTop = texCoordTop;
	float clippedTexCoordRight = texCoordRight;
	float clippedTexCoordBottom = texCoordBottom;

	if (clippedLeft < m_clipRegion.left)
	{
		clippedLeft = m_clipRegion.left;
		float t = InverseLerp(left, right, clippedLeft);
		clippedTexCoordLeft = Lerp(texCoordLeft, texCoordRight, t);
	}
	if (clippedRight > m_clipRegion.right)
	{
		clippedRight = m_clipRegion.right;
		float t = InverseLerp(right, left, clippedRight);
		clippedTexCoordRight = Lerp(texCoordRight, texCoordLeft, t);
	}
	if (clippedTop < m_clipRegion.top)
	{
		clippedTop = m_clipRegion.top;
		float t = InverseLerp(top, bottom, clippedTop);
		clippedTexCoordBottom = Lerp(texCoordBottom, texCoordTop, t);
	}
	if (clippedBottom > m_clipRegion.bottom)
	{
		clippedBottom = m_clipRegion.bottom;
		float t = InverseLerp(bottom, top, clippedBottom);
		clippedTexCoordTop = Lerp(texCoordTop, texCoordBottom, t);
	}

	left = clippedLeft;
	top = clippedTop;
	right = clippedRight;
	bottom = clippedBottom;
	texCoordLeft = clippedTexCoordLeft;
	texCoordTop = clippedTexCoordTop;
	texCoordRight = clippedTexCoordRight;
	texCoordBottom = clippedTexCoordBottom;

	return true;
}

void SpriteBatch::SetSpriteInfo(uint spriteIndex, const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	uint base = m_vertices->GetCurrentPosition();

	m_vertices->SetPosition2(base, destLeft, destTop);
	m_vertices->SetPosition2(base + 1, destRight, destTop);
	m_vertices->SetPosition2(base + 2, destRight, destBottom);
	m_vertices->SetPosition2(base + 3, destLeft, destTop);
	m_vertices->SetPosition2(base + 4, destRight, destBottom);
	m_vertices->SetPosition2(base + 5, destLeft, destBottom);

	m_vertices->SetTexCoord(base, texCoordLeft, texCoordBottom);
	m_vertices->SetTexCoord(base + 1, texCoordRight, texCoordBottom);
	m_vertices->SetTexCoord(base + 2, texCoordRight, texCoordTop);
	m_vertices->SetTexCoord(base + 3, texCoordLeft, texCoordBottom);
	m_vertices->SetTexCoord(base + 4, texCoordRight, texCoordTop);
	m_vertices->SetTexCoord(base + 5, texCoordLeft, texCoordTop);

	m_vertices->SetColor(base, color);
	m_vertices->SetColor(base + 1, color);
	m_vertices->SetColor(base + 2, color);
	m_vertices->SetColor(base + 3, color);
	m_vertices->SetColor(base + 4, color);
	m_vertices->SetColor(base + 5, color);

	// move ahead past this sprite's vertices, so we're ready for the next one
	m_vertices->Move(VERTICES_PER_SPRITE);
	
	m_textures[spriteIndex] = texture;
}

void SpriteBatch::End()
{
	ASSERT(m_begunRendering == true);

	// don't do anything if nothing is to be rendered!
	if (m_currentSpritePointer == 0)
	{
		// should do this regardless if we're rendering anything or not
		ClearClipRegion();
		m_begunRendering = false;
		return;
	}

	if (m_isRenderStateOverridden)
		m_overrideRenderState.Apply();
	else
		m_renderState->Apply();
	if (m_isBlendStateOverridden)
		m_overrideBlendState.Apply();
	else
		m_blendState->Apply();

	m_graphicsDevice->BindShader(m_shader);
	m_shader->SetModelViewMatrix(IDENTITY_MATRIX);
	m_shader->SetProjectionMatrix(m_graphicsDevice->GetViewContext()->GetOrthographicProjectionMatrix());
	RenderQueue();
	m_graphicsDevice->UnbindShader();

	// ClearClipRegion expects that m_begunRendering == true, so we need to set that last
	ClearClipRegion();
	m_begunRendering = false;
}

void SpriteBatch::RenderQueue()
{
	m_graphicsDevice->BindVertexBuffer(m_vertices);

	uint firstSpriteIndex = 0;
	uint lastSpriteIndex = 0;

	for (uint i = 0; i < m_currentSpritePointer; ++i)
	{
		if (m_textures[lastSpriteIndex] != m_textures[i])
		{
			// if the next texture is different then the last range's texture,
			// then we need to render the last range now
			RenderQueueRange(firstSpriteIndex, lastSpriteIndex);

			// switch to the new range with this new texture
			firstSpriteIndex = i;
		}

		lastSpriteIndex = i;
	}
	
	// we'll have one last range to render at this point (the loop would have
	// ended before it was caught by the checks inside the loop)
	RenderQueueRange(firstSpriteIndex, lastSpriteIndex);

	m_graphicsDevice->UnbindVertexBuffer();
}

void SpriteBatch::RenderQueueRange(uint firstSpriteIndex, uint lastSpriteIndex)
{
	uint startVertex = firstSpriteIndex * VERTICES_PER_SPRITE;
	uint lastVertex = (lastSpriteIndex + 1) * VERTICES_PER_SPRITE;  // render up to and including the last sprite
	
	// take the texture from anywhere in this range
	// (doesn't matter where, should all be the same texture)
	const Texture *texture = m_textures[firstSpriteIndex];
	bool hasAlphaOnly = (texture->GetFormat() == TEXTURE_FORMAT_ALPHA ? true : false);
	
	m_graphicsDevice->BindTexture(texture);
	m_shader->SetTextureHasAlphaOnly(hasAlphaOnly);
	m_graphicsDevice->RenderTriangles(startVertex, (lastVertex - startVertex) / 3);
}

uint SpriteBatch::GetRemainingSpriteSpaces() const
{
	uint currentMaxSprites = m_vertices->GetNumElements() / VERTICES_PER_SPRITE;
	return currentMaxSprites - m_currentSpritePointer;
}

void SpriteBatch::AddMoreSpriteSpace(uint numSprites)
{
	uint numVerticesToAdd = numSprites * VERTICES_PER_SPRITE;
	uint newTextureArraySize = m_textures.size() + numSprites;
	
	m_vertices->Extend(numVerticesToAdd);
	m_textures.resize(newTextureArraySize);
}

inline int SpriteBatch::FixYCoord(int y, uint sourceHeight) const
{
	return m_graphicsDevice->GetViewContext()->GetViewportHeight() - y - sourceHeight;
}

inline float SpriteBatch::FixYCoord(int y, float sourceHeight) const
{
	return (float)m_graphicsDevice->GetViewContext()->GetViewportHeight() - (float)y - sourceHeight;
}

void SpriteBatch::SetClipRegion(const Rect &rect)
{
	ASSERT(m_begunRendering == true);
	m_isClipping = true;

	int fixedTop = ((int)m_graphicsDevice->GetViewContext()->GetViewportHeight() - rect.top - rect.GetHeight());
	int fixedBottom = fixedTop + rect.GetHeight();

	m_clipRegion.left = (float)rect.left;
	m_clipRegion.top = (float)fixedTop;
	m_clipRegion.right = (float)rect.right;
	m_clipRegion.bottom = (float)fixedBottom;
}

void SpriteBatch::SetClipRegion(int left, int top, int right, int bottom)
{
	ASSERT(m_begunRendering == true);
	m_isClipping = true;

	int height = bottom - top;
	int fixedTop = (m_graphicsDevice->GetViewContext()->GetViewportHeight() - top - height);
	int fixedBottom = fixedTop + height;

	m_clipRegion.left = (float)left;
	m_clipRegion.top = (float)fixedTop;
	m_clipRegion.right = (float)right;
	m_clipRegion.bottom = (float)fixedBottom;
}

void SpriteBatch::ClearClipRegion()
{
	ASSERT(m_begunRendering == true);
	m_isClipping = false;
}

