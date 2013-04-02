#include "../debug.h"

#include "customstandardshader.h"

CustomStandardShader::CustomStandardShader()
{
}

CustomStandardShader::~CustomStandardShader()
{
}

bool CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	
	return true;
}

bool CustomStandardShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!StandardShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	
	return true;
}
