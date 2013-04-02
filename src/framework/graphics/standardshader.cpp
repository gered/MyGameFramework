#include "../debug.h"

#include "standardshader.h"
#include "../math/matrix4x4.h"

StandardShader::StandardShader()
{
	m_inlineVertexShaderSource = NULL;
	m_inlineFragmentShaderSource = NULL;

	SetModelViewMatrixUniform("u_modelViewMatrix");
	SetProjectionMatrixUniform("u_projectionMatrix");
}

StandardShader::~StandardShader()
{
}

BOOL StandardShader::LoadCompileAndLinkInlineSources(const char *inlineVertexShaderSource, const char *inlineFragmentShaderSource)
{
	ASSERT(inlineVertexShaderSource != NULL);
	ASSERT(inlineFragmentShaderSource != NULL);

	BOOL result = LoadCompileAndLink(inlineVertexShaderSource, inlineFragmentShaderSource);
	if (result)
	{
		// keep the pointers around to the inline sources for easy reloading later
		// (of course this means that these pointers need to be kept valid for a long time)
		m_inlineVertexShaderSource = inlineVertexShaderSource;
		m_inlineFragmentShaderSource = inlineFragmentShaderSource;
	}

	return result;
}

void StandardShader::SetModelViewMatrix(const Matrix4x4 &matrix)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_modelViewMatrixUniform, matrix);
}

void StandardShader::SetProjectionMatrix(const Matrix4x4 &matrix)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_projectionMatrixUniform, matrix);
}

void StandardShader::OnNewContext()
{
	ReloadCompileAndLink(m_inlineVertexShaderSource, m_inlineFragmentShaderSource);
}

void StandardShader::OnLostContext()
{
}
