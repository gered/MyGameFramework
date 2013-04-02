#ifndef __EFFECTS_EFFECT_H_INCLUDED__
#define __EFFECTS_EFFECT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"

class RenderContext;

typedef const char* EFFECT_TYPE;

class Effect
{
public:
	TYPE_BASE(EFFECT_TYPE);
	
	Effect();
	virtual ~Effect();

	virtual void OnAdd()                                   {};
	virtual void OnRemove()                                {};
	virtual void OnAppGainFocus()                          {};
	virtual void OnAppLostFocus()                          {};
	virtual void OnAppPause()                              {};
	virtual void OnAppResume()                             {};
	virtual void OnLostContext()                           {};
	virtual void OnNewContext()                            {};
	virtual void OnRender(RenderContext *renderContext)    {};
	virtual void OnResize()                                {};
	virtual void OnUpdate(float delta)                     {};

	bool IsActive() const                                  { return m_isActive; }
	void MarkInactive();

private:
	bool m_isActive;
};

inline Effect::Effect()
{
	m_isActive = true;
}

inline Effect::~Effect()
{
}

inline void Effect::MarkInactive()
{
	m_isActive = false;
}

#endif

