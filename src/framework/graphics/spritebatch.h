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
	void Render(const Texture *texture, int x, int y, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture as a sprite.
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const Texture *texture, int x, int y, uint width, uint height, const Color &color = COLOR_WHITE);

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
	void Render(const Texture *texture, int x, int y, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

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
	void Render(const Texture *texture, int x, int y, uint width, uint height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

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
	void Render(const Texture *texture, const Vector3 &worldPosition, uint width, uint height, const Color &color = COLOR_WHITE);

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
	void Render(const Texture *texture, const Vector3 &worldPosition, uint width, uint height, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite.
	 * @param index the index of the sub-texture / tile to render
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint index, int x, int y, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite.
	 * @param index the index of the sub-texture / tile to render
	 * @param x X coordinate to render at
	 * @param y Y coordinate to render at
	 * @param width custom width to scale the texture to during rendering
	 * @param height custom height to scale the texture to during rendering
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint index, int x, int y, uint width, uint height, const Color &color = COLOR_WHITE);

	/**
	 * Renders a texture atlas sub-texture / tile as a sprite. The given 
	 * world position will be  projected to 2D screen coordinates where the
	 * texture will be centered on and rendered at.
	 * @param index the index of the sub-texture / tile to render
	 * @param worldPosition 3D world position to render at
	 * @param color color to tint the texture with
	 */
	void Render(const TextureAtlas *atlas, uint index, const Vector3 &worldPosition, const Color &color = COLOR_WHITE);

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
	void Render(const TextureAtlas *atlas, uint index, const Vector3 &worldPosition, uint width, uint height, const Color &color = COLOR_WHITE);

	/**
	 * Renders text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, int x, int y, const Color &color, const char *text);

	/**
	 * Renders text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param text the string of text to render
	 */
	void Render(const SpriteFont *font, int x, int y, const Color &color, float scale, const char *text);

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
	void Printf(const SpriteFont *font, int x, int y, const Color &color, const char *format, ...);

	/**
	 * Renders formatted text as series of sprites.
	 * @param font the font to render with
	 * @param x X coordinate to begin rendering at
	 * @param y Y coordinate to begin rendering at
	 * @param color the color to render the text in
	 * @param scale scaling factor to render with
	 * @param format the string of text to render
	 */
	void Printf(const SpriteFont *font, int x, int y, const Color &color, float scale, const char *format, ...);

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
	void SetClipRegion(int left, int top, int right, int bottom);

	/**
	 * Removes the clipping region. All subsequent rendering will no longer
	 * be clipped.
	 */
	void ClearClipRegion();

private:
	void InternalBegin(const RenderState *renderState, const BlendState *blendState, SpriteShader *shader);

	void AddSprite(const Texture *texture, int destLeft, int destTop, int destRight, int destBottom, uint sourceLeft, uint sourceTop, uint sourceRight, uint sourceBottom, const Color &color);
	void AddSprite(const Texture *texture, int destLeft, int destTop, int destRight, int destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	void AddSprite(const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);
	bool ClipSpriteCoords(float &left, float &top, float &right, float &bottom, float &texCoordLeft, float &texCoordTop, float &texCoordRight, float &texCoordBottom);
	void SetSpriteInfo(uint spriteIndex, const Texture *texture, float destLeft, float destTop, float destRight, float destBottom, float texCoordLeft, float texCoordTop, float texCoordRight, float texCoordBottom, const Color &color);

	void RenderQueue();
	void RenderQueueRange(uint firstSpriteIndex, uint lastSpriteIndex);

	uint GetRemainingSpriteSpaces() const;
	void AddMoreSpriteSpace(uint numSprites);

	int FixYCoord(int y, uint sourceHeight) const;
	float FixYCoord(int y, float sourceHeight) const;

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
	Matrix4x4 m_previousProjection;
	Matrix4x4 m_previousModelview;
	bool m_isClipping;
	RectF m_clipRegion;

	bool m_begunRendering;
};

#endif
