#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHTRIANGLE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHTRIANGLE_H_INCLUDED__

#include "../../common.h"

/**
 * Contains vertex and sub-mesh indices for a triangle in a static mesh.
 */
struct StaticMeshTriangle
{
	uint32_t vertices[3];
	uint32_t subMeshIndex;
};

#endif
