#include "../debug.h"

#include "customspriteshader.h"

CustomSpriteShader::CustomSpriteShader()
{
}

CustomSpriteShader::~CustomSpriteShader()
{
}

BOOL CustomSpriteShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!SpriteShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == TRUE);
	
	return TRUE;
}

BOOL CustomSpriteShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!SpriteShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == TRUE);
	
	return TRUE;
}

