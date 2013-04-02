#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHINSTANCE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHINSTANCE_H_INCLUDED__

#include "../../common.h"
#include <stl/string.h>

class BlendState;
class RenderState;
class SkeletalMesh;
class Texture;
struct Matrix4x4;
struct Vector3;
struct Quaternion;

class SkeletalMeshInstance
{
public:
	SkeletalMeshInstance(SkeletalMesh *mesh);
	virtual ~SkeletalMeshInstance();

	virtual void OnUpdate(float delta);
	void CalculateJointTransformations(uint frame);
	void CalculateJointTransformations(uint startFrame, uint endFrame, float interpolation);

	uint GetNumSubsets() const                             { return m_numSubsets; }
	BOOL IsSubsetEnabled(uint index) const                 { return m_enabledSubsets[index]; }
	void EnableSubset(const stl::string &subset, BOOL enable);
	void EnableSubset(uint index, BOOL enable)             { m_enabledSubsets[index] = enable; }
	
	Texture** GetTextures() const                          { return m_textures; }
	void SetTexture(uint index, Texture *texture);
	void SetTexture(const stl::string &subset, Texture *texture);
	
	uint GetNumJoints() const                              { return m_numJoints; }
	Matrix4x4* GetJointTransformations() const             { return m_jointTransformations; }
	Vector3* GetJointPositions() const                     { return m_jointPositions; }
	Quaternion* GetJointRotations() const                  { return m_jointRotations; }
	const Matrix4x4* GetJointTransformation(const stl::string &jointName) const;
	void SetFixedRootJointTransformation(const Matrix4x4 &transform);
	void ClearFixedRootJointTransformation();

	RenderState* GetRenderState() const                    { return m_renderState; }
	BlendState* GetBlendState() const                      { return m_blendState; }
	BlendState* GetAlphaBlendState() const                 { return m_alphaBlendState; }
	
	BOOL GetRenderAllSubsetsAlphaBlended() const           { return m_renderAllSubsetsAlphaBlended; }
	void SetRenderAllSubsetsAlphaBlended(BOOL enable)      { m_renderAllSubsetsAlphaBlended = enable; }
	
	SkeletalMesh* GetMesh() const                          { return m_mesh; }

private:
	void BreakDownJointTransformationMatrix(uint jointMatrixIndex);
	
	SkeletalMesh *m_mesh;
	uint m_numSubsets;
	BOOL *m_enabledSubsets;
	Texture **m_textures;
	RenderState *m_renderState;
	BlendState *m_blendState;
	BlendState *m_alphaBlendState;
	BOOL m_renderAllSubsetsAlphaBlended;
	
	uint m_numJoints;
	Matrix4x4 *m_jointTransformations;
	Vector3 *m_jointPositions;
	Quaternion *m_jointRotations;
	BOOL m_rootJointHasFixedTransform;
};

#endif
