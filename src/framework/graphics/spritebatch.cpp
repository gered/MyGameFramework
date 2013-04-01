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

const size_t PRINTF_BUFFER_SIZE = 8096;
char __spriteBatch_printfBuffer[PRINTF_BUFFER_SIZE + 1];

SpriteBatch::SpriteBatch(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
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

	m_entities.reserve(1);

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);
	m_renderState->SetDepthTesting(FALSE);

	m_blendState = new BLENDSTATE_ALPHABLEND;
	ASSERT(m_blendState != NULL);

	m_begunRendering = FALSE;

	m_isRenderStateOverridden = FALSE;
	m_isBlendStateOverridden = FALSE;

	m_isClipping = FALSE;
}

SpriteBatch::~SpriteBatch()
{
	STACK_TRACE;
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_blendState);
}

void SpriteBatch::InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == FALSE);

	// keep these around for any 3d -> 2d coordinate projection we may need to do
	// (since we're switching to a 2d orthographic projection mode next)
	m_previousProjection = m_graphicsDevice->GetViewContext()->GetProjectionMatrix();
	m_previousModelview = m_graphicsDevice->GetViewContext()->GetModelViewMatrix();

	if (shader == NULL)
		m_shader = m_graphicsDevice->GetSprite2DShader();
	else
	{
		ASSERT(shader->IsReadyForUse() == TRUE);
		m_shader = shader;
	}

	if (renderState != NULL)
	{
		m_isRenderStateOverridden = TRUE;
		m_overrideRenderState = *renderState;
	}
	else
		m_isRenderStateOverridden = FALSE;

	if (blendState != NULL)
	{
		m_isBlendStateOverridden = TRUE;
		m_overrideBlendState = *blendState;
	}
	else
		m_isBlendStateOverridden = FALSE;

	m_currentSpritePointer = 0;
	m_begunRendering = TRUE;
	m_vertices->MoveToStart();
}

void SpriteBatch::Begin(SpriteShader *shader)
{
	STACK_TRACE;
	InternalBegin(NULL, NULL, shader);
}

void SpriteBatch::Begin(const RenderState &renderState, SpriteShader *shader)
{
	STACK_TRACE;
	InternalBegin(&renderState, NULL, shader);
}

void SpriteBatch::Begin(const BlendState &blendState, SpriteShader *shader)
{
	STACK_TRACE;
	InternalBegin(NULL, &blendState, shader);
}

void SpriteBatch::Begin(const RenderState &renderState, const BlendState &blendState, SpriteShader *shader)
{
	STACK_TRACE;
	InternalBegin(&renderState, &blendState, shader);
}

void SpriteBatch::Render(const Texture *texture, int16_t x, int16_t y, const Color &color)
{
	STACK_TRACE;
	y = FixYCoord(y, texture->GetHeight());
	AddSprite(texture, x, y, x + texture->GetWidth(), y + texture->GetHeight(), 0, 0, texture->GetWidth(), texture->GetHeight(), color);
}

void SpriteBatch::Render(const Texture *texture, int16_t x, int16_t y, uint16_t width, uint16_t height, const Color &color)
{
	STACK_TRACE;
	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, 0, 0, width, height, color);
}

