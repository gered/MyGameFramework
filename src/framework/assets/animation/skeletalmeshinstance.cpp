#include "../../debug.h"

#include "../../common.h"
#include "skeletalmeshinstance.h"
#include "skeletalmesh.h"
#include "joint.h"
#include "jointkeyframe.h"
#include "../../graphics/blendstate.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/texture.h"
#include "../../math/matrix4x4.h"
#include "../../math/quaternion.h"
#include "../../math/vector3.h"
#include <stl/string.h>

SkeletalMeshInstance::SkeletalMeshInstance(SkeletalMesh *mesh)
{
	STACK_TRACE;
	m_mesh = mesh;

	m_numSubsets = m_mesh->GetNumSubsets();
	
	m_enabledSubsets = new BOOL[m_numSubsets];
	m_textures = new Texture*[m_numSubsets];
	
	for (uint32_t i = 0; i < m_numSubsets; ++i)
	{
		m_enabledSubsets[i] = TRUE;
		m_textures[i] = NULL;
	}

	m_renderState = new RENDERSTATE_DEFAULT;
	m_blendState = new BLENDSTATE_DEFAULT;
	m_alphaBlendState = new BLENDSTATE_ALPHABLEND;
	m_renderAllSubsetsAlphaBlended = FALSE;
	
	m_numJoints = mesh->GetNumJoints();
	m_jointTransformations = new Matrix4x4[m_numJoints];
	m_jointPositions = new Vector3[m_numJoints];
	m_jointRotations = new Quaternion[m_numJoints];
	m_rootJointHasFixedTransform = FALSE;
}

SkeletalMeshInstance::~SkeletalMeshInstance()
{
	STACK_TRACE;
	SAFE_DELETE(m_renderState);
	SAFE_DELETE(m_blendState);
	SAFE_DELETE(m_alphaBlendState);
	SAFE_DELETE_ARRAY(m_enabledSubsets);
	SAFE_DELETE_ARRAY(m_textures);
	SAFE_DELETE_ARRAY(m_jointTransformations);
	SAFE_DELETE_ARRAY(m_jointPositions);
	SAFE_DELETE_ARRAY(m_jointRotations);
}

void SkeletalMeshInstance::OnUpdate(float delta)
{
	STACK_TRACE;
}

const Matrix4x4* SkeletalMeshInstance::GetJointTransformation(const stl::string &jointName) const
{
	STACK_TRACE;
	int32_t jointIndex = GetMesh()->GetIndexOfJoint(jointName);
	if (jointIndex == NO_JOINT)
		return NULL;
	else
		return &m_jointTransformations[jointIndex];
}

void SkeletalMeshInstance::SetFixedRootJointTransformation(const Matrix4x4 &transform)
{
	STACK_TRACE;
	ASSERT(GetMesh()->GetRootJointIndex() != NO_JOINT);
	m_rootJointHasFixedTransform = TRUE;
	m_jointTransformations[GetMesh()->GetRootJointIndex()] = transform;
}

void SkeletalMeshInstance::ClearFixedRootJointTransformation()
{
	STACK_TRACE;
	ASSERT(GetMesh()->GetRootJointIndex() != NO_JOINT);
	m_rootJointHasFixedTransform = FALSE;
}

void SkeletalMeshInstance::CalculateJointTransformations(uint32_t frame)
{
	STACK_TRACE;
	int32_t rootJointIndex = GetMesh()->GetRootJointIndex();
	
	for (uint32_t i = 0; i < m_numJoints; ++i)
	{
		if (!(m_rootJointHasFixedTransform && i == rootJointIndex))
		{
			Joint *joint = &GetMesh()->GetJoints()[i];
			
			const Vector3 *translation = &joint->frames[frame].position;
			const Quaternion *rotation = &joint->frames[frame].rotation;
			
			Matrix4x4 transform =
				joint->relative * 
				Matrix4x4::CreateTranslation(translation->x, translation->y, translation->z) * 
				rotation->ToMatrix();
					
			if (joint->parent != NULL)
			{
				// concatenate this joint's transformation onto the parent's 
				// transformation
				m_jointTransformations[i] = m_jointTransformations[joint->parentIndex];
				m_jointTransformations[i] *= transform;
			}
			else
			{
				// no parent, just use this joint's relative final transformation
				// as the final transformation
				m_jointTransformations[i] = transform;
			}
		}
		
		BreakDownJointTransformationMatrix(i);
	}
}

void SkeletalMeshInstance::CalculateJointTransformations(uint32_t startFrame, uint32_t endFrame, float interpolation)
{
	STACK_TRACE;	
	int32_t rootJointIndex = GetMesh()->GetRootJointIndex();

	for (uint32_t i = 0; i < m_numJoints; ++i)
	{
		if (!(m_rootJointHasFixedTransform && i == rootJointIndex))
		{
			Joint *joint = &GetMesh()->GetJoints()[i];
			const JointKeyFrame *frame1 = &joint->frames[startFrame];
			const JointKeyFrame *frame2 = &joint->frames[endFrame];
					
			// interpolate translation and rotation
			Vector3 translation = Vector3::Lerp(frame1->position, frame2->position, interpolation);
			Quaternion rotation = Quaternion::Slerp(frame1->rotation, frame2->rotation, interpolation);
			
			Matrix4x4 transform = 
				joint->relative *
				Matrix4x4::CreateTranslation(translation.x, translation.y, translation.z) * 
				rotation.ToMatrix();
			
			if (joint->parent != NULL)
			{
				// concatenate this joint's transformation onto the parent's 
				// transformation
				m_jointTransformations[i] = m_jointTransformations[joint->parentIndex];
				m_jointTransformations[i] *= transform;
			}
			else
			{
				// no parent, just use this joint's relative final transformation
				// as the final transformation
				m_jointTransformations[i] = transform;
			}
		}
		
		BreakDownJointTransformationMatrix(i);
	}
}

void SkeletalMeshInstance::BreakDownJointTransformationMatrix(uint32_t jointMatrixIndex)
{
	STACK_TRACE;
	const Matrix4x4 *jointMatrix = &m_jointTransformations[jointMatrixIndex];
	
	m_jointPositions[jointMatrixIndex] = jointMatrix->GetTranslation();
	m_jointRotations[jointMatrixIndex] = Quaternion::CreateFromRotationMatrix(*jointMatrix);
}

void SkeletalMeshInstance::EnableSubset(const stl::string &subset, BOOL enable)
{
	STACK_TRACE;
	int32_t index = m_mesh->GetIndexOfSubset(subset);
	ASSERT(index != -1);
	if (index == -1)
		return;
	else
		m_enabledSubsets[index] = enable;
}

void SkeletalMeshInstance::SetTexture(uint32_t index, Texture *texture)
{
	STACK_TRACE;
	ASSERT(index < m_numSubsets);
	m_textures[index] = texture;
}

void SkeletalMeshInstance::SetTexture(const stl::string &subset, Texture *texture)
{
	STACK_TRACE;
	int32_t index = m_mesh->GetIndexOfSubset(subset);
	ASSERT(index != -1);
	if (index == -1)
		return;
	else
		m_textures[index] = texture;
}
