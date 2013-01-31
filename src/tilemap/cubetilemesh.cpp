#include "../framework/debug.h"

#include "cubetilemesh.h"
#include "tilemeshdefs.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/mathhelpers.h"
#include "../framework/math/rectf.h"
#include "../framework/math/vector2.h"
#include "../framework/math/vector3.h"

CubeTileMesh::CubeTileMesh(CUBE_FACES faces, const RectF *textureAtlasTileBoundaries, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, BOOL alpha, float translucency, const Color &color)
{
	STACK_TRACE;
	m_faces = faces;

	SetOpaque(opaqueSides);
	SetAlpha(alpha);
	SetColor(color);
	SetTranslucency(translucency);
	SetLight(lightValue);

	m_topFaceVertexOffset = 0;
	m_bottomFaceVertexOffset = 0;
	m_frontFaceVertexOffset = 0;
	m_backFaceVertexOffset = 0;
	m_leftFaceVertexOffset = 0;
	m_rightFaceVertexOffset = 0;

	uint32_t numVertices = 0;

	if (HasFace(SIDE_TOP))
	{
		m_topFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}
	if (HasFace(SIDE_BOTTOM))
	{
		m_bottomFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}
	if (HasFace(SIDE_FRONT))
	{
		m_frontFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}
	if (HasFace(SIDE_BACK))
	{
		m_backFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}
	if (HasFace(SIDE_LEFT))
	{
		m_leftFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}
	if (HasFace(SIDE_RIGHT))
	{
		m_rightFaceVertexOffset = numVertices;
		numVertices += CUBE_VERTICES_PER_FACE;
	}

	m_vertices = new VertexBuffer(BUFFEROBJECT_USAGE_STATIC);
	ASSERT(m_vertices != NULL);
	m_vertices->AddAttribute(VERTEX_POS_3D);
	m_vertices->AddAttribute(VERTEX_NORMAL);
	m_vertices->AddAttribute(VERTEX_COLOR);
	m_vertices->AddAttribute(VERTEX_TEXCOORD);
	m_vertices->Create(numVertices);

	SetupFaceVertices(textureAtlasTileBoundaries);
	SetupCollisionVertices();
}

CubeTileMesh::~CubeTileMesh()
{
	STACK_TRACE;
	SAFE_DELETE(m_vertices);
}

