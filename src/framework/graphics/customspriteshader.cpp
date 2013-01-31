#include "../debug.h"

#include "customspriteshader.h"
#include "../support/text.h"

CustomSpriteShader::CustomSpriteShader(const char *vertexShaderSource, const char *fragmentShaderSource)
	: SpriteShader()
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);

	BOOL result = LoadCompileAndLink(vertexShaderSource, fragmentShaderSource);
	ASSERT(result == TRUE);

	CacheShaderSources(vertexShaderSource, fragmentShaderSource);

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == TRUE);
}

CustomSpriteShader::CustomSpriteShader(const Text *vertexShaderSource, const Text *fragmentShaderSource)
	: SpriteShader()
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(vertexShaderSource->GetLength() > 0);
	ASSERT(fragmentShaderSource != NULL);
	ASSERT(fragmentShaderSource->GetLength() > 0);

	BOOL result = LoadCompileAndLink(vertexShaderSource->GetText(), fragmentShaderSource->GetText());
	ASSERT(result == TRUE);

	CacheShaderSources(vertexShaderSource->GetText(), fragmentShaderSource->GetText());

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetTextureHasAlphaOnlyUniform()) == TRUE);
}

CustomSpriteShader::~CustomSpriteShader()
{
	STACK_TRACE;
}
