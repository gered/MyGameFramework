#ifndef __STL_MAP_H_INCLUDED__
#define __STL_MAP_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <map>
#elif defined(USE_EASTL_STL)
	#include <EASTL/map.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
