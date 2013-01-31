#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHTRIANGLE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHTRIANGLE_H_INCLUDED__

#include "../../common.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"

struct SkeletalMeshTriangle
{
	int32_t vertices[3];
	int32_t subMeshIndex;
};

#endif
