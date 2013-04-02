#ifndef __STATES_GAMESTATE_H_INCLUDED__
#define __STATES_GAMESTATE_H_INCLUDED__

#include "../framework/common.h"
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

	virtual GAMESTATE_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(GAMESTATE_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

	GameApp* GetGameApp() const                                                 { return m_gameApp; }
	ProcessManager* GetProcessManager() const                                   { return m_processManager; }
	EffectManager* GetEffectManager() const                                     { return m_effectManager; }

	BOOL IsTransitioning() const;
	BOOL IsTopState() const;
	BOOL IsFinished() const                                                     { return m_isFinished; }
	BOOL HasReturnValue() const                                                 { return m_hasReturnValue; }
	uint32_t GetReturnValue() const                                             { return m_returnValue; }

protected:
	StateManager* GetStateManager() const                                       { return m_stateManager; }

	void SetFinished();
	void SetFinished(uint32_t returnValue);

private:
	GameApp *m_gameApp;
	StateManager *m_stateManager;
	ProcessManager *m_processManager;
	EffectManager *m_effectManager;
	BOOL m_isFinished;
	uint32_t m_returnValue;
	BOOL m_hasReturnValue;
};

template<class T>
inline BOOL GameState::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL GameState::Is(GAMESTATE_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* GameState::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* GameState::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

