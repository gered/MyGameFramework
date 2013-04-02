#ifndef __PROCESSES_PROCESSMANAGER_H_INCLUDED__
#define __PROCESSES_PROCESSMANAGER_H_INCLUDED__

#include "../framework/common.h"
#include "gameprocess.h"
#include "processinfo.h"
#include "../states/gamestate.h"
#include <stl/list.h>
#include <stl/string.h>

class RenderContext;

typedef stl::list<ProcessInfo*> ProcessInfoList;
typedef stl::list<ProcessInfo*> ProcessInfoQueue;

class ProcessManager
{
public:
	ProcessManager(GameState *gameState);
	virtual ~ProcessManager();

	template<class T> T* Add();
	template<class T> T* Add(const stl::string &name);
	void Remove(const stl::string &name);
	template<class T> void RemoveFirst();
	void RemoveAll();
	
	bool HasProcess(const stl::string &name) const;

	void OnPause(bool dueToOverlay);
	void OnResume(bool fromOverlay);
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
	bool IsTransitioning(const GameProcess *process) const;

private:
	void Queue(ProcessInfo *newProcessInfo);
	void StartTransitionOut(ProcessInfoList::iterator itor, bool forRemoval);

	ProcessInfoList::iterator GetProcessItorFor(const stl::string &name);
	ProcessInfoList::iterator GetProcessItorForFirstOf(GAMEPROCESS_TYPE processType);
	ProcessInfo* GetProcessInfoFor(const GameProcess *process) const;

	void CleanupInactiveProcesses();
	void CheckForFinishedProcesses();
	void ProcessQueue();
	void UpdateTransitions(float delta);

	GameState *m_gameState;
	ProcessInfoList m_processes;
	ProcessInfoQueue m_queue;
};

template<class T>
T* ProcessManager::Add()
{
	return Add<T>("");
}

template<class T>
T* ProcessManager::Add(const stl::string &name)
{
	T *newProcess = new T(m_gameState, this);
	ProcessInfo *newProcessInfo = new ProcessInfo(newProcess, name);
	Queue(newProcessInfo);
	return newProcess;
}

template<class T>
void ProcessManager::RemoveFirst()
{
	ProcessInfoList::iterator itor = GetProcessItorForFirstOf(T::GetType());
	StartTransitionOut(itor, true);
}

inline bool ProcessManager::IsEmpty() const
{
	return (m_processes.empty() && m_queue.empty());
}

inline bool ProcessManager::IsTransitioning(const GameProcess *process) const
{
	return GetProcessInfoFor(process)->isTransitioning;
}

#endif

