#include "../framework/debug.h"
#include "../framework/common.h"

#include "stateinfo.h"
#include "gamestate.h"
#include <stl/string.h>

StateInfo::StateInfo(GameState *gameState)
{
	STACK_TRACE;
	ASSERT(gameState != NULL);
	this->gameState = gameState;
	name = "";
	isOverlay = FALSE;
	isOverlayed = FALSE;
	isTransitioning = FALSE;
	isTransitioningOut = FALSE;
	isTransitionStarting = FALSE;
	isInactive = FALSE;
	isBeingPopped = FALSE;
	
	SetDescriptor();
}

StateInfo::StateInfo(GameState *gameState, const stl::string &name)
{
	STACK_TRACE;
	ASSERT(gameState != NULL);
	this->gameState = gameState;
	this->name = name;
	isOverlay = FALSE;
	isOverlayed = FALSE;
	isTransitioning = FALSE;
	isTransitioningOut = FALSE;
	isTransitionStarting = FALSE;
	isInactive = FALSE;
	isBeingPopped = FALSE;
	
	SetDescriptor();
}

void StateInfo::SetDescriptor()
{
	STACK_TRACE;
	m_descriptor = gameState->GetTypeOf();
	
	if (name.length() > 0)
		m_descriptor += "[" + name + "]";
}
