#include "../debug.h"

#include "customvertexlerpshader.h"
#include "../support/text.h"

CustomVertexLerpShader::CustomVertexLerpShader(const char *vertexShaderSource, const char *fragmentShaderSource)
	: VertexLerpShader()
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);

	BOOL result = LoadCompileAndLink(vertexShaderSource, fragmentShaderSource);
	ASSERT(result == TRUE);

	CacheShaderSources(vertexShaderSource, fragmentShaderSource);

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetLerpUniform()) == TRUE);
}

CustomVertexLerpShader::CustomVertexLerpShader(const Text *vertexShaderSource, const Text *fragmentShaderSource)
	: VertexLerpShader()
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
	ASSERT(HasUniform(GetLerpUniform()) == TRUE);
}

CustomVertexLerpShader::~CustomVertexLerpShader()
{
	STACK_TRACE;
}
