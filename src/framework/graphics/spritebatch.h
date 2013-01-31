#ifndef __FRAMEWORK_GRAPHICS_SPRITEBATCH_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SPRITEBATCH_H_INCLUDED__

#include "../common.h"
#include "blendstate.h"
#include "color.h"
#include "renderstate.h"
#include "../math/matrix4x4.h"
#include "../math/rectf.h"
#include <stl/vector.h>

class GraphicsDevice;
class SpriteFont;
class SpriteShader;
class Texture;
class TextureAtlas;
class VertexBuffer;
struct Rect;
struct Vector3;

enum SPRITEBATCH_ENTITY_TYPE
{
	SPRITEBATCH_ENTITY_SPRITE,
	SPRITEBATCH_ENTITY_LINE,
	SPRITEBATCH_ENTITY_FILLEDBOX
};

struct SpriteBatchEntity
{
	const Texture *texture;
	SPRITEBATCH_ENTITY_TYPE type;
	uint32_t firstVertex;
	uint32_t lastVertex;
};

/**
 * Wrapper for 2D sprite and text rendering.
 */
class SpriteBatch
{
public:
	/**
	 * Creates a sprite batch object.
	 * @param graphicsDevice the graphics device to perform rendering with
	 */
	SpriteBatch(GraphicsDevice *graphicsDevice);

	virtual ~SpriteBatch();

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
	 * Renders a texture as a sprite.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, int16_t x, int16_t y, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, int16_t x, int16_t y, uint16_t width, uint16_t height, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param texCoordLeft left U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordTop top V texture coordinate defining a sub-region of the texture to render
	 * @param texCoordRight right U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordBottom bottom V texture coordinate defining a sub-region of the texture to render
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, int16_t x, int16_t y, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param texCoordLeft left U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordTop top V texture coordinate defining a sub-region of the texture to render
	 * @param texCoordRight right U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordBottom bottom V texture coordinate defining a sub-region of the texture to render
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, int16_t x, int16_t y, uint16_t width, uint16_t height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite. The given world position will be
	 * projected to 2D screen coordinates where the texture will be centered
	 * on and rendered at.
	 * @param worldPosition 3D world position to render at
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, const Vector3 &worldPosition, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite. The given world position will be
	 * projected to 2D screen coordinates where the texture will be centered
	 * on and rendered at.
	 * @param worldPosition 3D world position to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, const Vector3 &worldPosition, uint16_t width, uint16_t height, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite. The given world position will be
	 * projected to 2D screen coordinates where the texture will be centered
	 * on and rendered at.
	 * @param worldPosition 3D world position to render at
	 * @param texCoordLeft left U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordTop top V texture coordinate defining a sub-region of the texture to render
	 * @param texCoordRight right U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordBottom bottom V texture coordinate defining a sub-region of the texture to render
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, const Vector3 &worldPosition, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite. The given world position will be
	 * projected to 2D screen coordinates where the texture will be centered
	 * on and rendered at.
	 * @param worldPosition 3D world position to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param texCoordLeft left U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordTop top V texture coordinate defining a sub-region of the texture to render
	 * @param texCoordRight right U texture coordinate defining a sub-region of the texture to render
	 * @param texCoordBottom bottom V texture coordinate defining a sub-region of the texture to render
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, const Vector3 &worldPosition, uint16_t width, uint16_t height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite.
	 * @param index the index of the sub-texture / tile to render
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint32_t index, int16_t x, int16_t y, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite.
	 * @param index the index of the sub-texture / tile to render
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint32_t index, int16_t x, int16_t y, uint16_t width, uint16_t height, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite. The given 
	 * world position will be  projected to 2D screen coordinates where the
	 * texture will be centered on and rendered at.
	 * @param index the index of the sub-texture / tile to render
	 * @param worldPosition 3D world position to render at
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint32_t index, const Vector3 &worldPosition, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite. The given 
	 * world position will be  projected to 2D screen coordinates where the
	 * texture will be centered on and rendered at.
	 * @param index the index of the sub-texture / tile to render
	 * @param worldPosition 3D world position to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint32_t index, const Vector3 &worldPosition, uint16_t width, uint16_t height, const Color &color = COLOR_WHITE);

	/**
	 * Renders text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, int16_t x, int16_t y, const Color &color, const char *text);

	/**
	 * Renders text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, int16_t x, int16_t y, const Color &color, float scale, const char *text);

	/**
	 * Renders text as series of sprites. The given world position will be
	 * projected to 2D screen coordinates where the text will be centered on
	 * and rendered at.
	 * @param font the font to render with
	 * @param worldPosition 3D world position to render at
	 * @param color the color to render the text in
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, const char *text);

	/**
	 * Renders text as series of sprites. The given world position will be
	 * projected to 2D screen coordinates where the text will be centered on
	 * and rendered at.
	 * @param font the font to render with
	 * @param worldPosition 3D world position to render at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *text);

	/**
	 * Renders formatted text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, int16_t x, int16_t y, const Color &color, const char *format, ...);

	/**
	 * Renders formatted text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, int16_t x, int16_t y, const Color &color, float scale, const char *format, ...);

	/**
	 * Renders formatted text as series of sprites. The given world position
	 * will be projected to 2D screen coordinates where the text will be 
	 * centered on and rendered at.
	 * @param font the font to render with
	 * @param worldPosition 3D world position to render at
	 * @param color the color to render the text in
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, const char *format, ...);

	/**
	 * Renders formatted text as series of sprites. The given world position
	 * will be projected to 2D screen coordinates where the text will be 
	 * centered on and rendered at.
	 * @param font the font to render with
	 * @param worldPosition 3D world position to render at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, const Vector3 &worldPosition, const Color &color, float scale, const char *format, ...);

	/**
	 * Renders a line between two points.
	 * @param x1 X coordinate of the first point
	 * @param y1 Y coordinate of the first point
	 * @param x2 X coordinate of the second point
	 * @param y2 Y coordinate of the second point
	 * @param color the color to render the line with
	 */
	void RenderLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, const Color &color);

	/**
	 * Renders a box.
	 * @param left left X coordinate of the box
	 * @param top top Y coordinate of the box
	 * @param right right X coordinate of the box
	 * @param bottom bottom Y coordinate of the box
	 * @param color the color to render the box with
	 */
	void RenderBox(int16_t left, int16_t top, int16_t right, int16_t bottom, const Color &color);

	/**
	 * Renders a box.
	 * @param rect the position and dimensions of the box
	 * @param color the color to render the box with
	 */
	void RenderBox(const Rect &rect, const Color &color);

	/**
	 * Renders a filled box.
	 * @param left left X coordinate of the box
	 * @param top top Y coordinate of the box
	 * @param right right X coordinate of the box
	 * @param bottom bottom Y coordinate of the box
	 * @param color the color to render the box with
	 */
	void RenderFilledBox(int16_t left, int16_t top, int16_t right, int16_t bottom, const Color &color);

	/**
	 * Renders a filled box.
	 * @param rect the position and dimensions of the box
	 * @param color the color to render the box with
	 */
	void RenderFilledBox(const Rect &rect, const Color &color);

	/**
	 * Ends a rendering block, flushing out all rendering calls made
	 * since the Begin() call to the video card.
	 */
	void End();

	/**
	 * Defines a clipping region which all subsequent rendering will be
	 * clipped to.
	 * @param rect the clipping region
	 */
	void SetClipRegion(const Rect &rect);

	/**
	* Defines a clipping region which all subsequent rendering will be
	* clipped to.
	* @param left left X coordinate of the clipping region
	* @param top top Y coordinate of the clipping region
	* @param right right X coordinate of the clipping region
	* @param bottom bottom Y coordinate of the clipping region
	*/
	void SetClipRegion(int16_t left, int16_t top, int16_t right, int16_t bottom);

	/**
	 * Removes the clipping region. All subsequent rendering will no longer
	 * be clipped.
	 */
	void ClearClipRegion();

