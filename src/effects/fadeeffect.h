#ifndef __EFFECTS_FADEEFFECT_H_INCLUDED__
#define __EFFECTS_FADEEFFECT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "effect.h"
#include "../framework/graphics/color.h"

class RenderContext;

class FadeEffect : public Effect
{
public:
	TYPE_DEFINE(EFFECT_TYPE, "FadeEffect");

	FadeEffect();
	virtual ~FadeEffect();

	void OnRender(RenderContext *renderContext);
	void OnUpdate(float delta);

	FadeEffect* SetColor(const Color &color);
	FadeEffect* SetFadeSpeed(float speed);
	FadeEffect* SetFadeOut(float toAlpha = COLOR_ALPHA_OPAQUE);
	FadeEffect* SetFadeIn(float toAlpha = COLOR_ALPHA_TRANSPARENT);

	BOOL IsDoneFading() const                              { return m_doneFading; }

private:
	BOOL m_doneFading;
	float m_fadeSpeed;
	BOOL m_fadingOut;
	float m_alpha;
	Color m_color;
	float m_fadeToAlpha;
};

inline FadeEffect* FadeEffect::SetColor(const Color &color)
{
	m_color = color;
	return this;
}

inline FadeEffect* FadeEffect::SetFadeSpeed(float speed)
{
	m_fadeSpeed = speed;
	return this;
}

inline FadeEffect* FadeEffect::SetFadeOut(float toAlpha)
{
	m_fadingOut = TRUE;
	m_alpha = COLOR_ALPHA_TRANSPARENT;
	m_fadeToAlpha = toAlpha;
	return this;
}

inline FadeEffect* FadeEffect::SetFadeIn(float toAlpha)
{
	m_fadingOut = FALSE;
	m_alpha = COLOR_ALPHA_OPAQUE;
	m_fadeToAlpha = toAlpha;
	return this;
}

#endif

