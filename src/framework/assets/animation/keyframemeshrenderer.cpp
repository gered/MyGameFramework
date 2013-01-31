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
	STACK_TRACE;
}

KeyframeMeshRenderer::~KeyframeMeshRenderer()
{
	STACK_TRACE;
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), instance->GetCurrentFrame(), instance->GetNextFrame(), instance->GetInterpolation(), shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint32_t frame, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), frame, shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint32_t startFrame, uint32_t endFrame, float interpolation, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	instance->GetRenderState()->Apply();
	Render(graphicsDevice, instance->GetMesh(), instance->GetTexture(), instance->GetCurrentFrame(), instance->GetNextFrame(), instance->GetInterpolation(), shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	Render(graphicsDevice, mesh, texture, 0, shader);
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint32_t frame, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	SetFrameVertices(mesh, frame);

	if (texture != NULL)
		graphicsDevice->BindTexture(texture);

	shader->SetLerp(0.0f);

	graphicsDevice->BindVertexBuffer(mesh->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

void KeyframeMeshRenderer::Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint32_t startFrame, uint32_t endFrame, float interpolation, VertexLerpShader *shader)
{
	STACK_TRACE;
	ASSERT(shader->IsBound() == TRUE);
	SetFrameVertices(mesh, startFrame, endFrame);

	if (texture != NULL)
		graphicsDevice->BindTexture(texture);

	shader->SetLerp(interpolation);

	graphicsDevice->BindVertexBuffer(mesh->GetVertices());
	graphicsDevice->RenderTriangles();
	graphicsDevice->UnbindVertexBuffer();
}

void KeyframeMeshRenderer::SetFrameVertices(KeyframeMesh *mesh, uint32_t frame)
{
	STACK_TRACE;
	int pos;
	Keyframe *keyframe = mesh->GetFrames()[frame];
	KeyframeMeshTriangle *triangle;
	VertexBuffer *vertices = mesh->GetVertices();

	for (uint32_t i = 0; i < mesh->GetNumTriangles(); ++i)
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

void KeyframeMeshRenderer::SetFrameVertices(KeyframeMesh *mesh, uint32_t startFrame, uint32_t endFrame)
{
	STACK_TRACE;
	uint32_t pos;
	Keyframe *frame1 = mesh->GetFrames()[startFrame];
	Keyframe *frame2 = mesh->GetFrames()[endFrame];
	KeyframeMeshTriangle *triangle;
	VertexBuffer *vertices = mesh->GetVertices();

	for (uint32_t i = 0; i < mesh->GetNumTriangles(); ++i)
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
