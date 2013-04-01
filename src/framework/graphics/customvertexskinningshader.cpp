#include "../debug.h"

#include "customvertexskinningshader.h"
#include "../support/text.h"

CustomVertexSkinningShader::CustomVertexSkinningShader()
{
	STACK_TRACE;
}

CustomVertexSkinningShader::~CustomVertexSkinningShader()
{
	STACK_TRACE;
}

BOOL CustomVertexSkinningShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	if (!VertexSkinningShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetJointPositionsUniform()) == TRUE);
	ASSERT(HasUniform(GetJointRotationsUniform()) == TRUE);
	
	return TRUE;
}

BOOL CustomVertexSkinningShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	STACK_TRACE;
	if (!VertexSkinningShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return FALSE;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == TRUE);
	ASSERT(HasUniform(GetJointPositionsUniform()) == TRUE);
	ASSERT(HasUniform(GetJointRotationsUniform()) == TRUE);
	
	return TRUE;
}