void SpriteBatch::Render(const Texture *texture, int16_t x, int16_t y, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	y = FixYCoord(y, texture->GetHeight());
	AddSprite(texture, x, y, x + texture->GetWidth(), y + texture->GetHeight(), texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, int16_t x, int16_t y, uint16_t width, uint16_t height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= texture->GetWidth() / 2;
	screenCoordinates.y -= texture->GetHeight() / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, uint16_t width, uint16_t height, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, width, height, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= texture->GetWidth() / 2;
	screenCoordinates.y -= texture->GetHeight() / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const Texture *texture, const Vector3 &worldPosition, uint16_t width, uint16_t height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(texture, screenCoordinates.x, screenCoordinates.y, width, height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, int16_t x, int16_t y, const Color &color)
{
	STACK_TRACE;
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Rect *tileSize = &atlas->GetTile(index).dimensions;
	const Texture *texture = atlas->GetTexture();

	y = FixYCoord(y, (uint16_t)tileSize->GetHeight());
	AddSprite(texture, x, y, x + tileSize->GetWidth(), y + tileSize->GetHeight(), texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, int16_t x, int16_t y, uint16_t width, uint16_t height, const Color &color)
{
	STACK_TRACE;
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Texture *texture = atlas->GetTexture();

	y = FixYCoord(y, height);
	AddSprite(texture, x, y, x + width, y + height, texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, const Vector3 &worldPosition, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	const TextureAtlasTile &tile = atlas->GetTile(index);
	screenCoordinates.x -= tile.dimensions.GetWidth() / 2;
	screenCoordinates.y -= tile.dimensions.GetHeight() / 2;

	Render(atlas, index, screenCoordinates.x, screenCoordinates.y, color);
}

void SpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, const Vector3 &worldPosition, uint16_t width, uint16_t height, const Color &color)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	screenCoordinates.x -= width / 2;
	screenCoordinates.y -= height / 2;

	Render(atlas, index, screenCoordinates.x, screenCoordinates.y, width, height, color);
}

void SpriteBatch::Render(const SpriteFont *font, int16_t x, int16_t y, const Color &color, const char *text)
{
	STACK_TRACE;
	size_t textLength = strlen(text);

	y = FixYCoord(y, (uint16_t)font->GetLetterHeight());

	int16_t drawX = x;
	int16_t drawY = y;

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

void SpriteBatch::Render(const SpriteFont *font, int16_t x, int16_t y, const Color &color, float scale, const char *text)
{
	STACK_TRACE;
	size_t textLength = strlen(text);

	float scaledLetterHeight = (float)font->GetLetterHeight() * scale;

	y = (int16_t)FixYCoord(y, scaledLetterHeight);

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
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	uint16_t textWidth = 0;
	uint16_t textHeight = 0;
	font->MeasureString(&textWidth, &textHeight, text);

	screenCoordinates.x -= textWidth / 2;
	screenCoordinates.y -= textHeight / 2;

	Render(font, screenCoordinates.x, screenCoordinates.y, color, text);
}

void SpriteBatch::Render(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *text)
{
	STACK_TRACE;
	Point2 screenCoordinates = m_graphicsDevice->GetViewContext()->GetCamera()->Project(worldPosition, m_previousModelview, m_previousProjection);

	uint16_t textWidth = 0;
	uint16_t textHeight = 0;
	font->MeasureString(&textWidth, &textHeight, scale, text);

	screenCoordinates.x -= textWidth / 2;
	screenCoordinates.y -= textHeight / 2;

	Render(font, screenCoordinates.x, screenCoordinates.y, color, scale, text);
}

void SpriteBatch::Printf(const SpriteFont *font, int16_t x, int16_t y, const Color &color, const char *format, ...)
{
	STACK_TRACE;
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, x, y, color, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, int16_t x, int16_t y, const Color &color, float scale, const char *format, ...)
{
	STACK_TRACE;
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, x, y, color, scale, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, const char *format, ...)
{
	STACK_TRACE;
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, worldPosition, color, __spriteBatch_printfBuffer);
}

void SpriteBatch::Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *format, ...)
{
	STACK_TRACE;
	va_list args;
	va_start(args, format);
	vsnprintf(__spriteBatch_printfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, worldPosition, color, scale, __spriteBatch_printfBuffer);
}

void SpriteBatch::RenderLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const Color &color)
{
	STACK_TRACE;
	y1 = FixYCoord(y1, (uint16_t)1);
	y2 = FixYCoord(y2, (uint16_t)1);

	AddLine(x1, y1, x2, y2, color);
}

void SpriteBatch::RenderBox(int16_t left, int16_t top, int16_t right, int16_t bottom, const Color &color)
{
	STACK_TRACE;
	uint16_t height = bottom - top;
	top = FixYCoord(top, height);
	bottom = top + height;

	AddLine(left, top, left, bottom, color);
	AddLine(left, bottom, right, bottom, color);
	AddLine(right, top, right, bottom, color);
	AddLine(left, top, right, top, color);
}

void SpriteBatch::RenderBox(const Rect &rect, const Color &color)
{
	STACK_TRACE;
	int16_t left = (int16_t)rect.left;
	int16_t top = (int16_t)rect.top;
	int16_t right = (int16_t)rect.right;
	int16_t bottom = (int16_t)rect.bottom;

	uint16_t height = bottom - top;
	top = FixYCoord(top, height);
	bottom = top + height;

	AddLine(left, top, left, bottom, color);
	AddLine(left, bottom, right, bottom, color);
	AddLine(right, top, right, bottom, color);
	AddLine(left, top, right, top, color);
}

void SpriteBatch::RenderFilledBox(int16_t left, int16_t top, int16_t right, int16_t bottom, const Color &color)
{
	STACK_TRACE;
	uint16_t height = bottom - top;
	top = FixYCoord(top, height);
	bottom = top + height;

	AddFilledBox(left, top, right, bottom, color);
}

