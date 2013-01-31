#ifndef __STLCONFIG_NAMESPACEALIAS_H_INCLUDED__
#define __STLCONFIG_NAMESPACEALIAS_H_INCLUDED__

#ifdef USE_STD_STL
namespace stl = std;
#elif defined(USE_EASTL_STL)
namespace stl = eastl;
#else
#error Must define either USE_STD_STL or USE_EASTL_STL to choose an STL implementation.
#endif

#endif
