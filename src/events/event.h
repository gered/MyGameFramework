#ifndef __EVENTS_EVENT_H_INCLUDED__
#define __EVENTS_EVENT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"

typedef const char* EVENT_TYPE;

const EVENT_TYPE EVENT_TYPE_WILDCARD = "EventWildcard";

struct Event
{
	TYPE_BASE(EVENT_TYPE);
	
	virtual ~Event();
};

inline Event::~Event()
{
}

#endif
