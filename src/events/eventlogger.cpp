#include "../framework/debug.h"
#include "../framework/log.h"

#include "eventlogger.h"
#include "event.h"
#include "eventmanager.h"

EventLogger::EventLogger(EventManager *eventManager)
	: EventListener()
{
	STACK_TRACE;
	eventManager->AddListener(this, EVENT_TYPE_WILDCARD);
}

EventLogger::~EventLogger()
{
	STACK_TRACE;
}

BOOL EventLogger::Handle(const Event *event)
{
	STACK_TRACE;
	LOG_INFO("EVENTLOGGER", "Event \"%s\" occurred.\n", event->GetTypeOf());
	return FALSE;
}