void SpriteBatch::RenderFilledBox(const Rect &rect, const Color &color)
{
	STACK_TRACE;
	int16_t left = (int16_t)rect.left;
	int16_t top = (int16_t)rect.top;
	int16_t right = (int16_t)rect.right;
	int16_t bottom = (int16_t)rect.bottom;

	uint16_t height = bottom - top;
	top = FixYCoord(top, height);
	bottom = top + height;

	AddFilledBox(left, top, right, bottom, color);
}

void SpriteBatch::AddSprite(const Texture *texture, int16_t destLeft, int16_t destTop, int16_t destRight, int16_t destBottom, uint16_t sourceLeft, uint16_t sourceTop, uint16_t sourceRight, uint16_t sourceBottom, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	uint16_t width = sourceRight - sourceLeft;
	ASSERT(width > 0);
	uint16_t height = sourceBottom - sourceTop;
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

	CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_SPRITE);
	SetSpriteInfo(m_currentSpritePointer, texture, destLeftF, destTopF, destRightF, destBottomF, texLeft, texTop, texRight, texBottom, color);
	++m_currentSpritePointer;
}

void SpriteBatch::AddSprite(const Texture *texture, int16_t destLeft, int16_t destTop, int16_t destRight, int16_t destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	float destLeftF = (float)destLeft;
	float destTopF = (float)destTop;
	float destRightF = (float)destRight;
	float destBottomF = (float)destBottom;

	if (m_isClipping)
	{
		if (!ClipSpriteCoords(destLeftF, destTopF, destRightF, destBottomF, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom))
			return;
	}

	CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_SPRITE);
	SetSpriteInfo(m_currentSpritePointer, texture, destLeftF, destTopF, destRightF, destBottomF, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
	++m_currentSpritePointer;
}

void SpriteBatch::AddSprite(const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	if (m_isClipping)
	{
		if (!ClipSpriteCoords(destLeft, destTop, destRight, destBottom, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom))
			return;
	}

	CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_SPRITE);
	SetSpriteInfo(m_currentSpritePointer, texture, destLeft, destTop, destRight, destBottom, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
	++m_currentSpritePointer;
}

BOOL SpriteBatch::ClipSpriteCoords(float &left, float &top, float &right, float &bottom, float &texCoordLeft, float &texCoordTop, float &texCoordRight, float &texCoordBottom)
{
	STACK_TRACE;

	// check for completely out of bounds scenarios first
	if (left >= m_clipRegion.right)
		return FALSE;
	if (right < m_clipRegion.left)
		return FALSE;
	if (top >= m_clipRegion.bottom)
		return FALSE;
	if (bottom < m_clipRegion.top)
		return FALSE;

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

	return TRUE;
}

void SpriteBatch::SetSpriteInfo(uint32_t spriteIndex, const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	STACK_TRACE;
	uint32_t base = m_vertices->GetCurrentPosition();

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
	m_vertices->Move(6);

	// keep info about this entity we just added vertices for
	SpriteBatchEntity &entity = m_entities[spriteIndex];
	entity.texture = texture;
	entity.type = SPRITEBATCH_ENTITY_SPRITE;
	entity.firstVertex = base;
	entity.lastVertex = base + 5;
}

void SpriteBatch::AddLine(float x1, float y1, float x2, float y2, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	if (m_isClipping)
	{
		if (!ClipLineCoords(x1, y1, x2, y2))
			return;
	}

	CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_LINE);
	SetLineInfo(m_currentSpritePointer, x1, y1, x2, y2, color);
	++m_currentSpritePointer;
}

void SpriteBatch::SetLineInfo(uint32_t spriteIndex, float x1, float y1, float x2, float y2, const Color &color)
{
	STACK_TRACE;
	uint32_t base = m_vertices->GetCurrentPosition();

	m_vertices->SetPosition2(base, x1, y1);
	m_vertices->SetPosition2(base + 1, x2, y2);

	// not used
	m_vertices->SetTexCoord(base, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 1, 0.0f, 0.0f);

	m_vertices->SetColor(base, color);
	m_vertices->SetColor(base + 1, color);

	// move ahead past this sprite's vertices, so we're ready for the next one
	m_vertices->Move(2);

	// keep info about this entity we just added vertices for
	SpriteBatchEntity &entity = m_entities[spriteIndex];
	entity.texture = NULL;
	entity.type = SPRITEBATCH_ENTITY_LINE;
	entity.firstVertex = base;
	entity.lastVertex = base + 1;
}

