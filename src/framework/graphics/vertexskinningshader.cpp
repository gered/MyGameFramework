#include "../debug.h"

#include "vertexskinningshader.h"
#include "../math/quaternion.h"
#include "../math/vector3.h"

VertexSkinningShader::VertexSkinningShader()
{
	SetJointPositionsUniform("u_jointPositions");
	SetJointRotationsUniform("u_jointRotations");
}

VertexSkinningShader::~VertexSkinningShader()
{
}

void VertexSkinningShader::SetJointPositions(const Vector3 *positions, uint32_t count)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_positionsUniform, positions, count);
}

void VertexSkinningShader::SetJointRotations(const Quaternion *rotations, uint32_t count)
{
	ASSERT(IsReadyForUse() == TRUE);
	SetUniform(m_rotationsUniform, rotations, count);
}
