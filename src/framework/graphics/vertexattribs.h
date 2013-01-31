#ifndef __FRAMEWORK_GRAPHICS_VERTEXATTRIBS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VERTEXATTRIBS_H_INCLUDED__

#include "../common.h"

/**
 * Constant values to refer to different standard types of vertex attributes.
 */
enum VERTEX_ATTRIBS
{
	VERTEX_POS_2D = 1,
	VERTEX_POS_3D = 2,
	VERTEX_NORMAL = 4,
	VERTEX_COLOR = 8,
	VERTEX_TEXCOORD = 16,
	VERTEX_GENERIC = 32
};

/**
 * Common attribute sizes.
 */
enum VERTEX_ATTRIB_SIZES
{
	ATTRIB_SIZE_1F = 1,
	ATTRIB_SIZE_2F = 2,
	ATTRIB_SIZE_VEC2 = 2,
	ATTRIB_SIZE_3F = 3,
	ATTRIB_SIZE_VEC3 = 3,
	ATTRIB_SIZE_4F = 4,
	ATTRIB_SIZE_VEC4 = 4,
	ATTRIB_SIZE_MAT3X3 = 9,
	ATTRIB_SIZE_MAT4x4 = 16
};

/**
 * Holds information about what type and size of data a single attribute
 * in a vertex buffer is for.
 */
struct VertexBufferAttribute
{
	/**
	 * The standard type of the attribute data. This will be VERTEX_GENERIC 
	 * if not a standard type (that is, when this attribute is not for the
	 * "primary" 2D/3D vertex position, texture coordinates, normals, etc).
	 */
	VERTEX_ATTRIBS standardType;

	/**
	 * The number of floating point value components that make up the
	 * attribute. e.g. 3 for a Vector3, 4 for a Color, etc.
	 */
	uint32_t size;

	/**
	 * The offset from the start of a vertex to where this attribute's data
	 * begins. Specified as the number of floating point components
	 * (Multiply by sizeof(float) to get the offset as the number of bytes).
	 */
	uint32_t offset;
};

#endif
