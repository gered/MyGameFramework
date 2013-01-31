#ifndef __STL_ALGORITHM_H_INCLUDED__
#define __STL_ALGORITHM_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <algorithm>
#elif defined(USE_EASTL_STL)
	#include <EASTL/algorithm.h>
	#include <EASTL/sort.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
