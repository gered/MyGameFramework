#ifndef __GRAPHICS_TEXTUREATLASANIMATIONSEQUENCE_H_INCLUDED__
#define __GRAPHICS_TEXTUREATLASANIMATIONSEQUENCE_H_INCLUDED__

#include "../framework/common.h"
#include <stl/string.h>

class Image;
class TextureAtlas;

struct TextureAtlasAnimationSequence
{
	TextureAtlas *atlas;
	uint animatingIndex;
	uint start;
	uint stop;
	uint current;
	float delay;
	float currentFrameTime;
	bool isAnimating;
	bool loop;
	Image *originalAnimatingTile;
	Image **frames;
	stl::string name;

	TextureAtlasAnimationSequence();

	uint GetNumFrames() const;
	bool IsAnimationFinished() const;
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
	isAnimating = false;
	loop = false;
	originalAnimatingTile = NULL;
	frames = NULL;
}

inline uint TextureAtlasAnimationSequence::GetNumFrames() const
{
	return stop - start + 1;
}

inline bool TextureAtlasAnimationSequence::IsAnimationFinished() const
{
	if (isAnimating && !loop && current == stop)
		return true;
	else
		return false;
}

#endif

