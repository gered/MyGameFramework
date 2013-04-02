#ifndef __PROCESSES_GAMEPROCESS_H_INCLUDED__
#define __PROCESSES_GAMEPROCESS_H_INCLUDED__

#include "../framework/common.h"

#include "../states/gamestate.h"
#include "../events/eventlistenerex.h"

typedef const char* GAMEPROCESS_TYPE;

class GameApp;
class ProcessManager;
class RenderContext;
struct Event;

class GameProcess : public EventListenerEx
{
public:
	GameProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~GameProcess();

	virtual void OnAdd();
	virtual void OnRemove();
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

	virtual GAMEPROCESS_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(GAMEPROCESS_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

	GameApp* GetGameApp() const                                                 { return m_gameState->GetGameApp(); }

	BOOL IsFinished() const                                                     { return m_finished; }
	BOOL IsTransitioning() const;

protected:
	GameState* GetGameState() const                                             { return m_gameState; }
	ProcessManager* GetProcessManager() const                                   { return m_processManager; }

	void SetFinished();

private:
	GameState* m_gameState;
	ProcessManager *m_processManager;
	BOOL m_finished;
};

template<class T>
inline BOOL GameProcess::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL GameProcess::Is(GAMEPROCESS_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* GameProcess::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* GameProcess::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

