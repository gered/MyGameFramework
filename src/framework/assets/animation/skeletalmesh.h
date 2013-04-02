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

	uint GetNumVertices() const                            { return m_numVertices; }
	Vector3* GetVertices() const                           { return m_vertices; }
	JointVertexMapping* GetJointMappings() const           { return m_jointMappings; }
	uint GetNumSubsets() const                             { return m_numSubsets; }
	SkeletalMeshSubset* GetSubsets() const                 { return m_subsets; }
	int GetIndexOfSubset(const stl::string &name) const;
	uint GetNumJoints() const                              { return m_numJoints; }
	Joint* GetJoints() const                               { return m_joints; }
	Joint* GetJoint(const stl::string &name) const;
	int GetIndexOfJoint(const stl::string &name) const;
	int GetRootJointIndex() const                          { return m_rootJointIndex; }
	uint GetNumFrames() const                              { return m_numFrames; }
	const AnimationSequence* GetAnimation(const stl::string &name) const;
	VertexBuffer* GetVertexBuffer() const                  { return m_vertexBuffer; }

private:
	SkeletalMesh();
	void FindAndSetRootJointIndex();
	
	uint m_numVertices;
	Vector3 *m_vertices;
	JointVertexMapping *m_jointMappings;
	uint m_numSubsets;
	SkeletalMeshSubset *m_subsets;
	uint m_numJoints;
	Joint *m_joints;
	int m_rootJointIndex;
	uint m_numFrames;
	AnimationList m_animations;
	VertexBuffer *m_vertexBuffer;
};

#endif
