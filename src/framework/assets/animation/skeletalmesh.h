#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESH_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESH_H_INCLUDED__

#include "../../common.h"
#include "../../support/animationsequence.h"
#include "../../content/content.h"
#include "../../util/typesystem.h"
#include <stl/string.h>

class SkeletalMeshFile;
class SkeletalMeshSubset;
class VertexBuffer;
struct Joint;
struct JointVertexMapping;
struct Vector3;

class SkeletalMesh : public Content
{
	friend class SkeletalMeshFile;
	
public:
	TYPE_DEFINE(CONTENT_TYPE, "SkeletalMesh");
	
	virtual ~SkeletalMesh();

	uint32_t GetNumVertices() const                        { return m_numVertices; }
	Vector3* GetVertices() const                           { return m_vertices; }
	JointVertexMapping* GetJointMappings() const           { return m_jointMappings; }
	uint32_t GetNumSubsets() const                         { return m_numSubsets; }
	SkeletalMeshSubset* GetSubsets() const                 { return m_subsets; }
	int32_t GetIndexOfSubset(const stl::string &name) const;
	uint32_t GetNumJoints() const                          { return m_numJoints; }
	Joint* GetJoints() const                               { return m_joints; }
	Joint* GetJoint(const stl::string &name) const;
	int32_t GetIndexOfJoint(const stl::string &name) const;
	int32_t GetRootJointIndex() const                     { return m_rootJointIndex; }
	uint32_t GetNumFrames() const                          { return m_numFrames; }
	const AnimationSequence* GetAnimation(const stl::string &name) const;
	VertexBuffer* GetVertexBuffer() const                  { return m_vertexBuffer; }

private:
	SkeletalMesh();
	void FindAndSetRootJointIndex();
	
	uint32_t m_numVertices;
	Vector3 *m_vertices;
	JointVertexMapping *m_jointMappings;
	uint32_t m_numSubsets;
	SkeletalMeshSubset *m_subsets;
	uint32_t m_numJoints;
	Joint *m_joints;
	int32_t m_rootJointIndex;
	uint32_t m_numFrames;
	AnimationList m_animations;
	VertexBuffer *m_vertexBuffer;
};

#endif
