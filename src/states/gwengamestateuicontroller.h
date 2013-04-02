#ifndef __STATES_GWENGAMESTATEUICONTROLLERHANDLER_H_INCLUDED__
#define __STATES_GWENGAMESTATEUICONTROLLERHANDLER_H_INCLUDED__

#include "../framework/common.h"
#include "../events/event.h"
#include "gwengamestate.h"
#include <gwen_events.h>
#include <stl/string.h>

class RenderContext;
class StateManager;

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

class GwenGameStateUIController : public Gwen::Event::Handler
{
public:
	GwenGameStateUIController(GwenGameState *gameState);
	virtual ~GwenGameStateUIController();
	
	virtual void OnPush();
	virtual void OnPop();
	virtual void OnPause(bool dueToOverlay);
	virtual void OnResume(bool fromOverlay);
	virtual void OnLostContext();
	virtual void OnNewContext();
	virtual void OnRender(RenderContext *renderContext);
	virtual void OnResize();
	virtual void OnUpdate(float delta);
	virtual bool OnTransition(float delta, bool isTransitioningOut, bool started);
	
	template<class T> bool ListenFor();
	template<class T> bool StopListeningFor();
	bool ListenFor(EVENT_TYPE type);
	bool StopListeningFor(EVENT_TYPE type);
	
	virtual bool Handle(const Event *event);
	
protected:
	Gwen::Controls::Canvas* InitializeGwen(const stl::string &skinFilename, const stl::string &fontFilename, uint fontSize);
	void ResizeAndScaleCanvas();
	
	GwenGameState* GetGameState() const                                         { return m_gameState; }
	Gwen::Controls::Canvas* GetCanvas() const                                   { return m_canvas; }
	
	void EnableGwenInput(bool enable);
	
	float GetAlpha() const                                                      { return m_alpha; }
	float GetScale() const                                                      { return m_scale; }
	void SetAlpha(float alpha);
	void SetScale(float scale);
	
	// these are for convenience and simply just call the equivalent protected
	// member functions on the parent game state so that we can avoid having
	// to create a new subclass of GwenGameState for each subclass of 
	// GwenGameStateUIController just to make this all work
	
	StateManager* GetStateManager() const;
	void SetStateFinished();
	void SetStateFinished(int returnValue);
	
private:
	GwenGameState *m_gameState;
	
	Gwen::Controls::Canvas *m_canvas;
	Gwen::Input::InputProcessor *m_inputProcessor;
	Gwen::Renderer::SpriteBatchRenderer *m_renderer;
	Gwen::Skin::TexturedBase *m_skin;
	float m_alpha;
	float m_scale;
};

template<class T>
bool GwenGameStateUIController::ListenFor()
{
	return m_gameState->ListenFor<T>();
}

template<class T>
bool GwenGameStateUIController::StopListeningFor()
{
	return m_gameState->StopListeningFor<T>();
}

inline bool GwenGameStateUIController::ListenFor(EVENT_TYPE type)
{
	return m_gameState->ListenFor(type);
}

inline bool GwenGameStateUIController::StopListeningFor(EVENT_TYPE type)
{
	return m_gameState->StopListeningFor(type);
}

#endif