BOOL SpriteBatch::ClipLineCoords(float &x1, float &y1, float &x2, float &y2)
{
	STACK_TRACE;

	// TODO: implementation

	return TRUE;
}

void SpriteBatch::AddFilledBox(float left, float top, float right, float bottom, const Color &color)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	if (m_isClipping)
	{
		if (!ClipFilledBoxCoords(left, top, right, bottom))
			return;
	}

	CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_FILLEDBOX);
	SetFilledBoxInfo(m_currentSpritePointer, left, top, right, bottom, color);
	++m_currentSpritePointer;
}

void SpriteBatch::SetFilledBoxInfo(uint32_t spriteIndex, float left, float top, float right, float bottom, const Color &color)
{
	STACK_TRACE;
	uint32_t base = m_vertices->GetCurrentPosition();

	m_vertices->SetPosition2(base, left, top);
	m_vertices->SetPosition2(base + 1, right, top);
	m_vertices->SetPosition2(base + 2, right, bottom);
	m_vertices->SetPosition2(base + 3, left, top);
	m_vertices->SetPosition2(base + 4, right, bottom);
	m_vertices->SetPosition2(base + 5, left, bottom);

	// not used
	m_vertices->SetTexCoord(base, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 1, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 2, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 3, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 4, 0.0f, 0.0f);
	m_vertices->SetTexCoord(base + 5, 0.0f, 0.0f);

	m_vertices->SetColor(base, color);
	m_vertices->SetColor(base + 1, color);
	m_vertices->SetColor(base + 2, color);
	m_vertices->SetColor(base + 3, color);
	m_vertices->SetColor(base + 4, color);
	m_vertices->SetColor(base + 5, color);

	// move ahead past this sprite's vertices, so we're ready for the next one
	m_vertices->Move(6);

	// keep info about this entity we just added vertices for
	SpriteBatchEntity &entity = m_entities[spriteIndex];
	entity.texture = NULL;
	entity.type = SPRITEBATCH_ENTITY_FILLEDBOX;
	entity.firstVertex = base;
	entity.lastVertex = base + 5;
}

BOOL SpriteBatch::ClipFilledBoxCoords(float &left, float &top, float &right, float &bottom)
{
	STACK_TRACE;

	// check for completely out of bounds scenarios first
	if (left >= m_clipRegion.right)
		return FALSE;
	if (right < m_clipRegion.left)
		return FALSE;
	if (top >= m_clipRegion.bottom)
		return FALSE;
	if (bottom < m_clipRegion.top)
		return FALSE;

	float clippedLeft = left;
	float clippedTop = top;
	float clippedRight = right;
	float clippedBottom = bottom;

	if (clippedLeft < m_clipRegion.left)
		clippedLeft = m_clipRegion.left;
	if (clippedRight > m_clipRegion.right)
		clippedRight = m_clipRegion.right;
	if (clippedTop < m_clipRegion.top)
		clippedTop = m_clipRegion.top;
	if (clippedBottom > m_clipRegion.bottom)
		clippedBottom = m_clipRegion.bottom;

	left = clippedLeft;
	top = clippedTop;
	right = clippedRight;
	bottom = clippedBottom;

	return TRUE;
}

void SpriteBatch::End()
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);

	// don't do anything if nothing is to be rendered!
	if (m_currentSpritePointer == 0)
	{
		// should do this regardless if we're rendering anything or not
		ClearClipRegion();
		m_begunRendering = FALSE;
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

	// ClearClipRegion expects that m_begunRendering == TRUE, so we need to set that last
	ClearClipRegion();
	m_begunRendering = FALSE;
}

void SpriteBatch::RenderQueue()
{
	STACK_TRACE;
	m_graphicsDevice->BindVertexBuffer(m_vertices);

	const SpriteBatchEntity *firstEntity = &m_entities[0];
	const SpriteBatchEntity *lastEntity = &m_entities[0];

	for (uint32_t i = 0; i < m_currentSpritePointer; ++i)
	{
		if (lastEntity->type != m_entities[i].type)
		{
			// if the next type is different then the last range's type, then
			// we need to render the last range now
			RenderQueueRange(firstEntity, lastEntity);

			// switch to the new range with this new type
			firstEntity = &m_entities[i];
		}
		else
		{
			if (lastEntity->texture != m_entities[i].texture)
			{
				// if the next texture is different then the last range's
				// texture, then we need to render the last range now
				RenderQueueRange(firstEntity, lastEntity);

				// switch to the new range with this new texture
				firstEntity = &m_entities[i];
			}
		}

		lastEntity = &m_entities[i];
	}
	// we'll have one last range to render at this point (the loop would have
	// ended before it was caught by the checks inside the loop)
	RenderQueueRange(firstEntity, lastEntity);

	m_graphicsDevice->UnbindVertexBuffer();
}

