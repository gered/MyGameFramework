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
	virtual void OnPause(bool dueToOverlay);
	virtual void OnResume(bool fromOverlay);
	virtual void OnAppGainFocus();
	virtual void OnAppLostFocus();
	virtual void OnAppPause();
	virtual void OnAppResume();
	virtual void OnLostContext();
	virtual void OnNewContext();
	virtual void OnRender(RenderContext *renderContext);
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	virtual bool OnTransition(float delta, bool isTransitioningOut, bool started);

	virtual bool Handle(const Event *event);

	GameApp* GetGameApp() const                                                 { return m_gameApp; }
	ProcessManager* GetProcessManager() const                                   { return m_processManager; }
	EffectManager* GetEffectManager() const                                     { return m_effectManager; }

	bool IsTransitioning() const;
	bool IsTopState() const;
	bool IsFinished() const                                                     { return m_isFinished; }
	bool HasReturnValue() const                                                 { return m_hasReturnValue; }
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
	bool m_isFinished;
	int m_returnValue;
	bool m_hasReturnValue;
};

#endif

