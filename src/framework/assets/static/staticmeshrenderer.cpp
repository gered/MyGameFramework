#include "staticmeshrenderer.h"

#include "staticmesh.h"
#include "staticmeshinstance.h"
#include "staticmeshsubset.h"
#include "../../graphics/graphicsdevice.h"
#include "../../graphics/texture.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/vertexbuffer.h"

StaticMeshRenderer::StaticMeshRenderer()
{
}

StaticMeshRenderer::~StaticMeshRenderer()
{
}

void StaticMeshRenderer::Render(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance)
{
	instance->GetRenderState()->Apply();

	if (instance->GetNumTextures() > 0)
		RenderAllSubsets(graphicsDevice, instance);
	else
		RenderAllSubsetsTextureless(graphicsDevice, instance);
}

void StaticMeshRenderer::RenderAllSubsets(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance)
{
	for (uint32_t i = 0; i < instance->GetMesh()->GetNumSubsets(); ++i)
		RenderSubset(graphicsDevice, instance->GetMesh()->GetSubset(i), instance->GetTexture(i));
}

void StaticMeshRenderer::RenderAllSubsetsTextureless(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance)
{
	for (uint32_t i = 0; i < instance->GetMesh()->GetNumSubsets(); ++i)
		RenderTexturelessSubset(graphicsDevice, instance->GetMesh()->GetSubset(i));
}

void StaticMeshRenderer::RenderSubset(GraphicsDevice *graphicsDevice, const StaticMeshSubset *subset, const Texture *texture)
{
	if (texture != NULL)
		graphicsDevice->BindTexture(texture);
	graphicsDevice->BindVertexBuffer(subset->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

void StaticMeshRenderer::RenderTexturelessSubset(GraphicsDevice *graphicsDevice, const StaticMeshSubset *subset)
{
	graphicsDevice->BindVertexBuffer(subset->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

