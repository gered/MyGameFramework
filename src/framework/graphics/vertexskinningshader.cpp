#include "../debug.h"

#include "vertexskinningshader.h"
#include "../math/quaternion.h"
#include "../math/vector3.h"

VertexSkinningShader::VertexSkinningShader()
	: StandardShader()
{
	STACK_TRACE;
	SetJointPositionsUniform("u_jointPositions");
	SetJointRotationsUniform("u_jointRotations");
}

VertexSkinningShader::~VertexSkinningShader()
{
	STACK_TRACE;
}

void VertexSkinningShader::SetJointPositions(const Vector3 *positions, uint32_t count)
{
	STACK_TRACE;
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_positionsUniform, positions, count);
}

void VertexSkinningShader::SetJointRotations(const Quaternion *rotations, uint32_t count)
{
	STACK_TRACE;
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_rotationsUniform, rotations, count);
}
