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
	isTransitioning = FALSE;
	isTransitioningOut = FALSE;
	isTransitionStarting = FALSE;
	isInactive = FALSE;
	isBeingRemoved = FALSE;
	
	SetDescriptor();
}

ProcessInfo::ProcessInfo(GameProcess *process, const stl::string &name)
{
	ASSERT(process != NULL);
	this->process = process;
	this->name = name;
	isTransitioning = FALSE;
	isTransitioningOut = FALSE;
	isTransitionStarting = FALSE;
	isInactive = FALSE;
	isBeingRemoved = FALSE;
	
	SetDescriptor();
}

void ProcessInfo::SetDescriptor()
{
	m_descriptor = process->GetTypeOf();
	
	if (name.length() > 0)
		m_descriptor += "[" + name + "]";
}
