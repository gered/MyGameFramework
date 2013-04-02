#include "../debug.h"

#include "customvertexskinningshader.h"
#include "../support/text.h"

CustomVertexSkinningShader::CustomVertexSkinningShader()
{
}

CustomVertexSkinningShader::~CustomVertexSkinningShader()
{
}

bool CustomVertexSkinningShader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	if (!VertexSkinningShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetJointPositionsUniform()) == true);
	ASSERT(HasUniform(GetJointRotationsUniform()) == true);
	
	return true;
}

bool CustomVertexSkinningShader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	if (!VertexSkinningShader::Initialize(graphicsDevice, vertexShaderSource, fragmentShaderSource))
		return false;
	
	ASSERT(HasUniform(GetModelViewMatrixUniform()) == true);
	ASSERT(HasUniform(GetProjectionMatrixUniform()) == true);
	ASSERT(HasUniform(GetJointPositionsUniform()) == true);
	ASSERT(HasUniform(GetJointRotationsUniform()) == true);
	
	return true;
}
