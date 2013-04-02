#include "../debug.h"

#include "vertexlerpshader.h"

VertexLerpShader::VertexLerpShader()
{
	SetLerpUniform("u_lerp");
}

VertexLerpShader::~VertexLerpShader()
{
}

void VertexLerpShader::SetLerp(float t)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_lerpUniform, t);
}
