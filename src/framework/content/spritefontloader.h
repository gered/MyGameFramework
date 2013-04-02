#ifndef __FRAMEWORK_CONTENT_SPRITEFONTLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_SPRITEFONTLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../graphics/spritefont.h"
#include "../math/rect.h"
#include <stb_truetype.h>
#include <stl/string.h>

class ContentManager;
class File;

struct SpriteFontGlyphMetrics
{
	int index;
	float scale;
	Rect dimensions;
	int ascent;
	int descent;
	int lineGap;
	int advance;
	int letterWidth;
	int letterHeight;

	SpriteFontGlyphMetrics()
	{
		index = 0;
		scale = 0.0f;
		dimensions = Rect(0, 0, 0, 0);
		ascent = 0;
		descent = 0;
		lineGap = 0;
		advance = 0;
		letterWidth = 0;
		letterHeight = 0;
	}
};

/**
 * Content loader for sprite fonts.
 */
class SpriteFontLoader : public ContentLoaderMapStoreBase<SpriteFont>
{
public:
	/**
	 * Creates a sprite font loader.
	 * @param contentManager the parent content manager
	 */
	SpriteFontLoader(ContentManager *contentManager);

	/**
	 * Creates a sprite font loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with font filenames
	 */
	SpriteFontLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~SpriteFontLoader();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();
	
protected:
	stl::string ProcessFilename(const stl::string &filename, const ContentParam *params) const;

	SpriteFont* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(SpriteFont *content);
	
private:
	void DecomposeFilename(const stl::string &filename, stl::string &outFilename, uint &outSize) const;
	SpriteFont* Load(File *file, uint size, SpriteFont *existing) const;

	bool GetGlyphMetrics(stbtt_fontinfo *fontInfo, char glyph, uint size, SpriteFontGlyphMetrics *glyphMetrics) const;
};

#endif
