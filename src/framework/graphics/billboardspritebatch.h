#ifndef __FRAMEWORK_GRAPHICS_BILLBOARDSPRITEBATCH_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_BILLBOARDSPRITEBATCH_H_INCLUDED__

#include "../common.h"
#include "blendstate.h"
#include "color.h"
#include "renderstate.h"
#include "../math/vector3.h"
#include <stl/vector.h>

class GraphicsDevice;
class SpriteFont;
class SpriteShader;
class Texture;
class TextureAtlas;
class VertexBuffer;
struct Matrix4x4;

enum BILLBOARDSPRITE_TYPE
{
	BILLBOARDSPRITE_SPHERICAL,
	BILLBOARDSPRITE_CYLINDRICAL,
	BILLBOARDSPRITE_SCREENALIGNED,
	BILLBOARDSPRITE_SCREENANDAXISALIGNED
};

/**
 * Wrapper for 3D sprite and text rendering as billboards.
 */
class BillboardSpriteBatch
{
public:
	/**
	 * Creates a billboard sprite batch object.
	 * @param graphicsDevice the graphics device to perform rendering with
	 */
	BillboardSpriteBatch(GraphicsDevice *graphicsDevice);

	virtual ~BillboardSpriteBatch();

	/**
	 * Begins a rendering block. All rendering with this object should be
	 * performed after this and then completed with a call to End().
	 * @param shader shader to render with, or NULL to use a default shader
	 */
	void Begin(SpriteShader *shader = NULL);

	/**
	 * Begins a rendering block. All rendering with this object should be
	 * performed after this and then completed with a call to End().
	 * @param renderState custom render state to use for rendering
	 * @param shader shader to render with, or NULL to use a default shader
	 */
	void Begin(const RenderState &renderState, SpriteShader *shader = NULL);

	/**
	 * Begins a rendering block. All rendering with this object should be
	 * performed after this and then completed with a call to End().
	 * @param blendState custom blend state to use for rendering
	 * @param shader shader to render with, or NULL to use a default shader
	 */
	void Begin(const BlendState &blendState, SpriteShader *shader = NULL);

	/**
	 * Begins a rendering block. All rendering with this object should be
	 * performed after this and then completed with a call to End().
	 * @param renderState custom render state to use for rendering
	 * @param blendState custom blend state to use for rendering
	 * @param shader shader to render with, or NULL to use a default shader
	 */
	void Begin(const RenderState &renderState, const BlendState &blendState, SpriteShader *shader = NULL);

	/**
	 * Renders a texture as a billboard. The billboard will be centered on
	 * the coordinates provided.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param z Z coordinate to render at
	 * @param width width to render the billboard at
	 * @param height height to render the billboard at
	 * @param type the type of billboard to render this texture as
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, float x, float y, float z, float width, float height, BILLBOARDSPRITE_TYPE type = BILLBOARDSPRITE_SPHERICAL, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a billboard. The billboard will be centered on
	 * the coordinates provided.
	 * @param position position to render at
	 * @param width width to render the billboard at
	 * @param height height to render the billboard at
	 * @param type the type of billboard to render this texture as
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, const Vector3 &position, float width, float height, BILLBOARDSPRITE_TYPE type = BILLBOARDSPRITE_SPHERICAL, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a billboard. The billboard 
	 * will be centered on the coordinates provided.
	 * @param index the index of the sub-texture / tile to render
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param z Z coordinate to render at
	 * @param width width to render the billboard at
	 * @param height height to render the billboard at
	 * @param type the type of billboard to render this texture as
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint index, float x, float y, float z, float width, float height, BILLBOARDSPRITE_TYPE type = BILLBOARDSPRITE_SPHERICAL, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a billboard. The billboard 
	 * will be centered on the coordinates provided.
	 * @param index the index of the sub-texture / tile to render
	 * @param position position to render at
	 * @param width width to render the billboard at
	 * @param height height to render the billboard at
	 * @param type the type of billboard to render this texture as
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint index, const Vector3 &position, float width, float height, BILLBOARDSPRITE_TYPE type = BILLBOARDSPRITE_SPHERICAL, const Color &color = COLOR_WHITE);

	/**
	 * Renders text as series of billboards. The text will be centered on
	 * the coordinates provided.
	 * @param font the font to render with
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param z Z coordinate to render at
	 * @param type the type of billboard to render the text billboards as
	 * @param color the color to render the text in
	 * @param pixelScale scale factor applied to each pixel of the font texture
	 *        to get the final text render size
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, float x, float y, float z, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *text);

	/**
	 * Renders text as series of billboards. The text will be centered on
	 * the coordinates provided.
	 * @param font the font to render with
	 * @param position position to render at
	 * @param type the type of billboard to render the text billboards as
	 * @param color the color to render the text in
	 * @param pixelScale scale factor applied to each pixel of the font texture
	 *        to get the final text render size
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, const Vector3 &position, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *text);

	/**
	 * Renders formatted text as series of billboards. The text will be
	 * centered on the coordinates provided.
	 * @param font the font to render with
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param z Z coordinate to render at
	 * @param type the type of billboard to render the text billboards as
	 * @param color the color to render the text in
	 * @param pixelScale scale factor applied to each pixel of the font texture
	 *        to get the final text render size
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, float x, float y, float z, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *format, ...);

	/**
	 * Renders formatted text as series of billboards. The text will be
	 * centered on the coordinates provided.
	 * @param font the font to render with
	 * @param position position to render at
	 * @param type the type of billboard to render the text billboards as
	 * @param color the color to render the text in
	 * @param pixelScale scale factor applied to each pixel of the font texture
	 *        to get the final text render size
	 * @param text the string of text to render
	 */
	void Printf(const SpriteFont *font, const Vector3 &position, BILLBOARDSPRITE_TYPE type, const Color &color, float pixelScale, const char *format, ...);

	/**
	 * Ends a rendering block, flushing out all rendering calls made
	 * since the Begin() call to the video card.
	 */
	void End();

private:
	void Initialize(GraphicsDevice *graphicsDevice, uint maxSprites);
	void InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader);
	void AddSprite(BILLBOARDSPRITE_TYPE type, const Texture *texture, const Vector3 &position, float width, float height, uint sourceLeft, uint sourceTop, uint sourceRight, uint sourceBottom, const Color &color);
	void AddSprite(BILLBOARDSPRITE_TYPE type, const Texture *texture, const Vector3 &position, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	void AddSprite(BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, uint sourceLeft, uint sourceTop, uint sourceRight, uint sourceBottom, const Color &color);
	void AddSprite(BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	void SetSpriteInfo(uint spriteIndex, BILLBOARDSPRITE_TYPE type, const Matrix4x4 &transform, const Texture *texture, const Vector3 &offset, float width, float height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	void RenderQueue();
	void RenderQueueRange(const Texture *texture, uint firstSprite, uint lastSprite);

	Matrix4x4 GetTransformFor(BILLBOARDSPRITE_TYPE type, const Vector3 &position) const;
	void CheckForNewSpriteSpace();

	uint m_currentSpriteCapacity;
	GraphicsDevice *m_graphicsDevice;
	SpriteShader *m_shader;
	RenderState *m_renderState;
	BlendState *m_blendState;
	bool m_isRenderStateOverridden;
	RenderState m_overrideRenderState;
	bool m_isBlendStateOverridden;
	BlendState m_overrideBlendState;
	VertexBuffer *m_vertices;
	stl::vector<const Texture*> m_textures;
	uint m_currentSpritePointer;

	Vector3 m_cameraPosition;
	Vector3 m_cameraForward;

	bool m_begunRendering;
};

#endif
