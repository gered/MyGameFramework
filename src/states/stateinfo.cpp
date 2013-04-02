#include "../framework/debug.h"
#include "../framework/common.h"

#include "stateinfo.h"
#include "gamestate.h"
#include <stl/string.h>

StateInfo::StateInfo(GameState *gameState)
{
	ASSERT(gameState != NULL);
	this->gameState = gameState;
	name = "";
	isOverlay = false;
	isOverlayed = false;
	isTransitioning = false;
	isTransitioningOut = false;
	isTransitionStarting = false;
	isInactive = false;
	isBeingPopped = false;
	
	SetDescriptor();
}

StateInfo::StateInfo(GameState *gameState, const stl::string &name)
{
	ASSERT(gameState != NULL);
	this->gameState = gameState;
	this->name = name;
	isOverlay = false;
	isOverlayed = false;
	isTransitioning = false;
	isTransitioningOut = false;
	isTransitionStarting = false;
	isInactive = false;
	isBeingPopped = false;
	
	SetDescriptor();
}

void StateInfo::SetDescriptor()
{
	m_descriptor = gameState->GetTypeOf();
	
	if (name.length() > 0)
		m_descriptor += "[" + name + "]";
}
