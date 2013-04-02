#include "../../debug.h"

#include "keyframemeshrenderer.h"

#include "keyframe.h"
#include "keyframemesh.h"
#include "keyframemeshinstance.h"
#include "keyframemeshtriangle.h"
#include "../../graphics/graphicsdevice.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/texture.h"
#include "../../graphics/vertexbuffer.h"
#include "../../graphics/vertexlerpshader.h"
#include "../../math/vector3.h"

KeyframeMeshRenderer::KeyframeMeshRenderer()
{
}

KeyframeMeshRenderer::~KeyframeMeshRenderer()
{
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), instance->GetCurrentFrame(), instance->GetNextFrame(), instance->GetInterpolation(), shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint frame, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), frame, shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint startFrame, uint endFrame, float interpolation, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), instance->GetCurrentFrame(), instance->GetNextFrame(), instance->GetInterpolation(), shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	Render(graphicsDevice, mesh, texture, 0, shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint frame, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	SetFrameVertices(mesh, frame);

	if (texture != NULL)
		graphicsDevice->BindTexture(texture);

	shader->SetLerp(0.0f);

	graphicsDevice->BindVertexBuffer(mesh->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint startFrame, uint endFrame, float interpolation, VertexLerpShader *shader)
{
	ASSERT(shader->IsBound() == true);
	SetFrameVertices(mesh, startFrame, endFrame);

	if (texture != NULL)
		graphicsDevice->BindTexture(texture);

	shader->SetLerp(interpolation);

	graphicsDevice->BindVertexBuffer(mesh->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

void KeyframeMeshRenderer::SetFrameVertices(KeyframeMesh *mesh, uint frame)
{
	uint pos;
	Keyframe *keyframe = mesh->GetFrames()[frame];
	KeyframeMeshTriangle *triangle;
	VertexBuffer *vertices = mesh->GetVertices();

	for (uint i = 0; i < mesh->GetNumTriangles(); ++i)
	{
		pos = i * 3;
		triangle = &mesh->GetTriangles()[i];

		vertices->Set3f(0, pos, keyframe->GetVertices()[triangle->vertices[0]]);
		vertices->Set3f(1, pos, ZERO_VECTOR);
		vertices->Set3f(2, pos, keyframe->GetNormals()[triangle->vertices[0]]);
		vertices->Set3f(3, pos, ZERO_VECTOR);
		vertices->Set2f(4, pos, mesh->GetTexCoords()[triangle->textureCoords[0]]);

		vertices->Set3f(0, pos + 1, keyframe->GetVertices()[triangle->vertices[1]]);
		vertices->Set3f(1, pos + 1, ZERO_VECTOR);
		vertices->Set3f(2, pos + 1, keyframe->GetNormals()[triangle->vertices[1]]);
		vertices->Set3f(3, pos + 2, ZERO_VECTOR);
		vertices->Set2f(4, pos + 1, mesh->GetTexCoords()[triangle->textureCoords[1]]);

		vertices->Set3f(0, pos + 2, keyframe->GetVertices()[triangle->vertices[2]]);
		vertices->Set3f(1, pos + 2, ZERO_VECTOR);
		vertices->Set3f(2, pos + 2, keyframe->GetNormals()[triangle->vertices[2]]);
		vertices->Set3f(3, pos + 2, ZERO_VECTOR);
		vertices->Set2f(4, pos + 2, mesh->GetTexCoords()[triangle->textureCoords[2]]);
	}
}

void KeyframeMeshRenderer::SetFrameVertices(KeyframeMesh *mesh, uint startFrame, uint endFrame)
{
	uint pos;
	Keyframe *frame1 = mesh->GetFrames()[startFrame];
	Keyframe *frame2 = mesh->GetFrames()[endFrame];
	KeyframeMeshTriangle *triangle;
	VertexBuffer *vertices = mesh->GetVertices();

	for (uint i = 0; i < mesh->GetNumTriangles(); ++i)
	{
		pos = i * 3;
		triangle = &mesh->GetTriangles()[i];

		vertices->Set3f(0, pos, frame1->GetVertices()[triangle->vertices[0]]);
		vertices->Set3f(1, pos, frame2->GetVertices()[triangle->vertices[0]]);
		vertices->Set3f(2, pos, frame1->GetNormals()[triangle->vertices[0]]);
		vertices->Set3f(3, pos, frame2->GetNormals()[triangle->vertices[0]]);
		vertices->Set2f(4, pos, mesh->GetTexCoords()[triangle->textureCoords[0]]);

		vertices->Set3f(0, pos + 1, frame1->GetVertices()[triangle->vertices[1]]);
		vertices->Set3f(1, pos + 1, frame2->GetVertices()[triangle->vertices[1]]);
		vertices->Set3f(2, pos + 1, frame1->GetNormals()[triangle->vertices[1]]);
		vertices->Set3f(3, pos + 1, frame2->GetNormals()[triangle->vertices[1]]);
		vertices->Set2f(4, pos + 1, mesh->GetTexCoords()[triangle->textureCoords[1]]);

		vertices->Set3f(0, pos + 2, frame1->GetVertices()[triangle->vertices[2]]);
		vertices->Set3f(1, pos + 2, frame2->GetVertices()[triangle->vertices[2]]);
		vertices->Set3f(2, pos + 2, frame1->GetNormals()[triangle->vertices[2]]);
		vertices->Set3f(3, pos + 2, frame2->GetNormals()[triangle->vertices[2]]);
		vertices->Set2f(4, pos + 2, mesh->GetTexCoords()[triangle->textureCoords[2]]);
	}
}
