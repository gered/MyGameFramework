#ifndef __STL_SET_H_INCLUDED__
#define __STL_SET_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <set>
#elif defined(USE_EASTL_STL)
	#include <EASTL/set.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
