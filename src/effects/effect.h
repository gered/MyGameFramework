#ifndef __EFFECTS_EFFECT_H_INCLUDED__
#define __EFFECTS_EFFECT_H_INCLUDED__

#include "../framework/common.h"

class RenderContext;

typedef const char* EFFECT_TYPE;

class Effect
{
public:
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

	BOOL IsActive() const                                  { return m_isActive; }
	void MarkInactive();

	virtual EFFECT_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(EFFECT_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

private:
	BOOL m_isActive;
};

inline Effect::Effect()
{
	m_isActive = TRUE;
}

inline Effect::~Effect()
{
}

inline void Effect::MarkInactive()
{
	m_isActive = FALSE;
}

template<class T>
inline BOOL Effect::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Effect::Is(EFFECT_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Effect::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Effect::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

