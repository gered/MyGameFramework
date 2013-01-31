#ifndef __GAMEAPP_H_INCLUDED__
#define __GAMEAPP_H_INCLUDED__

#include "framework/common.h"
#include "framework/basegameapp.h"
#include "contexts/rendercontext.h"

class ContentCache;
class EventManager;
class StateManager;

class GameApp : public BaseGameApp
{
public:
	GameApp();
	virtual ~GameApp();

	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnAppPause();
	void OnAppResume();
	BOOL OnInit();
	void OnLoadGame();
	void OnLostContext();
	void OnNewContext();
	void OnRender();
	void OnResize();
	void OnUpdate(float delta);

	ContentCache* GetContentCache() const                  { return m_contentCache; }
	StateManager* GetStateManager() const                  { return m_stateManager; }
	EventManager* GetEventManager() const                  { return m_eventManager; }

	uint32_t GetScreenScale() const;

private:
	ContentCache *m_contentCache;
	RenderContext *m_renderContext;
	StateManager *m_stateManager;
	EventManager *m_eventManager;
};

#endif
