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
	void CalculateJointTransformations(uint32_t frame);
	void CalculateJointTransformations(uint32_t startFrame, uint32_t endFrame, float interpolation);

	uint32_t GetNumSubsets() const                         { return m_numSubsets; }
	BOOL IsSubsetEnabled(uint32_t index) const             { return m_enabledSubsets[index]; }
	void EnableSubset(const stl::string &subset, BOOL enable);
	void EnableSubset(uint32_t index, BOOL enable)         { m_enabledSubsets[index] = enable; }
	
	Texture** GetTextures() const                          { return m_textures; }
	void SetTexture(uint32_t index, Texture *texture);
	void SetTexture(const stl::string &subset, Texture *texture);
	
	uint32_t GetNumJoints() const                          { return m_numJoints; }
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
	void BreakDownJointTransformationMatrix(uint32_t jointMatrixIndex);
	
	SkeletalMesh *m_mesh;
	uint32_t m_numSubsets;
	BOOL *m_enabledSubsets;
	Texture **m_textures;
	RenderState *m_renderState;
	BlendState *m_blendState;
	BlendState *m_alphaBlendState;
	BOOL m_renderAllSubsetsAlphaBlended;
	
	uint32_t m_numJoints;
	Matrix4x4 *m_jointTransformations;
	Vector3 *m_jointPositions;
	Quaternion *m_jointRotations;
	BOOL m_rootJointHasFixedTransform;
};

#endif
