#ifndef __FRAMEWORK_SUPPORT_ANIMATIONSEQUENCE_H_INCLUDED__
#define __FRAMEWORK_SUPPORT_ANIMATIONSEQUENCE_H_INCLUDED__

#include "../common.h"
#include <stl/map.h>
#include <stl/string.h>

struct AnimationSequence
{
	uint start;
	uint stop;
	float delay;
	bool tileIsMultiDirectional;
	uint tileDirectionFrameOffset;

	AnimationSequence();
	AnimationSequence(uint start, uint stop, float delay = 1.0f, bool tileIsMultiDirectional = false, uint tileDirectionFrameOffset = 0);
};

typedef stl::map<stl::string, AnimationSequence> AnimationList;
typedef stl::map<stl::string, AnimationSequence*> AnimationListPtr;

inline AnimationSequence::AnimationSequence()
{
	this->start = 0;
	this->stop = 0;
	this->delay = 0.0f;
	this->tileIsMultiDirectional = false;
	this->tileDirectionFrameOffset = 0;
}

inline AnimationSequence::AnimationSequence(uint start, uint stop, float delay, bool tileIsMultiDirectional, uint tileDirectionFrameOffset)
{
	this->start = start;
	this->stop = stop;
	this->delay = delay;
	this->tileIsMultiDirectional = tileIsMultiDirectional;
	this->tileDirectionFrameOffset = tileDirectionFrameOffset;
}

#endif

