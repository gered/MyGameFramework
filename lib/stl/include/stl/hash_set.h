#ifndef __STL_HASH_SET_H_INCLUDED__
#define __STL_HASH_SET_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#ifdef _MSC_VER
		#include <hash_set>
	#elif defined(__GNUC__)
		#ifdef __S3E__
			#include <hash_set>
		#else
			#include <ext/hash_set>
		#endif
	#else
		#error Unknown header location for hash_set.
	#endif
#elif defined(USE_EASTL_STL)
	#include <EASTL/hash_set.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
