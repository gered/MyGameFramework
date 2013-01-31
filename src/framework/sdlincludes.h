#ifdef SDL
#ifndef __FRAMEWORK_SDLINCLUDES_H_INCLUDED__
#define __FRAMEWORK_SDLINCLUDES_H_INCLUDED__

// this solely exists because MSVC is usually the only compiler where I *may* have
// SDL headers in a non-standard spot
#ifdef _MSC_VER
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif

#endif
#endif