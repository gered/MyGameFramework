#ifndef __EFFECTS_EFFECTINFO_H_INCLUDED__
#define __EFFECTS_EFFECTINFO_H_INCLUDED__

#include "../framework/common.h"

class Effect;

class EffectInfo
{
public:
	EffectInfo(Effect *effect, bool isLocal);
	virtual ~EffectInfo();

	Effect* GetEffect() const                              { return m_effect; }
	bool IsLocal() const                                   { return m_isLocal; }

private:
	Effect *m_effect;
	bool m_isLocal;
};

#endif

