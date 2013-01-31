#include "../debug.h"

#include "customvertexskinningshader.h"
#include "../support/text.h"

CustomVertexSkinningShader::CustomVertexSkinningShader(const char *vertexShaderSource, const char *fragmentShaderSource)
	: VertexSkinningShader()
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);
	
	BOOL result = LoadCompileAndLink(vertexShaderSource, fragmentShaderSource);
	ASSERT(result == TRUE);
	
	CacheShaderSources(vertexShaderSource, fragmentShaderSource);
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetJointPositionsUniform()) == TRUE);
	ASSERT(HasUniform(GetJointRotationsUniform()) == TRUE);
}

CustomVertexSkinningShader::CustomVertexSkinningShader(const Text *vertexShaderSource, const Text *fragmentShaderSource)
	: VertexSkinningShader()
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
	ASSERT(HasUniform(GetJointPositionsUniform()) == TRUE);
	ASSERT(HasUniform(GetJointRotationsUniform()) == TRUE);
}

CustomVertexSkinningShader::~CustomVertexSkinningShader()
{
	STACK_TRACE;
}