void CubeTileMesh::SetupFaceVertices(const RectF *textureAtlasTileBoundaries)
{
	STACK_TRACE;
	uint32_t pos = 0;
	Vector3 a(-0.5f, -0.5f, -0.5f);
	Vector3 b(0.5f, 0.5f, 0.5f);

	if (HasFace(SIDE_TOP))
	{
		pos = m_topFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(a.x, b.y, b.z));
		m_vertices->SetNormal(pos, UP);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(b.x, b.y, b.z));
		m_vertices->SetNormal(pos + 1, UP);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(a.x, b.y, a.z));
		m_vertices->SetNormal(pos + 2, UP);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(b.x, b.y, b.z));
		m_vertices->SetNormal(pos + 3, UP);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(b.x, b.y, a.z));
		m_vertices->SetNormal(pos + 4, UP);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(a.x, b.y, a.z));
		m_vertices->SetNormal(pos + 5, UP);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}

	if (HasFace(SIDE_BOTTOM))
	{
		pos = m_bottomFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(b.x, a.y, b.z));
		m_vertices->SetNormal(pos, DOWN);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(a.x, a.y, b.z));
		m_vertices->SetNormal(pos + 1, DOWN);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(b.x, a.y, a.z));
		m_vertices->SetNormal(pos + 2, DOWN);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(a.x, a.y, b.z));
		m_vertices->SetNormal(pos + 3, DOWN);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(a.x, a.y, a.z));
		m_vertices->SetNormal(pos + 4, DOWN);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(b.x, a.y, a.z));
		m_vertices->SetNormal(pos + 5, DOWN);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}

	if (HasFace(SIDE_FRONT))
	{
		pos = m_frontFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(b.x, a.y, a.z));
		m_vertices->SetNormal(pos, FORWARD);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(a.x, a.y, a.z));
		m_vertices->SetNormal(pos + 1, FORWARD);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(b.x, b.y, a.z));
		m_vertices->SetNormal(pos + 2, FORWARD);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(a.x, a.y, a.z));
		m_vertices->SetNormal(pos + 3, FORWARD);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(a.x, b.y, a.z));
		m_vertices->SetNormal(pos + 4, FORWARD);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(b.x, b.y, a.z));
		m_vertices->SetNormal(pos + 5, FORWARD);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}

	if (HasFace(SIDE_BACK))
	{
		pos = m_backFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(a.x, a.y, b.z));
		m_vertices->SetNormal(pos, BACKWARD);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(b.x, a.y, b.z));
		m_vertices->SetNormal(pos + 1, BACKWARD);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(a.x, b.y, b.z));
		m_vertices->SetNormal(pos + 2, BACKWARD);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(b.x, a.y, b.z));
		m_vertices->SetNormal(pos + 3, BACKWARD);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(b.x, b.y, b.z));
		m_vertices->SetNormal(pos + 4, BACKWARD);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(a.x, b.y, b.z));
		m_vertices->SetNormal(pos + 5, BACKWARD);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}

	if (HasFace(SIDE_LEFT))
	{
		pos = m_leftFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(a.x, a.y, a.z));
		m_vertices->SetNormal(pos, LEFT);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(a.x, a.y, b.z));
		m_vertices->SetNormal(pos + 1, LEFT);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(a.x, b.y, a.z));
		m_vertices->SetNormal(pos + 2, LEFT);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(a.x, a.y, b.z));
		m_vertices->SetNormal(pos + 3, LEFT);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(a.x, b.y, b.z));
		m_vertices->SetNormal(pos + 4, LEFT);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(a.x, b.y, a.z));
		m_vertices->SetNormal(pos + 5, LEFT);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}

	if (HasFace(SIDE_RIGHT))
	{
		pos = m_rightFaceVertexOffset;

		m_vertices->SetPosition3(pos, Vector3(b.x, a.y, b.z));
		m_vertices->SetNormal(pos, RIGHT);
		m_vertices->SetTexCoord(pos, ScaleTexCoord(Vector2(0.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 1, Vector3(b.x, a.y, a.z));
		m_vertices->SetNormal(pos + 1, RIGHT);
		m_vertices->SetTexCoord(pos + 1, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 2, Vector3(b.x, b.y, b.z));
		m_vertices->SetNormal(pos + 2, RIGHT);
		m_vertices->SetTexCoord(pos + 2, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 3, Vector3(b.x, a.y, a.z));
		m_vertices->SetNormal(pos + 3, RIGHT);
		m_vertices->SetTexCoord(pos + 3, ScaleTexCoord(Vector2(1.0f, 1.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 4, Vector3(b.x, b.y, a.z));
		m_vertices->SetNormal(pos + 4, RIGHT);
		m_vertices->SetTexCoord(pos + 4, ScaleTexCoord(Vector2(1.0f, 0.0f), *textureAtlasTileBoundaries));

		m_vertices->SetPosition3(pos + 5, Vector3(b.x, b.y, b.z));
		m_vertices->SetNormal(pos + 5, RIGHT);
		m_vertices->SetTexCoord(pos + 5, ScaleTexCoord(Vector2(0.0f, 0.0f), *textureAtlasTileBoundaries));
	}
}

void CubeTileMesh::SetupCollisionVertices()
{
	STACK_TRACE;
	m_numCollisionVertices = m_vertices->GetNumElements();

	m_collisionVertices = new Vector3[m_numCollisionVertices];
	ASSERT(m_collisionVertices != NULL);

	for (uint32_t i = 0; i < m_numCollisionVertices; ++i)
		m_collisionVertices[i] = m_vertices->GetPosition3(i);
}

inline Vector2 CubeTileMesh::ScaleTexCoord(const Vector2 &texCoord, const RectF &tileBoundaries) const
{
	Vector2 out;
	out.x = ScaleRange(texCoord.x, 0.0f, 1.0f, tileBoundaries.left, tileBoundaries.right);
	out.y = ScaleRange(texCoord.y, 0.0f, 1.0f, tileBoundaries.top, tileBoundaries.bottom);
	return out;
}

