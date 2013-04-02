#ifndef __STATES_STATEMANAGER_H_INCLUDED__
#define __STATES_STATEMANAGER_H_INCLUDED__

#include "../framework/common.h"
#include "gamestate.h"
#include "stateinfo.h"
#include <stl/list.h>
#include <stl/string.h>

class GameApp;
class RenderContext;

typedef stl::list<StateInfo*> StateInfoList;
typedef stl::list<StateInfo*> StateInfoQueue;

class StateManager
{
public:
	StateManager(GameApp *gameApp);
	virtual ~StateManager();

	template<class T> T* Push();
	template<class T> T* Push(const stl::string &name);
	template<class T> T* Overlay();
	template<class T> T* Overlay(const stl::string &name);
	template<class T> T* SwapTopWith();
	template<class T> T* SwapTopWith(const stl::string &name);
	template<class T> T* SwapTopNonOverlayWith();
	template<class T> T* SwapTopNonOverlayWith(const stl::string &name);
	void Pop();
	void PopTopNonOverlay();
	
	BOOL HasState(const stl::string &name) const;

	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnAppPause();
	void OnAppResume();
	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);

	BOOL IsTransitioning() const;
	BOOL IsEmpty() const;
	BOOL IsTop(const GameState *state) const;
	BOOL IsTransitioning(const GameState *state) const;

	GameState* GetTopState() const;
	GameState* GetTopNonOverlayState() const;

	uint32_t GetLastReturnValue() const                                         { return m_stateReturnValue; }
	BOOL HasLastReturnValue() const                                             { return m_hasStateReturnValue; }

private:
	void QueueForPush(StateInfo *newStateInfo);
	void QueueForSwap(StateInfo *newStateInfo, BOOL swapTopNonOverlay);

	StateInfo* GetTop() const;
	StateInfo* GetTopNonOverlay() const;
	StateInfoList::iterator GetTopNonOverlayItor();
	StateInfo* GetStateInfoFor(const GameState *state) const;

	void StartTopStatesTransitioningOut(BOOL pausing);
	void StartOnlyTopStateTransitioningOut(BOOL pausing);
	void CleanupInactiveStates();
	void CheckForFinishedStates();
	void ProcessQueues();
	void ResumeStatesIfNeeded();
	void UpdateTransitions(float delta);

	void TransitionOut(StateInfo *stateInfo, BOOL forPopping);
	void TransitionIn(StateInfo *stateInfo, BOOL forResuming);

	GameApp *m_gameApp;
	StateInfoList m_states;
	StateInfoQueue m_pushQueue;
	StateInfoQueue m_swapQueue;
	uint32_t m_stateReturnValue; 
	BOOL m_hasStateReturnValue;
	BOOL m_pushQueueHasOverlay;
	BOOL m_swapQueueHasOverlay;
	BOOL m_lastCleanedStatesWereAllOverlays;
};

template<class T>
T* StateManager::Push()
{
	return Push<T>("");
}

template<class T>
T* StateManager::Push(const stl::string &name)
{
	T *newState = new T(m_gameApp, this);
	StateInfo *newStateInfo = new StateInfo(newState, name);
	QueueForPush(newStateInfo);
	return newState;
};

template<class T>
T* StateManager::Overlay()
{
	return Overlay<T>("");
}

template<class T>
T* StateManager::Overlay(const stl::string &name)
{
	T* newState = new T(m_gameApp, this);
	StateInfo *newStateInfo = new StateInfo(newState, name);
	newStateInfo->isOverlay = TRUE;
	QueueForPush(newStateInfo);
	return newState;
}

template<class T>
T* StateManager::SwapTopWith()
{
	return SwapTopWith<T>("");
}

template<class T>
T* StateManager::SwapTopWith(const stl::string &name)
{
	// figure out if the current top state is an overlay or not. use that
	// same setting for the new state that is to be swapped in
	StateInfo *currentTopStateInfo = GetTop();
	BOOL isOverlay = currentTopStateInfo->isOverlay;

	T* newState = new T(m_gameApp, this);
	StateInfo *newStateInfo = new StateInfo(newState, name);
	newStateInfo->isOverlay = isOverlay;
	QueueForSwap(newStateInfo, FALSE);
	return newState;
}

template<class T>
T* StateManager::SwapTopNonOverlayWith()
{
	return SwapTopNonOverlayWith<T>("");
}

template<class T>
T* StateManager::SwapTopNonOverlayWith(const stl::string &name)
{
	T *newState = new T(m_gameApp, this);
	StateInfo *newStateInfo = new StateInfo(newState, name);
	QueueForSwap(newStateInfo, TRUE);
	return newState;
}

inline BOOL StateManager::IsEmpty() const
{
	return (m_states.empty() && m_pushQueue.empty() && m_swapQueue.empty());
}

inline BOOL StateManager::IsTop(const GameState *state) const
{
	return GetTop()->gameState == state;
}

inline BOOL StateManager::IsTransitioning(const GameState *state) const
{
	return GetStateInfoFor(state)->isTransitioning;
}

inline GameState* StateManager::GetTopState() const
{
	return GetTop()->gameState;
}

inline GameState* StateManager::GetTopNonOverlayState() const
{
	return GetTopNonOverlay()->gameState;
}

inline StateInfo* StateManager::GetTop() const
{
	if (m_states.empty())
		return NULL;
	else
		return m_states.back();
}

#endif

