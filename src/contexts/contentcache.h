#ifndef __CONTEXTS_CONTENTCACHE_H_INCLUDED__
#define __CONTEXTS_CONTENTCACHE_H_INCLUDED__

#include "../framework/common.h"

class ContentManager;
class Image;
class SpriteFont;
class Texture;
class TextureAtlas;

#include <stl/string.h>

class ContentCache
{
public:
	ContentCache(ContentManager *contentManager);
	virtual ~ContentCache();

	void OnLoadGame();

	Texture* GetUISkin() const                             { return m_uiSkin; }
	Image* GetUISkinImage() const                          { return m_uiSkinImage; }
	const stl::string& GetUISkinFilename() const           { return m_uiSkinFilename; }
	SpriteFont* GetFont() const                            { return m_standardFont; }
	SpriteFont* GetUIFont() const                          { return m_uiFont; }
	const stl::string& GetUIFontFilename() const           { return m_uiFontFilename; }
	uint8_t GetUIFontSize() const                          { return m_uiFontSize; }

private:
	ContentManager *m_contentManager;

	Texture *m_uiSkin;
	Image *m_uiSkinImage;
	stl::string m_uiSkinFilename;
	SpriteFont *m_standardFont;
	SpriteFont *m_uiFont;
	stl::string m_uiFontFilename;
	uint8_t m_uiFontSize;
};

#endif

