#include "../debug.h"

#include "customvertexlerpshader.h"

CustomVertexLerpShader::CustomVertexLerpShader()
{
}

CustomVertexLerpShader::~CustomVertexLerpShader()
{
}

bool CustomVertexLerpShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!VertexLerpShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetLerpUniform()) == true);
	
	return true;
}

bool CustomVertexLerpShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!VertexLerpShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetLerpUniform()) == true);
	
	return true;
}
