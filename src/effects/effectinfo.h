#ifndef __EFFECTS_EFFECTINFO_H_INCLUDED__
#define __EFFECTS_EFFECTINFO_H_INCLUDED__

#include "../framework/common.h"

class Effect;

class EffectInfo
{
public:
	EffectInfo(Effect *effect, BOOL isLocal);
	virtual ~EffectInfo();

	Effect* GetEffect() const                              { return m_effect; }
	BOOL IsLocal() const                                   { return m_isLocal; }

private:
	Effect *m_effect;
	BOOL m_isLocal;
};

#endif

