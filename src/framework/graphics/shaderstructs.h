#ifndef __FRAMEWORK_GRAPHICS_SHADERSTRUCTS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SHADERSTRUCTS_H_INCLUDED__

#include "../common.h"
#include "vertexattribs.h"

#include <stl/map.h>
#include <stl/string.h>

/**
 * Metadata about a shader uniform.
 */
struct ShaderUniform
{
	uint32_t location;
	uint32_t type;
	uint32_t size;
};

/**
 * Metadata about a shader attribute.
 */
struct ShaderAttribute
{
	uint32_t location;
	uint32_t type;
	uint32_t size;
	BOOL isTypeBound;
};

/**
 * Shader attribute to vertex buffer attribute mapping information.
 */
struct ShaderAttributeMapInfo
{
	BOOL usesStandardType;
	VERTEX_ATTRIBS standardType;
	uint32_t attribIndex;
};

/**
 * Type enum for temporarily cached shader uniform values.
 */
enum CACHED_SHADER_UNIFORM_TYPE
{
	CACHED_UNIFORM_1F,
	CACHED_UNIFORM_1I,
	CACHED_UNIFORM_2F,
	CACHED_UNIFORM_2I,
	CACHED_UNIFORM_3F,
	CACHED_UNIFORM_3I,
	CACHED_UNIFORM_4F,
	CACHED_UNIFORM_4I,
	CACHED_UNIFORM_9F,
	CACHED_UNIFORM_16F
};

/**
 * Holds type and value for temporarily cached shader uniform values.
 */
struct CachedShaderUniform
{
	CACHED_SHADER_UNIFORM_TYPE type;
	union
	{
		struct
		{
			float x;
		} f1;
		struct
		{
			int32_t x;
		} i1;
		struct
		{
			float x;
			float y;
		} f2;
		struct
		{
			int32_t x;
			int32_t y;
		} i2;
		struct
		{
			float x;
			float y;
			float z;
		} f3;
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
		} i3;
		struct
		{
			float x;
			float y;
			float z;
			float w;
		} f4;
		struct
		{
			int32_t x;
			int32_t y;
			int32_t z;
			int32_t w;
		} i4;
		struct
		{
			float m[9];
		} f9;
		struct
		{
			float m[16];
		} f16;
	};
};

struct CachedShaderArrayUniform
{
	CACHED_SHADER_UNIFORM_TYPE type;
	uint32_t count;
	float *values;
	
	CachedShaderArrayUniform()
	{
		values = NULL;
	}
	
	~CachedShaderArrayUniform()
	{
		SAFE_DELETE_ARRAY(values);
	}
};

typedef stl::map<stl::string, ShaderUniform> ShaderUniformMap;
typedef stl::map<stl::string, ShaderAttribute> ShaderAttributeMap;
typedef stl::map<stl::string, CachedShaderUniform> CachedShaderUniformMap;

#endif
