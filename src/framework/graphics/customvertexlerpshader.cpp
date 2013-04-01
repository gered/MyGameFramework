#include "../debug.h"

#include "customvertexlerpshader.h"

CustomVertexLerpShader::CustomVertexLerpShader()
{
	STACK_TRACE;
}

CustomVertexLerpShader::~CustomVertexLerpShader()
{
	STACK_TRACE;
}

BOOL CustomVertexLerpShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	if (!VertexLerpShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetLerpUniform()) == TRUE);
	
	return TRUE;
}

BOOL CustomVertexLerpShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	STACK_TRACE;
	if (!VertexLerpShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;

	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetLerpUniform()) == TRUE);
	
	return TRUE;
}
