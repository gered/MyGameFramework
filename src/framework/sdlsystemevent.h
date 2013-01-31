#ifdef SDL
#ifndef __FRAMEWORK_SDLSYSTEMEVENT_H_INCLUDED__
#define __FRAMEWORK_SDLSYSTEMEVENT_H_INCLUDED__

#include "osevent.h"
#include "sdlincludes.h"

struct SDLSystemEvent : OSEvent
{
	SDL_Event *event;
};

#endif
#endif
