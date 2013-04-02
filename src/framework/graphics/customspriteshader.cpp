#include "../debug.h"

#include "customspriteshader.h"

CustomSpriteShader::CustomSpriteShader()
{
}

CustomSpriteShader::~CustomSpriteShader()
{
}

bool CustomSpriteShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!SpriteShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == true);
	
	return true;
}

bool CustomSpriteShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!SpriteShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == true);
	
	return true;
}

