#ifndef __STL_ITERATOR_H_INCLUDED__
#define __STL_ITERATOR_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#include <iterator>
#elif defined(USE_EASTL_STL)
	#include <EASTL/iterator.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
