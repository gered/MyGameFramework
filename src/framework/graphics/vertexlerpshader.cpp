#include "../debug.h"

#include "vertexlerpshader.h"

VertexLerpShader::VertexLerpShader()
{
	STACK_TRACE;
	SetLerpUniform("u_lerp");
}

VertexLerpShader::~VertexLerpShader()
{
	STACK_TRACE;
}

void VertexLerpShader::SetLerp(float t)
{
	STACK_TRACE;
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_lerpUniform, t);
}
