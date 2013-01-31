#ifndef __STL_VECTOR_H_INCLUDED__
#define __STL_VECTOR_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <vector>
#elif defined(USE_EASTL_STL)
	#include <EASTL/vector.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
