#include "../debug.h"

#include "spriteshader.h"

SpriteShader::SpriteShader()
{
	STACK_TRACE;
	SetTextureHasAlphaOnlyUniform("u_textureHasAlphaOnly");
}

SpriteShader::~SpriteShader()
{
	STACK_TRACE;
}

void SpriteShader::SetTextureHasAlphaOnly(BOOL hasAlphaOnly)
{
	STACK_TRACE;
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_textureHasAlphaOnlyUniform, (int32_t)hasAlphaOnly);
}
