#ifndef __STATES_GWENGAMESTATE_H_INCLUDED__
#define __STATES_GWENGAMESTATE_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "../framework/debug.h"
#include "gamestate.h"

class GameApp;
class GwenGameStateUIController;
class StateManager;
class RenderContext;
struct Event;

class GwenGameState : public GameState
{
public:
	TYPE_DEFINE(GAMESTATE_TYPE, "GwenGameState");

	GwenGameState(GameApp *gameApp, StateManager *stateManager);
	virtual ~GwenGameState();

	template<class T> void SetGwenHandler();
	
	virtual void OnPush();
	virtual void OnPop();
	virtual void OnPause(bool dueToOverlay);
	virtual void OnResume(bool fromOverlay);
	virtual void OnLostContext();
	virtual void OnNewContext();
	virtual void OnRender(RenderContext *renderContext);
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	virtual bool OnTransition(float delta, bool isTransitioningOut, bool started);
	
	virtual bool Handle(const Event *event);
	
private:
	GwenGameStateUIController *m_gwenController;
	
	// this is only needed so that the GwenGameStateUIController class can 
	// drive the lifecycle of it's parent GwenGameState object, thereby 
	// reducing the number of cases where we'll need to create a new subclass 
	// of GwenGameState to handle specific tasks
	friend class GwenGameStateUIController;
};

template<class T>
void GwenGameState::SetGwenHandler()
{
	ASSERT(m_gwenController == NULL);
	T *gwenController = new T(this);
	m_gwenController = (GwenGameStateUIController*)gwenController;
}

#endif
