#ifndef __PROCESSES_PROCESSINFO_H_INCLUDED__
#define __PROCESSES_PROCESSINFO_H_INCLUDED__

#include "../framework/common.h"
#include <stl/string.h>

class GameProcess;

struct ProcessInfo
{
	ProcessInfo(GameProcess *process);
	ProcessInfo(GameProcess *process, const stl::string &name);

	GameProcess *process;
	stl::string name;
	BOOL isTransitioning;
	BOOL isTransitioningOut;
	BOOL isTransitionStarting;
	BOOL isInactive;
	BOOL isBeingRemoved;

	const stl::string& GetDescriptor() const                                    { return m_descriptor; }
	
private:
	void SetDescriptor();
	
	stl::string m_descriptor;
};

#endif

