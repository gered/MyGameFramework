#ifndef __EFFECTS_DIMEFFECT_H_INCLUDED__
#define __EFFECTS_DIMEFFECT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "effect.h"
#include "../framework/graphics/color.h"

class RenderContext;

class DimEffect : public Effect
{
public:
	TYPE_DEFINE(EFFECT_TYPE, "DimEffect");

	DimEffect();
	virtual ~DimEffect();

	void OnRender(RenderContext *renderContext);

	DimEffect* SetColor(const Color &color);
	DimEffect* SetAlpha(float alpha);

private:
	float m_alpha;
	Color m_color;
};

inline DimEffect* DimEffect::SetColor(const Color &color)
{
	m_color = color;
	return this;
}

inline DimEffect* DimEffect::SetAlpha(float alpha)
{
	m_alpha = alpha;
	return this;
}

#endif

