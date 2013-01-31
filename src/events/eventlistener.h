#ifndef __EVENTS_EVENTLISTENER_H_INCLUDED__
#define __EVENTS_EVENTLISTENER_H_INCLUDED__

#include "../framework/common.h"

struct Event;

class EventListener
{
public:
	virtual BOOL Handle(const Event *event) = 0;
};

#endif

