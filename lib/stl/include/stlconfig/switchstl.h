#ifndef __STLCONFIG_SWITCHSTL_H_INCLUDED__
#define __STLCONFIG_SWITCHSTL_H_INCLUDED__

#if defined(USE_STD_STL) && defined(USE_EASTL_STL)
	#error USE_STL_STL and USE_EASTL_STL cannot both be defined.
#endif

// pick a default STL if an STL implementation define doesn't exist
#if !defined(USE_STD_STL) && !defined(USE_EASTL_STL)
	#ifdef DEBUG
		// use the standard STL for debug builds for those debuggers which
		// have decent debugger visualizers for STL containers (which typically
		// will only work out-of-the-box with the standard STL)
		#define USE_STD_STL
	#else
		// for all other cases, use EASTL by default (this would be release
		// builds, or where the debugger being used doesn't provide any kind of
		// debugger assistance for STL containers at all and so it doesn't 
		// matter which one is used for debugging purposes)
		#define USE_EASTL_STL
	#endif
#endif

#endif
