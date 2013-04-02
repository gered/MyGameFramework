#ifndef __EFFECTS_EFFECTMANAGER_H_INCLUDED__
#define __EFFECTS_EFFECTMANAGER_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/debug.h"
#include <stl/map.h>

#include "effect.h"
#include "effectinfo.h"

class RenderContext;

typedef stl::map<EFFECT_TYPE, EffectInfo*> EffectList;
typedef stl::map<EFFECT_TYPE, EffectInfo*>::iterator EffectListItor;
typedef stl::map<EFFECT_TYPE, EffectInfo*>::const_iterator EffectListConstItor;

class EffectManager
{
public:
	EffectManager();
	virtual ~EffectManager();

	template<class T> T* Get() const;
	template<class T> T* Add(bool isLocalEffect = true);
	template<class T> void Remove();
	void RemoveAll();

	void OnAppGainFocus();
	void OnAppLostFocus();
	void OnAppPause();
	void OnAppResume();
	void OnLostContext();
	void OnNewContext();
	void OnRenderLocal(RenderContext *renderContext);
	void OnRenderGlobal(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);

private:
	void Add(EFFECT_TYPE type, EffectInfo *effectInfo);
	void Remove(EffectListItor itor);

	EffectList m_effects;
	uint m_numLocalEffects;
	uint m_numGlobalEffects;
};

template<class T>
T* EffectManager::Get() const
{
	EffectListConstItor itor = m_effects.find(T::GetType());
	if (itor == m_effects.end())
		return NULL;
	else
		return (T*)itor->second->GetEffect();
}

template<class T>
T* EffectManager::Add(bool isLocalEffect)
{
	if (Get<T>() != NULL)
		return NULL;

	T* effect = new T();
	EffectInfo *newEffectInfo = new EffectInfo(effect, isLocalEffect);
	Add(T::GetType(), newEffectInfo);

	return effect;
}

template<class T>
void EffectManager::Remove()
{
	EffectListItor itor = m_effects.find(T::GetType());
	ASSERT(itor != m_effects.end());

	Remove(itor);
}

#endif

