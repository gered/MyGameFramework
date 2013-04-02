#include "../framework/debug.h"
#include "../framework/common.h"

#include "processinfo.h"
#include "gameprocess.h"
#include <stl/string.h>

ProcessInfo::ProcessInfo(GameProcess *process)
{
	ASSERT(process != NULL);
	this->process = process;
	name = "";
	isTransitioning = false;
	isTransitioningOut = false;
	isTransitionStarting = false;
	isInactive = false;
	isBeingRemoved = false;
	
	SetDescriptor();
}

ProcessInfo::ProcessInfo(GameProcess *process, const stl::string &name)
{
	ASSERT(process != NULL);
	this->process = process;
	this->name = name;
	isTransitioning = false;
	isTransitioningOut = false;
	isTransitionStarting = false;
	isInactive = false;
	isBeingRemoved = false;
	
	SetDescriptor();
}

void ProcessInfo::SetDescriptor()
{
	m_descriptor = process->GetTypeOf();
	
	if (name.length() > 0)
		m_descriptor += "[" + name + "]";
}
