#include "../framework/debug.h"

#include "effectinfo.h"
#include "effect.h"

EffectInfo::EffectInfo(Effect *effect, bool isLocal)
{
	ASSERT(effect != NULL);
	m_effect = effect;
	m_isLocal = isLocal;
}

EffectInfo::~EffectInfo()
{
	SAFE_DELETE(m_effect);
}

