#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHRENDERER_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHRENDERER_H_INCLUDED__

#include "../../common.h"

class GraphicsDevice;
class SkeletalMesh;
class SkeletalMeshInstance;
class SkeletalMeshAnimationInstance;
class SkeletalMeshSubset;
class Texture;
class VertexSkinningShader;
struct Matrix4x4;

class SkeletalMeshRenderer
{
public:
	SkeletalMeshRenderer();
	virtual ~SkeletalMeshRenderer();

	void Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, VertexSkinningShader *shader);
	void Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, uint32_t frame, VertexSkinningShader *shader);
	void Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, uint32_t startFrame, uint32_t endFrame, float interpolation, VertexSkinningShader *shader);
	void Render(GraphicsDevice *graphicsDevice, SkeletalMeshAnimationInstance *instance, VertexSkinningShader *shader);

private:
	void RenderAllSubsets(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, VertexSkinningShader *shader);
	void RenderSubset(GraphicsDevice *graphicsDevice, const SkeletalMeshSubset *subset, const Texture *texture);
};

#endif
