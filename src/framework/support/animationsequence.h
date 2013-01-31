#ifndef __FRAMEWORK_SUPPORT_ANIMATIONSEQUENCE_H_INCLUDED__
#define __FRAMEWORK_SUPPORT_ANIMATIONSEQUENCE_H_INCLUDED__

#include "../common.h"
#include <stl/map.h>
#include <stl/string.h>

struct AnimationSequence
{
	uint32_t start;
	uint32_t stop;
	float delay;
	BOOL tileIsMultiDirectional;
	uint32_t tileDirectionFrameOffset;

	AnimationSequence();
	AnimationSequence(uint32_t start, uint32_t stop, float delay = 1.0f, BOOL tileIsMultiDirectional = FALSE, uint32_t tileDirectionFrameOffset = 0);
};

typedef stl::map<stl::string, AnimationSequence> AnimationList;
typedef stl::map<stl::string, AnimationSequence*> AnimationListPtr;

inline AnimationSequence::AnimationSequence()
{
	this->start = 0;
	this->stop = 0;
	this->delay = 0.0f;
	this->tileIsMultiDirectional = FALSE;
	this->tileDirectionFrameOffset = 0;
}

inline AnimationSequence::AnimationSequence(uint32_t start, uint32_t stop, float delay, BOOL tileIsMultiDirectional, uint32_t tileDirectionFrameOffset)
{
	this->start = start;
	this->stop = stop;
	this->delay = delay;
	this->tileIsMultiDirectional = tileIsMultiDirectional;
	this->tileDirectionFrameOffset = tileDirectionFrameOffset;
}

#endif

