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
	Gwen::Controls::Canvas* InitializeGwen(const stl::string &skinFilename, const stl::string &fontFilename, uint8_t fontSize);
	void ResizeAndScaleCanvas();
	
	GwenGameState* GetGameState() const                                         { return m_gameState; }
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
	
	StateManager* GetStateManager() const;
	void SetStateFinished();
	void SetStateFinished(uint32_t returnValue);
	
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
BOOL GwenGameStateUIController::ListenFor()
{
	return m_gameState->ListenFor<T>();
}

template<class T>
BOOL GwenGameStateUIController::StopListeningFor()
{
	return m_gameState->StopListeningFor<T>();
}

inline BOOL GwenGameStateUIController::ListenFor(EVENT_TYPE type)
{
	return m_gameState->ListenFor(type);
}

inline BOOL GwenGameStateUIController::StopListeningFor(EVENT_TYPE type)
{
	return m_gameState->StopListeningFor(type);
}

#endif
