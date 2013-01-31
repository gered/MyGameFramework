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
	BOOL isOverlay;
	BOOL isOverlayed;
	BOOL isTransitioning;
	BOOL isTransitioningOut;
	BOOL isTransitionStarting;
	BOOL isInactive;
	BOOL isBeingPopped;
	
	const stl::string& GetDescriptor() const               { return m_descriptor; }
	
private:
	void SetDescriptor();
	
	stl::string m_descriptor;
};

#endif

