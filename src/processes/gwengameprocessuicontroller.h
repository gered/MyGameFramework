#ifndef __STATES_GWENGAMEPROCESSUICONTROLLERHANDLER_H_INCLUDED__
#define __STATES_GWENGAMEPROCESSUICONTROLLERHANDLER_H_INCLUDED__

#include "../framework/common.h"
#include "../events/event.h"
#include "gwengameprocess.h"
#include <gwen_events.h>
#include <stl/string.h>

class GameState;
class RenderContext;
class ProcessManager;

namespace Gwen
{
	namespace Controls
	{
		class Canvas;
	}
	namespace Input
	{
		class InputProcessor;
	}
	namespace Renderer
	{
		class SpriteBatchRenderer;
	}
	namespace Skin
	{
		class TexturedBase;
	}
}

class GwenGameProcessUIController : public Gwen::Event::Handler
{
public:
	GwenGameProcessUIController(GwenGameProcess *gameState);
	virtual ~GwenGameProcessUIController();
	
	virtual void OnAdd();
	virtual void OnRemove();
	virtual void OnPause(BOOL dueToOverlay);
	virtual void OnResume(BOOL fromOverlay);
	virtual void OnLostContext();
	virtual void OnNewContext();
	virtual void OnRender(RenderContext *renderContext);
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	virtual BOOL OnTransition(float delta, BOOL isTransitioningOut, BOOL started);
	
	template<class T> BOOL ListenFor();
	template<class T> BOOL StopListeningFor();
	BOOL ListenFor(EVENT_TYPE type);
	BOOL StopListeningFor(EVENT_TYPE type);
	
	virtual BOOL Handle(const Event *event);
	
protected:
	Gwen::Controls::Canvas* InitializeGwen(const stl::string &skinFilename, const stl::string &fontFilename, uint fontSize);
	void ResizeAndScaleCanvas();
	
	GwenGameProcess* GetGameProcess() const                                     { return m_gameProcess; }
	Gwen::Controls::Canvas* GetCanvas() const                                   { return m_canvas; }
	
	void EnableGwenInput(BOOL enable);

	float GetAlpha() const                                                      { return m_alpha; }
	float GetScale() const                                                      { return m_scale; }
	void SetAlpha(float alpha);
	void SetScale(float scale);
	
	// these are for convenience and simply just call the equivalent protected
	// member functions on the parent game state so that we can avoid having
	// to create a new subclass of GwenGameState for each subclass of 
	// GwenGameStateUIController just to make this all work
	
	ProcessManager* GetProcessManager() const;
	GameState* GetGameState() const;
	void SetProcessFinished();
	
private:
	GwenGameProcess *m_gameProcess;
	
	Gwen::Controls::Canvas *m_canvas;
	Gwen::Input::InputProcessor *m_inputProcessor;
	Gwen::Renderer::SpriteBatchRenderer *m_renderer;
	Gwen::Skin::TexturedBase *m_skin;
	float m_alpha;
	float m_scale;
};

template<class T>
BOOL GwenGameProcessUIController::ListenFor()
{
	return m_gameProcess->ListenFor<T>();
}

template<class T>
BOOL GwenGameProcessUIController::StopListeningFor()
{
	return m_gameProcess->StopListeningFor<T>();
}

inline BOOL GwenGameProcessUIController::ListenFor(EVENT_TYPE type)
{
	return m_gameProcess->ListenFor(type);
}

inline BOOL GwenGameProcessUIController::StopListeningFor(EVENT_TYPE type)
{
	return m_gameProcess->StopListeningFor(type);
}

#endif
