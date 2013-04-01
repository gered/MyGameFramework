#include "../debug.h"

#include "customstandardshader.h"

CustomStandardShader::CustomStandardShader()
{
	STACK_TRACE;
}

CustomStandardShader::~CustomStandardShader()
{
	STACK_TRACE;
}

BOOL CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	
	return TRUE;
}

BOOL CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	STACK_TRACE;
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	
	return TRUE;
}
