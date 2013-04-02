#include "../debug.h"

#include "customstandardshader.h"

CustomStandardShader::CustomStandardShader()
{
}

CustomStandardShader::~CustomStandardShader()
{
}

BOOL CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	
	return TRUE;
}

BOOL CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	
	return TRUE;
}
