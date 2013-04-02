#include "../framework/common.h"
#include "../framework/debug.h"

#include "contentcache.h"

#include "../framework/content/contentmanager.h"
#include "../framework/content/spritefontparam.h"
#include "../framework/graphics/gridtextureatlas.h"
#include "../framework/graphics/image.h"
#include "../framework/graphics/spritefont.h"
#include "../framework/graphics/texture.h"
#include "../framework/graphics/textureatlas.h"

ContentCache::ContentCache(ContentManager *contentManager)
{
	m_contentManager = contentManager;
	m_uiFontSize = 0;
}

ContentCache::~ContentCache()
{
	m_contentManager->Free<Texture>(m_uiSkin, true);
	m_contentManager->Free<Image>(m_uiSkinImage, true);
	m_contentManager->Free<SpriteFont>(m_standardFont, true);
	m_contentManager->Free<SpriteFont>(m_uiFont, true);
}

void ContentCache::OnLoadGame()
{
	m_uiSkinFilename = "assets://ui_skin.png";
	m_uiSkinImage = m_contentManager->Get<Image>(m_uiSkinFilename, true);
	m_uiSkin = m_contentManager->Get<Texture>(m_uiSkinFilename, true);
	m_standardFont = m_contentManager->Get<SpriteFont>("assets://fonts/dlxfont.ttf", SpriteFontParam(8), true);
	m_uiFontFilename = "assets://fonts/dlxfont.ttf";
	m_uiFontSize = 8;
	m_uiFont = m_contentManager->Get<SpriteFont>(m_uiFontFilename, SpriteFontParam(m_uiFontSize), true);
}

