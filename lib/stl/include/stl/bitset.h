#ifndef __STL_BITSET_H_INCLUDED__
#define __STL_BITSET_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <bitset>
#elif defined(USE_EASTL_STL)
	#include <EASTL/bitset.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
