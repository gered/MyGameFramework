#ifndef __STL_STRING_H_INCLUDED__
#define __STL_STRING_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <string>
#elif defined(USE_EASTL_STL)
	#include <EASTL/string.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
