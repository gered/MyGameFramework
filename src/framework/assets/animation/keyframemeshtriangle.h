#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHTRIANGLE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHTRIANGLE_H_INCLUDED__

#include "../../common.h"

/**
 * Contains vertex and texture coordinate indices for a triangle in a 
 * keyframe mesh.
 */
struct KeyframeMeshTriangle
{
	uint32_t vertices[3];
	uint32_t textureCoords[3];
};

#endif
