#ifndef __STATES_GAMESTATE_H_INCLUDED__
#define __STATES_GAMESTATE_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "../events/eventlistenerex.h"

typedef const char* GAMESTATE_TYPE;

class EffectManager;
class GameApp;
class ProcessManager;
class RenderContext;
class StateManager;
struct Event;

class GameState : public EventListenerEx
{
public:
	TYPE_BASE(GAMESTATE_TYPE);
	
	GameState(GameApp *gameApp, StateManager *stateManager);
	virtual ~GameState();

	virtual void OnPush();
	virtual void OnPop();
	virtual void OnPause(BOOL dueToOverlay);
	virtual void OnResume(BOOL fromOverlay);
	virtual void OnAppGainFocus();
	virtual void OnAppLostFocus();
	virtual void OnAppPause();
	virtual void OnAppResume();
	virtual void OnLostContext();
	virtual void OnNewContext();
	virtual void OnRender(RenderContext *renderContext);
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	virtual BOOL OnTransition(float delta, BOOL isTransitioningOut, BOOL started);

	virtual BOOL Handle(const Event *event);

	GameApp* GetGameApp() const                                                 { return m_gameApp; }
	ProcessManager* GetProcessManager() const                                   { return m_processManager; }
	EffectManager* GetEffectManager() const                                     { return m_effectManager; }

	BOOL IsTransitioning() const;
	BOOL IsTopState() const;
	BOOL IsFinished() const                                                     { return m_isFinished; }
	BOOL HasReturnValue() const                                                 { return m_hasReturnValue; }
	int GetReturnValue() const                                                  { return m_returnValue; }

protected:
	StateManager* GetStateManager() const                                       { return m_stateManager; }

	void SetFinished();
	void SetFinished(int returnValue);

private:
	GameApp *m_gameApp;
	StateManager *m_stateManager;
	ProcessManager *m_processManager;
	EffectManager *m_effectManager;
	BOOL m_isFinished;
	int m_returnValue;
	BOOL m_hasReturnValue;
};

#endif

