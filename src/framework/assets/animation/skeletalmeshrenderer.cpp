#include "../../debug.h"

#include "../../common.h"
#include "skeletalmeshrenderer.h"
#include "skeletalmesh.h"
#include "skeletalmeshinstance.h"
#include "skeletalmeshanimationinstance.h"
#include "skeletalmeshsubset.h"
#include "../../graphics/blendstate.h"
#include "../../graphics/graphicsdevice.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/texture.h"
#include "../../graphics/vertexskinningshader.h"

SkeletalMeshRenderer::SkeletalMeshRenderer()
{
}

SkeletalMeshRenderer::~SkeletalMeshRenderer()
{
}

void SkeletalMeshRenderer::Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, VertexSkinningShader *shader)
{
	ASSERT(shader->IsBound() == TRUE);
	Render(graphicsDevice, instance, 0, shader);
}

void SkeletalMeshRenderer::Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, uint frame, VertexSkinningShader *shader)
{
	ASSERT(shader->IsBound() == TRUE);
	instance->CalculateJointTransformations(frame);
	shader->SetJointPositions(instance->GetJointPositions(), instance->GetNumJoints());
	shader->SetJointRotations(instance->GetJointRotations(), instance->GetNumJoints());
	RenderAllSubsets(graphicsDevice, instance, shader);
}

void SkeletalMeshRenderer::Render(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, uint startFrame, uint endFrame, float interpolation, VertexSkinningShader *shader)
{
	ASSERT(shader->IsBound() == TRUE);
	instance->CalculateJointTransformations(startFrame, endFrame, interpolation);
	shader->SetJointPositions(instance->GetJointPositions(), instance->GetNumJoints());
	shader->SetJointRotations(instance->GetJointRotations(), instance->GetNumJoints());
	RenderAllSubsets(graphicsDevice, instance, shader);
}

void SkeletalMeshRenderer::Render(GraphicsDevice *graphicsDevice, SkeletalMeshAnimationInstance *instance, VertexSkinningShader *shader)
{
	ASSERT(shader->IsBound() == TRUE);
	Render(graphicsDevice, instance, instance->GetCurrentFrame(), instance->GetNextFrame(), instance->GetInterpolation(), shader);
}

void SkeletalMeshRenderer::RenderAllSubsets(GraphicsDevice *graphicsDevice, SkeletalMeshInstance *instance, VertexSkinningShader *shader)
{
	instance->GetRenderState()->Apply();
	graphicsDevice->BindVertexBuffer(instance->GetMesh()->GetVertexBuffer());
	
	BOOL hasAlphaSubsets = FALSE;
	
	if (instance->GetRenderAllSubsetsAlphaBlended())
	{
		// this instance has been overridden to have all it's subsets rendered
		// with alpha blending enabled. don't bother with the first loop to find
		// non-alpha-enabled subsets...
		hasAlphaSubsets = TRUE;
	}
	else
	{
		// render only non-alpha subsets first
		instance->GetBlendState()->Apply();
		for (uint i = 0; i < instance->GetMesh()->GetNumSubsets(); ++i)
		{
			if (!instance->IsSubsetEnabled(i))
				continue;
			
			const SkeletalMeshSubset *subset = &instance->GetMesh()->GetSubsets()[i];
			if (subset->IsAlphaBlended())
				// note we have an alpha subset, but don't render it quite yet
				hasAlphaSubsets = TRUE;
			else
				RenderSubset(graphicsDevice, subset, instance->GetTextures()[i]);
		}
	}
	
	// now render only alpha subsets, if we found any
	if (hasAlphaSubsets)
	{
		instance->GetAlphaBlendState()->Apply();
		for (uint i = 0; i < instance->GetMesh()->GetNumSubsets(); ++i)
		{
			if (!instance->IsSubsetEnabled(i))
				continue;
			
			const SkeletalMeshSubset *subset = &instance->GetMesh()->GetSubsets()[i];
			if (subset->IsAlphaBlended() || instance->GetRenderAllSubsetsAlphaBlended())
				RenderSubset(graphicsDevice, subset, instance->GetTextures()[i]);
		}
	}
}

void SkeletalMeshRenderer::RenderSubset(GraphicsDevice *graphicsDevice, const SkeletalMeshSubset *subset, const Texture *texture)
{
	if (texture != NULL)
		graphicsDevice->BindTexture(texture);
	
	graphicsDevice->BindIndexBuffer(subset->GetIndices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindIndexBuffer();
}
