#ifndef __FRAMEWORK_GRAPHICS_SPRITEFONT_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SPRITEFONT_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"

class Texture;
class TextureAtlas;
struct TextureAtlasTile;

const uint8_t LOW_GLYPH = 32;
const uint8_t HIGH_GLYPH = 127;

/**
 * Represents a font that has been pre-rendered at a given size to a 
 * texture atlas so that text can be rendered quickly as sprites.
 */
class SpriteFont : public Content
{
public:
	static CONTENT_TYPE GetType()
	{
		static CONTENT_TYPE typeName = "SpriteFont";
		return typeName;
	}
	CONTENT_TYPE GetTypeOf() const                         { return GetType(); }

	/**
	 * Creates an uninitialized sprite font object.
	 */
	SpriteFont();
	virtual ~SpriteFont();

	/**
	 * Populates this sprite font object with texture and glyph data
	 * necessary for rendering with it.
	 * @param texture the texture containing the font glyphs. This texture
	 *                is expected to have been manually created and not
	 *                loaded through ContentManager.
	 * @param glyphs texture atlas with pre-defined sub-textures corresponding
	 *               to the font's individual characters (glyphs)
	 * @param size the size that the glyphs were rendered at
	 */
	void Load(Texture *texture, TextureAtlas *glyphs, uint8_t size);

	/**
	 * Lost OpenGL graphics context callback. This will free the texture
	 * and texture atlas passed to Load().
	 */
	void OnLostContext();

	/**
	 * @return the size that the glyphs were originally rendered at
	 */
	uint8_t GetSize() const                                { return m_size; }

	/**
	 * @return the texture containing the font glyphs
	 */
	const Texture* GetTexture() const                      { return m_texture; }

	/**
	 * @return the number of pixels that one line of text rendered with this
	 *         font takes up
	 */
	uint8_t GetLetterHeight() const                        { return m_letterHeight; }

	/**
	 * @param c the character to get sub-texture dimensions/positioning for
	 * @return the sub-texture dimensions/position corresponding with
	 *         the specified character
	 */
	const TextureAtlasTile& GetGlyph(unsigned char c) const;

	/**
	 * Measures the given string of text and returns the width and height
	 * that it would take up in pixels if rendered.
	 * @param width the width in pixels of the rendered text if not NULL
	 * @param height the height in pixels of the rendered text if not NULL
	 * @param format the text to measure
	 */
	void MeasureString(uint16_t *width, uint16_t *height, const char *format, ...) const;

	/**
	 * Measures the given string of text and returns the width and height
	 * that it would take up in pixels if rendered.
	 * @param width the width in pixels of the rendered text if not NULL
	 * @param height the height in pixels of the rendered text if not NULL
	 * @param scale a scaling factor for rendering to take into account
	 * @param format the text to measure
	 */
	void MeasureString(uint16_t *width, uint16_t *height, float scale, const char *format, ...) const;

private:
	uint8_t m_size;

	Texture *m_texture;
	TextureAtlas *m_glyphs;
	uint8_t m_letterHeight;
};

#endif
