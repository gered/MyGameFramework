#include "../framework/debug.h"

#include "effectmanager.h"
#include "effect.h"

EffectManager::EffectManager()
{
	STACK_TRACE;
	m_numLocalEffects = 0;
	m_numGlobalEffects = 0;
}

EffectManager::~EffectManager()
{
	STACK_TRACE;
	RemoveAll();
}

void EffectManager::RemoveAll()
{
	STACK_TRACE;
	EffectListItor itor = m_effects.begin();
	while (itor != m_effects.end())
		Remove(itor++);
}

void EffectManager::Add(EFFECT_TYPE type, EffectInfo *effectInfo)
{
	STACK_TRACE;
	ASSERT(effectInfo != NULL);
	m_effects.insert(EffectList::value_type(type, effectInfo));
	effectInfo->GetEffect()->OnAdd();

	if (effectInfo->IsLocal())
		++m_numLocalEffects;
	else
		++m_numGlobalEffects;
}

void EffectManager::Remove(EffectListItor itor)
{
	STACK_TRACE;
	if (itor->second->IsLocal())
		--m_numLocalEffects;
	else
		--m_numGlobalEffects;

	itor->second->GetEffect()->OnRemove();
	SAFE_DELETE(itor->second);
	m_effects.erase(itor);
}

void EffectManager::OnAppGainFocus()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnAppGainFocus();
}

void EffectManager::OnAppLostFocus()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnAppLostFocus();
}

void EffectManager::OnAppPause()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnAppPause();
}

void EffectManager::OnAppResume()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnAppResume();
}

void EffectManager::OnLostContext()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnLostContext();
}

void EffectManager::OnNewContext()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnNewContext();
}

void EffectManager::OnRenderLocal(RenderContext *renderContext)
{
	STACK_TRACE;
	if (m_numLocalEffects == 0)
		return;

	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
	{
		if (itor->second->IsLocal())
			itor->second->GetEffect()->OnRender(renderContext);
	}
}

void EffectManager::OnRenderGlobal(RenderContext *renderContext)
{
	STACK_TRACE;
	if (m_numGlobalEffects == 0)
		return;

	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
	{
		if (!itor->second->IsLocal())
			itor->second->GetEffect()->OnRender(renderContext);
	}
}

void EffectManager::OnResize()
{
	STACK_TRACE;
	for (EffectListItor itor = m_effects.begin(); itor != m_effects.end(); ++itor)
		itor->second->GetEffect()->OnResize();
}

void EffectManager::OnUpdate(float delta)
{
	STACK_TRACE;
	EffectListItor itor = m_effects.begin();
	while (itor != m_effects.end())
	{
		Effect *effect = itor->second->GetEffect();
		if (!effect->IsActive())
		{
			Remove(itor++);
		}
		else
		{
			effect->OnUpdate(delta);
			++itor;
		}
	}
}

