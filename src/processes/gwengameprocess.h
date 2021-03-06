#ifndef __PROCESSES_GWENGAMEPROCESS_H_INCLUDED__
#define __PROCESSES_GWENGAMEPROCESS_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/debug.h"
#include "../framework/util/typesystem.h"
#include "gameprocess.h"

class GwenGameProcessUIController;
class GameState;
class ProcessManager;
class RenderContext;
struct Event;

class GwenGameProcess : public GameProcess
{
public:
	TYPE_DEFINE(GAMEPROCESS_TYPE, "GwenGameProcess");

	GwenGameProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~GwenGameProcess();

	template<class T> void SetGwenHandler();
	
	virtual void OnAdd();
	virtual void OnRemove();
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
	GwenGameProcessUIController *m_gwenController;
	
	// this is only needed so that the GwenGameProcessUIController class can 
	// drive the lifecycle of it's parent GwenGameProcess object, thereby 
	// reducing the number of cases where we'll need to create a new subclass 
	// of GwenGameProcess to handle specific tasks
	friend class GwenGameProcessUIController;
};

template<class T>
void GwenGameProcess::SetGwenHandler()
{
	ASSERT(m_gwenController == NULL);
	T *gwenController = new T(this);
	m_gwenController = (GwenGameProcessUIController*)gwenController;
}

#endif

