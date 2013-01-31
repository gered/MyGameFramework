#ifndef __GRAPHICS_TEXTUREATLASANIMATIONSEQUENCE_H_INCLUDED__
#define __GRAPHICS_TEXTUREATLASANIMATIONSEQUENCE_H_INCLUDED__

#include "../framework/common.h"
#include <stl/string.h>

class Image;
class TextureAtlas;

struct TextureAtlasAnimationSequence
{
	TextureAtlas *atlas;
	uint32_t animatingIndex;
	uint32_t start;
	uint32_t stop;
	uint32_t current;
	float delay;
	float currentFrameTime;
	BOOL isAnimating;
	BOOL loop;
	Image *originalAnimatingTile;
	Image **frames;
	stl::string name;

	TextureAtlasAnimationSequence();

	uint32_t GetNumFrames() const;
	BOOL IsAnimationFinished() const;
};

inline TextureAtlasAnimationSequence::TextureAtlasAnimationSequence()
{
	atlas = NULL;
	animatingIndex = 0;
	start = 0;
	stop = 0;
	current = 0;
	delay = 0.0f;
	currentFrameTime = 0.0f;
	isAnimating = FALSE;
	loop = FALSE;
	originalAnimatingTile = NULL;
	frames = NULL;
}

inline uint32_t TextureAtlasAnimationSequence::GetNumFrames() const
{
	return stop - start + 1;
}

inline BOOL TextureAtlasAnimationSequence::IsAnimationFinished() const
{
	if (isAnimating && !loop && current == stop)
		return TRUE;
	else
		return FALSE;
}

#endif

