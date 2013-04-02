#ifndef __STATES_STATEINFO_H_INCLUDED__
#define __STATES_STATEINFO_H_INCLUDED__

#include "../framework/common.h"
#include <stl/string.h>

class GameState;

struct StateInfo
{
	StateInfo(GameState *gameState);
	StateInfo(GameState *gameState, const stl::string &name);

	GameState *gameState;
	stl::string name;
	bool isOverlay;
	bool isOverlayed;
	bool isTransitioning;
	bool isTransitioningOut;
	bool isTransitionStarting;
	bool isInactive;
	bool isBeingPopped;
	
	const stl::string& GetDescriptor() const                                    { return m_descriptor; }
	
private:
	void SetDescriptor();
	
	stl::string m_descriptor;
};

#endif

