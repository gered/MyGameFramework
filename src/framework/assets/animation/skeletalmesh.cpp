#include "../../debug.h"

#include "../../common.h"
#include "skeletalmesh.h"
#include "joint.h"
#include "jointkeyframe.h"
#include "jointvertexmapping.h"
#include "skeletalmeshsubset.h"
#include "skeletalmeshtriangle.h"
#include "../../graphics/indexbuffer.h"
#include "../../graphics/vertexbuffer.h"
#include "../../math/matrix4x4.h"
#include "../../math/quaternion.h"
#include "../../math/vector3.h"
#include "../../support/animationsequence.h"

SkeletalMesh::SkeletalMesh()
{
	m_numVertices = 0;
	m_numSubsets = 0;
	m_numJoints = 0;
	m_numFrames = 0;
	m_vertices = NULL;
	m_jointMappings = NULL;
	m_subsets = NULL;
	m_joints = NULL;
	m_rootJointIndex = NO_JOINT;
	m_vertexBuffer = NULL;
}

SkeletalMesh::~SkeletalMesh()
{
	SAFE_DELETE_ARRAY(m_joints);
	SAFE_DELETE_ARRAY(m_jointMappings);
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE(m_vertexBuffer);
	SAFE_DELETE_ARRAY(m_subsets);
	m_animations.clear();
}

int SkeletalMesh::GetIndexOfSubset(const stl::string &name) const
{
	for (uint i = 0; i < m_numSubsets; ++i)
	{
		if (m_subsets[i].GetName() == name)
			return (int)i;
	}
	
	return -1;
}

Joint* SkeletalMesh::GetJoint(const stl::string &name) const
{
	int jointIndex = GetIndexOfJoint(name);
	if (jointIndex == NO_JOINT)
		return NULL;
	else
		return &m_joints[jointIndex];
}

int SkeletalMesh::GetIndexOfJoint(const stl::string &name) const
{
	for (uint i = 0; i < m_numJoints; ++i)
	{
		if (m_joints[i].name == name)
			return (int)i;
	}
	
	return NO_JOINT;
}

const AnimationSequence* SkeletalMesh::GetAnimation(const stl::string &name) const
{
	AnimationList::const_iterator itor = m_animations.find(name);
	if (itor != m_animations.end())
		return &itor->second;
	else
		return NULL;
}

void SkeletalMesh::FindAndSetRootJointIndex()
{
	ASSERT(m_numJoints > 0);
	
	// if this mesh has only one joint, then that one has to be the root...
	if (m_numJoints == 1)
	{
		m_rootJointIndex = 0;
		return;
	}
	
	uint numJointsTried = 0;
	int rootIndex = NO_JOINT;
	while (numJointsTried < m_numJoints && rootIndex == NO_JOINT)
	{
		int parentlessJoint = NO_JOINT;

		// try the next joint without any parents
		for (uint i = numJointsTried; i < m_numJoints; ++i)
		{
			if (m_joints[i].parent == NULL)
			{
				parentlessJoint = (int)i;
				break;
			}
		}
		if (parentlessJoint == NO_JOINT)
		{
			// no more joints without a parent to try
			// this mesh doesn't have a valid root joint
			// TODO: should this be considered an error?
			break;
		}
		
		// ensure it has child joints
		bool hasChildJoints = false;
		for (uint i = 0; i < m_numJoints; ++i)
		{
			int parentIndex = m_joints[i].parentIndex;
			if (parentIndex == parentlessJoint)
			{
				hasChildJoints = true;
				break;
			}
		}
		
		if (hasChildJoints)
			rootIndex = parentlessJoint;
		else
			++numJointsTried;
	}
	
	m_rootJointIndex = rootIndex;
}
