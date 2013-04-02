#include "../debug.h"

#include "spriteshader.h"

SpriteShader::SpriteShader()
{
	SetTextureHasAlphaOnlyUniform("u_textureHasAlphaOnly");
}

SpriteShader::~SpriteShader()
{
}

void SpriteShader::SetTextureHasAlphaOnly(bool hasAlphaOnly)
{
	ASSERT(IsReadyForUse() == true);
	SetUniform(m_textureHasAlphaOnlyUniform, (int)hasAlphaOnly);
}
