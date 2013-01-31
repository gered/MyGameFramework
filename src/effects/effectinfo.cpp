#include "../framework/debug.h"

#include "effectinfo.h"
#include "effect.h"

EffectInfo::EffectInfo(Effect *effect, BOOL isLocal)
{
	STACK_TRACE;
	ASSERT(effect != NULL);
	m_effect = effect;
	m_isLocal = isLocal;
}

EffectInfo::~EffectInfo()
{
	STACK_TRACE;
	SAFE_DELETE(m_effect);
}

