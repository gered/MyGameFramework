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
	
	bool HasState(const stl::string &name) const;

	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnAppPause();
	void OnAppResume();
	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);

	bool IsTransitioning() const;
	bool IsEmpty() const;
	bool IsTop(const GameState *state) const;
	bool IsTransitioning(const GameState *state) const;

	GameState* GetTopState() const;
	GameState* GetTopNonOverlayState() const;

	int GetLastReturnValue() const                                              { return m_stateReturnValue; }
	bool HasLastReturnValue() const                                             { return m_hasStateReturnValue; }

private:
	void QueueForPush(StateInfo *newStateInfo);
	void QueueForSwap(StateInfo *newStateInfo, bool swapTopNonOverlay);

	StateInfo* GetTop() const;
	StateInfo* GetTopNonOverlay() const;
	StateInfoList::iterator GetTopNonOverlayItor();
	StateInfo* GetStateInfoFor(const GameState *state) const;

	void StartTopStatesTransitioningOut(bool pausing);
	void StartOnlyTopStateTransitioningOut(bool pausing);
	void CleanupInactiveStates();
	void CheckForFinishedStates();
	void ProcessQueues();
	void ResumeStatesIfNeeded();
	void UpdateTransitions(float delta);

	void TransitionOut(StateInfo *stateInfo, bool forPopping);
	void TransitionIn(StateInfo *stateInfo, bool forResuming);

	GameApp *m_gameApp;
	StateInfoList m_states;
	StateInfoQueue m_pushQueue;
	StateInfoQueue m_swapQueue;
	int m_stateReturnValue; 
	bool m_hasStateReturnValue;
	bool m_pushQueueHasOverlay;
	bool m_swapQueueHasOverlay;
	bool m_lastCleanedStatesWereAllOverlays;
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
	newStateInfo->isOverlay = true;
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
	bool isOverlay = currentTopStateInfo->isOverlay;

	T* newState = new T(m_gameApp, this);
	StateInfo *newStateInfo = new StateInfo(newState, name);
	newStateInfo->isOverlay = isOverlay;
	QueueForSwap(newStateInfo, false);
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
	QueueForSwap(newStateInfo, true);
	return newState;
}

inline bool StateManager::IsEmpty() const
{
	return (m_states.empty() && m_pushQueue.empty() && m_swapQueue.empty());
}

inline bool StateManager::IsTop(const GameState *state) const
{
	return GetTop()->gameState == state;
}

inline bool StateManager::IsTransitioning(const GameState *state) const
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

