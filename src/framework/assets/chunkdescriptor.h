#ifndef __FRAMEWORK_ASSETS_CHUNKDESCRIPTOR_H_INCLUDED__
#define __FRAMEWORK_ASSETS_CHUNKDESCRIPTOR_H_INCLUDED__

#include "../common.h"
#include <stl/map.h>
#include <stl/string.h>

struct ChunkDescriptor
{
	uint start;
	uint length;
};

typedef stl::map<stl::string, ChunkDescriptor> ChunkMap;

#endif
