#ifndef __EVENTS_EVENTLOGGER_H_INCLUDED__
#define __EVENTS_EVENTLOGGER_H_INCLUDED__

#include "../framework/common.h"
#include "eventlistener.h"

class EventManager;
struct Event;

class EventLogger : public EventListener
{
public:
	EventLogger(EventManager *eventManager);
	virtual ~EventLogger();

	bool Handle(const Event *event);
};

#endif
