#ifndef __STL_MEMORY_H_INCLUDED__
#define __STL_MEMORY_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <memory>
#elif defined(USE_EASTL_STL)
	#include <EASTL/memory.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