void SpriteBatch::RenderQueueRange(const SpriteBatchEntity *firstEntity, const SpriteBatchEntity *lastEntity)
{
	STACK_TRACE;
	uint32_t startVertex = firstEntity->firstVertex;
	uint32_t lastVertex = lastEntity->lastVertex + 1;

	if (lastEntity->texture != NULL)
	{
		m_graphicsDevice->BindTexture(lastEntity->texture);
		m_shader->SetTextureHasAlphaOnly(lastEntity->texture->GetFormat() == TEXTURE_FORMAT_ALPHA);
	}
	else
		// HACK: probably better idea to use a RenderState to turn texturing off
		m_graphicsDevice->UnbindTexture();

	switch (lastEntity->type)
	{
	case SPRITEBATCH_ENTITY_SPRITE:
		m_graphicsDevice->RenderTriangles(startVertex, (lastVertex - startVertex) / 3);
		break;
	case SPRITEBATCH_ENTITY_LINE:
		m_graphicsDevice->RenderLines(startVertex, (lastVertex - startVertex) / 2);
		break;
	case SPRITEBATCH_ENTITY_FILLEDBOX:
		m_graphicsDevice->RenderTriangles(startVertex, (lastVertex - startVertex) / 3);
		break;
	}
}

void SpriteBatch::CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_TYPE type)
{
	STACK_TRACE;
	// HACK: the assumption is made here that this will never expand the buffers
	//       by an amount that could be used to store more then one entity at a
	//       time. This is because we use std::vector::push_back to expand the
	//       entity object storage by only one.

	uint32_t verticesRequired = GetVerticesRequiredFor(type);
	if (m_vertices->GetRemainingSpace() < verticesRequired)
	{
		// need to add more space for a new entity of the specified type to fit
		m_vertices->Extend(verticesRequired - m_vertices->GetRemainingSpace());
		ASSERT(m_vertices->GetRemainingSpace() >= verticesRequired);

		m_entities.push_back(SpriteBatchEntity());
		ASSERT(m_currentSpritePointer < m_entities.size());
	}
}

inline uint32_t SpriteBatch::GetVerticesRequiredFor(SPRITEBATCH_ENTITY_TYPE type)
{
	uint32_t numVerticesRequired = 0;
	switch (type)
	{
	case SPRITEBATCH_ENTITY_SPRITE: numVerticesRequired = 6; break;
	case SPRITEBATCH_ENTITY_LINE: numVerticesRequired = 2; break;
	case SPRITEBATCH_ENTITY_FILLEDBOX: numVerticesRequired = 6; break;
	}

	ASSERT(numVerticesRequired != 0);
	return numVerticesRequired;
}

inline int16_t SpriteBatch::FixYCoord(int16_t y, uint16_t sourceHeight) const
{
	return m_graphicsDevice->GetViewContext()->GetViewportHeight() - y - sourceHeight;
}

inline float SpriteBatch::FixYCoord(int16_t y, float sourceHeight) const
{
	return (float)m_graphicsDevice->GetViewContext()->GetViewportHeight() - (float)y - sourceHeight;
}

void SpriteBatch::SetClipRegion(const Rect &rect)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	m_isClipping = TRUE;

	int32_t fixedTop = ((int32_t)m_graphicsDevice->GetViewContext()->GetViewportHeight() - rect.top - rect.GetHeight());
	int32_t fixedBottom = fixedTop + rect.GetHeight();

	m_clipRegion.left = (float)rect.left;
	m_clipRegion.top = (float)fixedTop;
	m_clipRegion.right = (float)rect.right;
	m_clipRegion.bottom = (float)fixedBottom;
}

void SpriteBatch::SetClipRegion(int16_t left, int16_t top, int16_t right, int16_t bottom)
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	m_isClipping = TRUE;

	int16_t height = bottom - top;
	int16_t fixedTop = (m_graphicsDevice->GetViewContext()->GetViewportHeight() - top - height);
	int16_t fixedBottom = fixedTop + height;

	m_clipRegion.left = (float)left;
	m_clipRegion.top = (float)fixedTop;
	m_clipRegion.right = (float)right;
	m_clipRegion.bottom = (float)fixedBottom;
}

void SpriteBatch::ClearClipRegion()
{
	STACK_TRACE;
	ASSERT(m_begunRendering == TRUE);
	m_isClipping = FALSE;
}

