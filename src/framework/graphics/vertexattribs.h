#ifndef __FRAMEWORK_GRAPHICS_VERTEXATTRIBS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VERTEXATTRIBS_H_INCLUDED__

#include "../common.h"

/**
 * Constant values to refer to the standard vertex attribute types.
 * High byte of each value = standard type bitmask
 * Low byte of each value = number of floats needed
 */
enum VERTEX_STANDARD_ATTRIBS
{
	VERTEX_STD_POS_2D = 0x0102,
	VERTEX_STD_POS_3D = 0x0203,
	VERTEX_STD_NORMAL = 0x0403,
	VERTEX_STD_COLOR = 0x0804,
	VERTEX_STD_TEXCOORD = 0x1002
};

/**
 * Constant values for vertex attributes.
 */
enum VERTEX_ATTRIBS
{
	VERTEX_POS_2D = VERTEX_STD_POS_2D,
	VERTEX_POS_3D = VERTEX_STD_POS_3D,
	VERTEX_NORMAL = VERTEX_STD_NORMAL,
	VERTEX_COLOR = VERTEX_STD_COLOR,
	VERTEX_TEXCOORD = VERTEX_STD_TEXCOORD,
	
	VERTEX_F1 = 1,
	VERTEX_F2 = 2,
	VERTEX_F3 = 3,
	VERTEX_F4 = 4,
	VERTEX_V2 = 2,
	VERTEX_V3 = 3,
	VERTEX_V4 = 4,
	VERTEX_M3 = 9,
	VERTEX_M4 = 16
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
	uint size;

	/**
	 * The offset from the start of a vertex to where this attribute's data
	 * begins. Specified as the number of floating point components
	 * (Multiply by sizeof(float) to get the offset as the number of bytes).
	 */
	uint offset;
};

#endif
