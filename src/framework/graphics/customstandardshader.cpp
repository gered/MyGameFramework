#include "../debug.h"

#include "customstandardshader.h"
#include "../support/text.h"

CustomStandardShader::CustomStandardShader(const char *vertexShaderSource, const char *fragmentShaderSource)
	: StandardShader()
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);

	BOOL result = LoadCompileAndLink(vertexShaderSource, fragmentShaderSource);
	ASSERT(result == TRUE);

	CacheShaderSources(vertexShaderSource, fragmentShaderSource);

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
}

CustomStandardShader::CustomStandardShader(const Text *vertexShaderSource, const Text *fragmentShaderSource)
	: StandardShader()
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
}

CustomStandardShader::~CustomStandardShader()
{
	STACK_TRACE;
}
