#include "../framework/log.h"

#include "eventlogger.h"
#include "event.h"
#include "eventmanager.h"

EventLogger::EventLogger(EventManager *eventManager)
{
	eventManager->AddListener(this, EVENT_TYPE_WILDCARD);
}

EventLogger::~EventLogger()
{
}

bool EventLogger::Handle(const Event *event)
{
	LOG_INFO("EVENTLOGGER", "Event \"%s\" occurred.\n", event->GetTypeOf());
	return false;
}
