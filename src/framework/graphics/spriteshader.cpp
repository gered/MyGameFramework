#include "../debug.h"

#include "spriteshader.h"

SpriteShader::SpriteShader()
{
	SetTextureHasAlphaOnlyUniform("u_textureHasAlphaOnly");
}

SpriteShader::~SpriteShader()
{
}

void SpriteShader::SetTextureHasAlphaOnly(BOOL hasAlphaOnly)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_textureHasAlphaOnlyUniform, (int)hasAlphaOnly);
}
