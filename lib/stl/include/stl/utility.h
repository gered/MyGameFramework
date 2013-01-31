#ifndef __STL_UTILITY_H_INCLUDED__
#define __STL_UTILITY_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <utility>
#elif defined(USE_EASTL_STL)
	#include <EASTL/utility.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
