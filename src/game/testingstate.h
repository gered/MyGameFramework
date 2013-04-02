#ifndef __GAME_TESTINGSTATE_H_INCLUDED__
#define __GAME_TESTINGSTATE_H_INCLUDED__

#include "../states/gamestate.h"
#include "../framework/util/typesystem.h"

class FreeCamera;
class GameApp;
class Grid;
class RenderContext;
class StateManager;

class TestingState : public GameState
{
public:
	TYPE_DEFINE(GAMESTATE_TYPE, "TestingState");

	TestingState(GameApp *gameApp, StateManager *stateManager);
	virtual ~TestingState();

	void OnPush();
	void OnPop();
	void OnPause(bool dueToOverlay);
	void OnResume(bool fromOverlay);
	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);
	bool OnTransition(float delta, bool isTransitioningOut, bool started);

private:
	Grid *m_grid;
	FreeCamera *m_camera;
};

#endif

