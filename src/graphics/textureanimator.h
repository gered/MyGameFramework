#ifndef __GRAPHICS_TEXTUREANIMATOR_H_INCLUDED__
#define __GRAPHICS_TEXTUREANIMATOR_H_INCLUDED__

#include "../framework/common.h"
#include "textureatlasanimationsequence.h"
#include <stl/list.h>
#include <stl/string.h>

class ContentManager;
class GraphicsDevice;
class Image;
class Texture;
class TextureAtlas;

typedef stl::list<TextureAtlasAnimationSequence> TextureAtlasAnimations;

class TextureAnimator
{
public:
	TextureAnimator(ContentManager *contentManager, GraphicsDevice *graphicsDevice);
	virtual ~TextureAnimator();

	void ResetAll();

	void AddTileSequence(TextureAtlas *atlas, uint tileToBeAnimated, uint start, uint stop, float delay, bool loop = true);
	void AddTileSequence(const stl::string &name, TextureAtlas *atlas, uint tileToBeAnimated, uint start, uint stop, float delay, bool loop = true);
	void ResetTileSequence(const stl::string &name);
	void StopTileSequence(const stl::string &name, bool restoreOriginalTile = false);
	void EnableTileSequence(const stl::string &name, bool enable);

	void OnUpdate(float delta);
	void OnNewContext();

private:
	TextureAtlasAnimationSequence* FindTileSequenceByName(const stl::string &name);
	void UpdateTextureWithCurrentTileFrame(TextureAtlasAnimationSequence &sequence);
	void RestoreTextureWithOriginalTile(TextureAtlasAnimationSequence &sequence);

	ContentManager *m_contentManager;
	GraphicsDevice *m_graphicsDevice;
	TextureAtlasAnimations m_textureAtlasAnimations;
};

#endif