private:
	void InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader);

	void AddSprite(const Texture *texture, int16_t destLeft, int16_t destTop, int16_t destRight, int16_t destBottom, uint16_t sourceLeft, uint16_t sourceTop, uint16_t sourceRight, uint16_t sourceBottom, const Color &color);
	void AddSprite(const Texture *texture, int16_t destLeft, int16_t destTop, int16_t destRight, int16_t destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	void AddSprite(const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	BOOL ClipSpriteCoords(float &left, float &top, float &right, float &bottom, float &texCoordLeft, float &texCoordTop, float &texCoordRight, float &texCoordBottom);
	void SetSpriteInfo(uint32_t spriteIndex, const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);

	void AddLine(float x1, float y1, float x2, float y2, const Color &color);
	void SetLineInfo(uint32_t spriteIndex, float x1, float y1, float x2, float y2, const Color &color);
	BOOL ClipLineCoords(float &x1, float &y1, float &x2, float &y2);

	void AddFilledBox(float left, float top, float right, float bottom, const Color &color);
	void SetFilledBoxInfo(uint32_t spriteIndex, float left, float top, float right, float bottom, const Color &color);
	BOOL ClipFilledBoxCoords(float &left, float &top, float &right, float &bottom);

	void RenderQueue();
	void RenderQueueRange(const SpriteBatchEntity *firstEntity, const SpriteBatchEntity *lastEntity);

	void CheckForNewSpriteSpace(SPRITEBATCH_ENTITY_TYPE type);

	uint32_t GetVerticesRequiredFor(SPRITEBATCH_ENTITY_TYPE type);
	int16_t FixYCoord(int16_t y, uint16_t sourceHeight) const;
	float FixYCoord(int16_t y, float sourceHeight) const;

	GraphicsDevice *m_graphicsDevice;
	SpriteShader *m_shader;
	RenderState *m_renderState;
	BlendState *m_blendState;
	BOOL m_isRenderStateOverridden;
	RenderState m_overrideRenderState;
	BOOL m_isBlendStateOverridden;
	BlendState m_overrideBlendState;
	VertexBuffer *m_vertices;
	stl::vector<SpriteBatchEntity> m_entities;
	uint32_t m_currentSpritePointer;
	Matrix4x4 m_previousProjection;
	Matrix4x4 m_previousModelview;
	BOOL m_isClipping;
	RectF m_clipRegion;

	BOOL m_begunRendering;
};

#endif
