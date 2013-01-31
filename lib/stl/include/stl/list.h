#ifndef __STL_LIST_H_INCLUDED__
#define __STL_LIST_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <list>
#elif defined(USE_EASTL_STL)
	#include <EASTL/list.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
