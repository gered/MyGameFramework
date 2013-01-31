#ifndef __STL_HASH_MAP_H_INCLUDED__
#define __STL_HASH_MAP_H_INCLUDED__

#include <stlconfig/switchstl.h>

#ifdef USE_STD_STL
	#ifdef _MSC_VER
		#include <hash_map>
	#elif defined(__GNUC__)
		#ifdef __S3E__
			#include <hash_map>
		#else
			#include <ext/hash_map>
		#endif
	#else
		#error Unknown header location for hash_map.
	#endif
#elif defined(USE_EASTL_STL)
	#include <EASTL/hash_map.h>
#endif

#include <stlconfig/namespacealias.h>

#endif
