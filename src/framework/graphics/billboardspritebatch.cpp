#include "../debug.h"

#include "billboardspritebatch.h"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "blendstate.h"
#include "graphicsdevice.h"
#include "renderstate.h"
#include "shader.h"
#include "spritefont.h"
#include "spriteshader.h"
#include "sprite3dshader.h"
#include "texture.h"
#include "textureatlas.h"
#include "vertexbuffer.h"
#include "viewcontext.h"
#include "../math/camera.h"
#include "../math/matrix4x4.h"
#include "../math/vector3.h"

const uint32_t VERTICES_PER_SPRITE = 6;

const size_t PRINTF_BUFFER_SIZE = 8096;
char __billboardSpriteBatch_PrintfBuffer[PRINTF_BUFFER_SIZE + 1];

BillboardSpriteBatch::BillboardSpriteBatch(GraphicsDevice *graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;
	m_shader = NULL;

	// TODO: default size of 1 is best?
	m_currentSpriteCapacity = 1;
	m_currentSpritePointer = 0;
	
	VERTEX_ATTRIBS attribs[] = {
		VERTEX_POS_3D,
		VERTEX_COLOR,
		VERTEX_TEXCOORD
	};

	// size vertices and texture storage to match m_currentSpriteCapacity
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(attribs, 3, m_currentSpriteCapacity * VERTICES_PER_SPRITE, BUFFEROBJECT_USAGE_STREAM);

	m_textures.resize(m_currentSpriteCapacity);

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);

	m_blendState = new BLENDSTATE_ALPHABLEND;
	ASSERT(m_blendState != NULL);

	m_begunRendering = FALSE;

	m_isRenderStateOverridden = FALSE;
	m_isBlendStateOverridden = FALSE;
}

BillboardSpriteBatch::~BillboardSpriteBatch()
{
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_blendState);
}

void BillboardSpriteBatch::InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader)
{
	ASSERT(m_begunRendering == FALSE);

	m_cameraPosition = m_graphicsDevice->GetViewContext()->GetCamera()->GetPosition();
	m_cameraForward = m_graphicsDevice->GetViewContext()->GetCamera()->GetForward();

	if (shader == NULL)
		m_shader = m_graphicsDevice->GetSprite3DShader();
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
}

void BillboardSpriteBatch::Begin(SpriteShader *shader)
{
	InternalBegin(NULL, NULL, shader);
}

void BillboardSpriteBatch::Begin(const RenderState &renderState, SpriteShader *shader)
{
	InternalBegin(&renderState, NULL, shader);
}

void BillboardSpriteBatch::Begin(const BlendState &blendState, SpriteShader *shader)
{
	InternalBegin(NULL, &blendState, shader);
}

void BillboardSpriteBatch::Begin(const RenderState &renderState, const BlendState &blendState, SpriteShader *shader)
{
	InternalBegin(&renderState, &blendState, shader);
}

void BillboardSpriteBatch::Render(const Texture *texture, float x, float y, float z, float width, float height, BILLBOARDSPRITE_TYPE type, const Color &color)
{
	AddSprite(type, texture, Vector3(x, y, z), width, height, 0, 0, texture->GetWidth(), texture->GetHeight(), color);
}

void BillboardSpriteBatch::Render(const Texture *texture, const Vector3 &position, float width, float height, BILLBOARDSPRITE_TYPE type, const Color &color)
{
	AddSprite(type, texture, position, width, height, 0, 0, texture->GetWidth(), texture->GetHeight(), color);
}

void BillboardSpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, float x, float y, float z, float width, float height, BILLBOARDSPRITE_TYPE type, const Color &color)
{
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Texture *texture = atlas->GetTexture();

	AddSprite(type, texture, Vector3(x, y, z), width, height, texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void BillboardSpriteBatch::Render(const TextureAtlas *atlas, uint32_t index, const Vector3 &position, float width, float height, BILLBOARDSPRITE_TYPE type, const Color &color)
{
	const RectF *texCoords = &atlas->GetTile(index).texCoords;
	const Texture *texture = atlas->GetTexture();

	AddSprite(type, texture, position, width, height, texCoords->left, texCoords->top, texCoords->right, texCoords->bottom, color);
}

void BillboardSpriteBatch::Render(const SpriteFont *font, float x, float y, float z, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *text)
{
	Render(font, Vector3(x, y, z), type, color, pixelScale, text);
}

void BillboardSpriteBatch::Render(const SpriteFont *font, const Vector3 &position, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *text)
{
	size_t textLength = strlen(text);

	uint16_t textWidth = 0;
	uint16_t textHeight = 0;
	font->MeasureString(&textWidth, &textHeight, text);

	// the x,y,z coordinate specified is used as the position to center the
	// text billboard around. we start drawing the text at the top-left of this
	float startX = -(float)((textWidth / 2) * pixelScale);
	float startY = -(float)((textHeight / 2) * pixelScale);

	float drawX = startX;
	float drawY = startY;
	float lineHeight = (float)(font->GetLetterHeight() * pixelScale);

	Matrix4x4 transform = GetTransformFor(type, position);

	for (size_t i = 0; i < textLength; ++i)
	{
		char c = text[i];
		if (c == '\n')
		{
			// new line
			drawX = startX;
			drawY += lineHeight;
		}
		else
		{
			const TextureAtlasTile &glyph = font->GetGlyph(c);
			float glyphWidth = (float)(glyph.dimensions.GetWidth() * pixelScale);
			float glyphHeight = (float)(glyph.dimensions.GetHeight() * pixelScale);

			AddSprite(
				type,
				transform,
				font->GetTexture(), 
				Vector3(-drawX, -drawY, 0.0f),
				glyphWidth, glyphHeight,
				glyph.texCoords.left, glyph.texCoords.top, glyph.texCoords.right, glyph.texCoords.bottom, 
				color
				);

			drawX += glyphWidth;
		}
	}
}

void BillboardSpriteBatch::Printf(const SpriteFont *font, float x, float y, float z, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__billboardSpriteBatch_PrintfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, Vector3(x, y, z), type, color, pixelScale, __billboardSpriteBatch_PrintfBuffer);
}

void BillboardSpriteBatch::Printf(const SpriteFont *font, const Vector3 &position, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(__billboardSpriteBatch_PrintfBuffer, PRINTF_BUFFER_SIZE, format, args);
	va_end(args);

	Render(font, position, type, color, pixelScale, __billboardSpriteBatch_PrintfBuffer);
}

void BillboardSpriteBatch::AddSprite(BILLBOARDSPRITE_TYPE type, const Texture *texture, const Vector3 &position, float width, float height, uint16_t sourceLeft, uint16_t sourceTop, uint16_t sourceRight, uint16_t sourceBottom, const Color &color)
{
	ASSERT(m_begunRendering == TRUE);
	Matrix4x4 transform = GetTransformFor(type, position);

	// zero vector offset as the transform will translate the billboard to
	// the specified position
	AddSprite(type, transform, texture, ZERO_VECTOR, width, height, sourceLeft, sourceTop, sourceRight, sourceBottom, color);
}

void BillboardSpriteBatch::AddSprite(BILLBOARDSPRITE_TYPE type, const Texture *texture, const Vector3 &position, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	ASSERT(m_begunRendering == TRUE);
	Matrix4x4 transform = GetTransformFor(type, position);

	// zero vector offset as the transform will translate the billboard to
	// the specified position
	AddSprite(type, transform, texture, ZERO_VECTOR, width, height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
}

void BillboardSpriteBatch::AddSprite(BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, uint16_t sourceLeft, uint16_t sourceTop, uint16_t sourceRight, uint16_t sourceBottom, const Color &color)
{
	ASSERT(m_begunRendering == TRUE);

	uint16_t sourceWidth = sourceRight - sourceLeft;
	ASSERT(sourceWidth > 0);
	uint16_t sourceHeight = sourceBottom - sourceTop;
	ASSERT(sourceHeight > 0);
	float texLeft = sourceLeft / (float)sourceWidth;
	float texTop = sourceTop / (float)sourceHeight;
	float texRight = sourceRight / (float)sourceWidth;
	float texBottom = sourceBottom / (float)sourceHeight;

	CheckForNewSpriteSpace();
	SetSpriteInfo(m_currentSpritePointer, type, transform, texture, offset, width, height, texLeft, texTop, texRight, texBottom, color);
	++m_currentSpritePointer;
}

void BillboardSpriteBatch::AddSprite(BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	ASSERT(m_begunRendering == TRUE);
	CheckForNewSpriteSpace();
	SetSpriteInfo(m_currentSpritePointer, type, transform, texture, offset, width, height, texCoordLeft, texCoordTop, texCoordRight, texCoordBottom, color);
	++m_currentSpritePointer;
}

void BillboardSpriteBatch::SetSpriteInfo(uint32_t spriteIndex, BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color)
{
	uint32_t base = spriteIndex * VERTICES_PER_SPRITE;

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	// TODO: needed to reverse this, and my brain isn't working atm to figure out why
	// leaves me feeling like I'm doing something wrong elsewhere...
	float left = halfWidth;
	float top = -halfHeight;
	float right = -halfWidth;
	float bottom = halfHeight;

	// HACK: I don't like this at all. I'm also not sure how this is
	//       performance-wise (haven't tested as I write this). Ideally in the
	//       future this will be done by a custom shader using OpenGL/ES 2...
	Vector3 v1 = (Vector3(left, top, 0.0f) + offset) * transform;
	Vector3 v2 = (Vector3(right, top, 0.0f) + offset) * transform;
	Vector3 v3 = (Vector3(right, bottom, 0.0f) + offset) * transform;
	Vector3 v4 = (Vector3(left, top, 0.0f) + offset) * transform;
	Vector3 v5 = (Vector3(right, bottom, 0.0f) + offset) * transform;
	Vector3 v6 = (Vector3(left, bottom, 0.0f) + offset) * transform;

	m_vertices->SetPosition3(base, v1);
	m_vertices->SetPosition3(base + 1, v2);
	m_vertices->SetPosition3(base + 2, v3);
	m_vertices->SetPosition3(base + 3, v4);
	m_vertices->SetPosition3(base + 4, v5);
	m_vertices->SetPosition3(base + 5, v6);

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

	m_textures[spriteIndex] = texture;
}

void BillboardSpriteBatch::End()
{
	ASSERT(m_begunRendering == TRUE);

	if (m_isRenderStateOverridden)
		m_overrideRenderState.Apply();
	else
		m_renderState->Apply();
	if (m_isBlendStateOverridden)
		m_overrideBlendState.Apply();
	else
		m_blendState->Apply();

	m_graphicsDevice->BindShader(m_shader);
	m_shader->SetModelViewMatrix(m_graphicsDevice->GetViewContext()->GetModelViewMatrix());
	m_shader->SetProjectionMatrix(m_graphicsDevice->GetViewContext()->GetProjectionMatrix());
	RenderQueue();
	m_graphicsDevice->UnbindShader();

	m_begunRendering = FALSE;
}

void BillboardSpriteBatch::RenderQueue()
{
	m_graphicsDevice->BindVertexBuffer(m_vertices);

	const Texture *currentTexture = NULL;
	uint32_t startOffset = 0;
	uint32_t stopOffset = 0;

	for (uint32_t i = 0; i < m_currentSpritePointer; ++i)
	{
		if (currentTexture != m_textures[i])
		{
			if (currentTexture != NULL)
			{
				// render the range for the current texture
				RenderQueueRange(currentTexture, startOffset, stopOffset);
			}

			// switch to the new texture and begin the new range
			currentTexture = m_textures[i];
			startOffset = i;
			stopOffset = startOffset;
		}
		++stopOffset;
	}
	if (currentTexture != NULL)
		RenderQueueRange(currentTexture, startOffset, stopOffset);

	m_graphicsDevice->UnbindVertexBuffer();
}

void BillboardSpriteBatch::RenderQueueRange(const Texture *texture, uint32_t firstSprite, uint32_t lastSprite)
{
	const int TRIANGLES_PER_SPRITE = 2;
	uint32_t vertexOffset = firstSprite * VERTICES_PER_SPRITE;
	uint32_t numTriangles = (lastSprite - firstSprite) * TRIANGLES_PER_SPRITE;

	m_graphicsDevice->BindTexture(texture);
	m_shader->SetTextureHasAlphaOnly(texture->GetFormat() == TEXTURE_FORMAT_ALPHA);
	m_graphicsDevice->RenderTriangles(vertexOffset, numTriangles);
}

inline Matrix4x4 BillboardSpriteBatch::GetTransformFor(BILLBOARDSPRITE_TYPE type, const Vector3 &position) const
{
	Matrix4x4 out;

	switch (type)
	{
	case BILLBOARDSPRITE_SPHERICAL:
		out = Matrix4x4::CreateBillboard(position, m_cameraPosition, UP, m_cameraForward);
		break;
	case BILLBOARDSPRITE_CYLINDRICAL:
		out = Matrix4x4::CreateCylindricalBillboard(position, m_cameraPosition, m_cameraForward, Y_AXIS);
		break;
	case BILLBOARDSPRITE_SCREENALIGNED:
		out = Matrix4x4::CreateScreenAlignedBillboard(position, UP, m_cameraForward);
		break;
	case BILLBOARDSPRITE_SCREENANDAXISALIGNED:
		out = Matrix4x4::CreateScreenAndAxisAlignedBillboard(position, m_cameraForward, Y_AXIS);
		break;
	}

	return out;
}

void BillboardSpriteBatch::CheckForNewSpriteSpace()
{
	// m_currentSpritePointer = zero-based index
	// m_currentSpriteCapacity = count of items (not zero-based)
	if (m_currentSpritePointer >= m_currentSpriteCapacity)
	{
		// increase capacity by one
		++m_currentSpriteCapacity;
		m_vertices->Extend(VERTICES_PER_SPRITE);
		m_textures.resize(m_currentSpriteCapacity);
	}
}

