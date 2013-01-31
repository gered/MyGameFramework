#include "../../debug.h"

#include "staticmeshbuilder.h"

#include "staticmesh.h"
#include "staticmeshsubset.h"
#include "../../graphics/texture.h"
#include "../../graphics/vertexbuffer.h"
#include "../../math/matrix4x4.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"

StaticMeshBuilder::StaticMeshBuilder()
{
	STACK_TRACE;
	m_transform = IDENTITY_MATRIX;
}

StaticMeshBuilder::~StaticMeshBuilder()
{
	STACK_TRACE;
	// BuildMesh() will handle passing off the subset objects to a mesh object
	// if it is called. if it's not called, this will free up the memory since
	// it was never passed off anywhere else in that case
	for (uint32_t i = 0; i < m_subsets.size(); ++i)
		SAFE_DELETE(m_subsets[i]);
	m_subsets.clear();
}

void StaticMeshBuilder::Reset()
{
	m_subsets.clear();
	m_transform = IDENTITY_MATRIX;
}

uint32_t StaticMeshBuilder::AddSubset(uint32_t numTriangles, Texture *texture)
{
	STACK_TRACE;
	ASSERT(numTriangles > 0);
	StaticMeshSubset *subset = new StaticMeshSubset(numTriangles, texture);
	ASSERT(subset != NULL);

	m_subsets.push_back(subset);

	return m_subsets.size() - 1;
}

void StaticMeshBuilder::SetTriangle(
	uint32_t subsetIndex,
	uint32_t triangle,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
	const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
	)
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > subsetIndex);

	StaticMeshSubset *subset = m_subsets[subsetIndex];
	ASSERT(subset != NULL);

	VertexBuffer *vertices = subset->GetVertices();
	uint32_t bufferIndex = triangle * 3;

	ASSERT((bufferIndex + 3) <= vertices->GetNumElements());

	SetTriangleInternal(
		vertices, bufferIndex,
		v1, v2, v3,
		n1, n2, n3,
		t1, t2, t3
		);
}

void StaticMeshBuilder::SetTriangle(
	uint32_t subsetIndex,
	uint32_t triangle,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
	)
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > subsetIndex);

	StaticMeshSubset *subset = m_subsets[subsetIndex];
	ASSERT(subset != NULL);

	VertexBuffer *vertices = subset->GetVertices();
	uint32_t bufferIndex = triangle * 3;

	ASSERT((bufferIndex + 3) <= vertices->GetNumElements());

	SetTriangleInternal(
		vertices, bufferIndex,
		v1, v2, v3,
		t1, t2, t3
		);
}

void StaticMeshBuilder::SetQuad(
	uint32_t subsetIndex,
	uint32_t firstTriangle,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, 
	const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, const Vector3 &n4, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3, const Vector2 &t4
	)
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > subsetIndex);

	StaticMeshSubset *subset = m_subsets[subsetIndex];
	ASSERT(subset != NULL);

	VertexBuffer *vertices = subset->GetVertices();
	uint32_t bufferIndex = firstTriangle * 3;

	ASSERT((bufferIndex + 6) <= vertices->GetNumElements());

	SetTriangleInternal(
		vertices, bufferIndex,
		v1, v2, v3,
		n1, n2, n3,
		t1, t2, t3
		);
	SetTriangleInternal(
		vertices, bufferIndex + 3,
		v2, v4, v3,
		n2, n4, n3,
		t2, t4, t3
		);
}

void StaticMeshBuilder::SetQuad(
	uint32_t subsetIndex,
	uint32_t firstTriangle,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3, const Vector2 &t4
	)
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > subsetIndex);

	StaticMeshSubset *subset = m_subsets[subsetIndex];
	ASSERT(subset != NULL);

	VertexBuffer *vertices = subset->GetVertices();
	uint32_t bufferIndex = firstTriangle * 3;

	ASSERT((bufferIndex + 6) <= vertices->GetNumElements());

	SetTriangleInternal(
		vertices, bufferIndex,
		v1, v2, v3,
		t1, t2, t3
		);
	SetTriangleInternal(
		vertices, bufferIndex + 3,
		v2, v4, v3,
		t2, t4, t3
		);
}

void StaticMeshBuilder::SetBox(
	uint32_t subsetIndex,
	uint32_t firstTriangle,
	const Vector3 &center, float width, float height, float depth
	)
{
	STACK_TRACE;
	ASSERT(width > 0.0f);
	ASSERT(height > 0.0f);
	ASSERT(depth > 0.0f);

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;
	float halfDepth = depth / 2.0f;

	Vector3 min = Vector3(center.x - halfWidth, center.y - halfHeight, center.z - halfDepth);
	Vector3 max = Vector3(center.x + halfWidth, center.y + halfHeight, center.z + halfDepth);

	SetBox(subsetIndex, firstTriangle, min, max);
}

void StaticMeshBuilder::SetBox(
	uint32_t subsetIndex,
	uint32_t firstTriangle,
	const Vector3 &min, const Vector3 &max
	)
{
	STACK_TRACE;
	// front
	SetQuad(
		subsetIndex, firstTriangle,
		Vector3(min.x, min.y, max.z), Vector3(max.x, min.y, max.z), Vector3(min.x, max.y, max.z), Vector3(max.x, max.y, max.z),
		Z_AXIS,                       Z_AXIS,                       Z_AXIS,                       Z_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);

	// back
	SetQuad(
		subsetIndex, firstTriangle + 2,
		Vector3(max.x, min.y, min.z), Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, min.z), Vector3(min.x, max.y, min.z),
		-Z_AXIS,                      -Z_AXIS,                      -Z_AXIS,                      -Z_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);

	// right
	SetQuad(
		subsetIndex, firstTriangle + 4,
		Vector3(max.x, min.y, max.z), Vector3(max.x, min.y, min.z), Vector3(max.x, max.y, max.z), Vector3(max.x, max.y, min.z),
		X_AXIS,                       X_AXIS,                       X_AXIS,                       X_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);

	// left
	SetQuad(
		subsetIndex, firstTriangle + 6,
		Vector3(min.x, min.y, min.z), Vector3(min.x, min.y, max.z), Vector3(min.x, max.y, min.z), Vector3(min.x, max.y, max.z),
		-X_AXIS,                      -X_AXIS,                      -X_AXIS,                      -X_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);

	// top
	SetQuad(
		subsetIndex, firstTriangle + 8,
		Vector3(min.x, max.y, max.z), Vector3(max.x, max.y, max.z), Vector3(min.x, max.y, min.z), Vector3(max.x, max.y, min.z),
		Y_AXIS,                       Y_AXIS,                       Y_AXIS,                       Y_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);

	// bottom
	SetQuad(
		subsetIndex, firstTriangle + 10,
		Vector3(max.x, min.y, max.z), Vector3(min.x, min.y, max.z), Vector3(max.x, min.y, min.z), Vector3(min.x, min.y, min.z),
		-Y_AXIS,                      -Y_AXIS,                      -Y_AXIS,                      -Y_AXIS,
		Vector2(0.0f, 1.0f),          Vector2(1.0f, 1.0f),          Vector2(0.0f, 0.0f),          Vector2(1.0f, 0.0f)
		);
}

void StaticMeshBuilder::GenerateNormals()
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > 0);

	for (uint32_t i = 0; i < m_subsets.size(); ++i)
	{
		StaticMeshSubset *subset = m_subsets[i];
		ASSERT(subset != NULL);

		VertexBuffer *buffer = subset->GetVertices();
		ASSERT(buffer->GetNumElements() % 3 == 0);

		// initialize all normals
		for (uint32_t n = 0; n < buffer->GetNumElements(); ++n)
			buffer->SetNormal(n, 0.0f, 0.0f, 0.0f);

		// calculate the normal for each triangle and add it to the normals for each individual vertex
		for (uint32_t v = 0; v < buffer->GetNumElements() / 3; ++v)
		{
			// calculate the triangle normal
			// they should have been added in CCW order (which StaticMeshBuilder does...)
			Vector3 a = buffer->GetPosition3(v * 3);
			Vector3 b = buffer->GetPosition3((v * 3) + 1);
			Vector3 c = buffer->GetPosition3((v * 3) + 2);
			Vector3 triangleNormal = Vector3::SurfaceNormal(a, b, c);

			// add it to the vertex normals for this triangle
			Vector3 na = buffer->GetNormal(v * 3);
			Vector3 nb = buffer->GetNormal((v * 3) + 1);
			Vector3 nc = buffer->GetNormal((v * 3) + 2);
			buffer->SetNormal(v * 3, na + triangleNormal);
			buffer->SetNormal((v * 3) + 1, nb + triangleNormal);
			buffer->SetNormal((v * 3) + 2, nc + triangleNormal);
		}

		// all the vertex normals will be way too long, need to normalize all of them now
		for (uint32_t n = 0; n < buffer->GetNumElements(); ++n)
		{
			Vector3 normal = buffer->GetNormal(n);
			buffer->SetNormal(n, Vector3::Normalize(normal));
		}
	}
}

StaticMesh* StaticMeshBuilder::BuildMesh()
{
	STACK_TRACE;
	ASSERT(m_subsets.size() > 0);

	// convert the mesh vector to an array so it can be passed into the mesh object
	StaticMeshSubset **subsets = new StaticMeshSubset*[m_subsets.size()];
	ASSERT(subsets != NULL);
	for (uint32_t i = 0; i < m_subsets.size(); ++i)
		subsets[i] = m_subsets[i];

	StaticMesh *mesh = new StaticMesh(m_subsets.size(), subsets);

	// the new StaticMesh object has now assumed responsibility for cleaning
	// up the memory allocated to the StaticMeshSubsets we passed to it.
	// remove the references we have to those objects so we don't delete the
	// memory twice accidentally
	Reset();

	return mesh;
}

void StaticMeshBuilder::SetTriangleInternal(
	VertexBuffer *buffer, 
	uint32_t bufferIndex,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
	const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
	)
{
	buffer->SetPosition3(bufferIndex, v1 * m_transform);
	buffer->SetNormal(bufferIndex, n1);
	buffer->SetTexCoord(bufferIndex, t1);

	buffer->SetPosition3(bufferIndex + 1, v2 * m_transform);
	buffer->SetNormal(bufferIndex + 1, n2);
	buffer->SetTexCoord(bufferIndex + 1, t2);

	buffer->SetPosition3(bufferIndex + 2, v3 * m_transform);
	buffer->SetNormal(bufferIndex + 2, n3);
	buffer->SetTexCoord(bufferIndex + 2, t3);
}

void StaticMeshBuilder::SetTriangleInternal(
	VertexBuffer *buffer, 
	uint32_t bufferIndex,
	const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
	const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
	)
{
	buffer->SetPosition3(bufferIndex, v1 * m_transform);
	buffer->SetTexCoord(bufferIndex, t1);

	buffer->SetPosition3(bufferIndex + 1, v2 * m_transform);
	buffer->SetTexCoord(bufferIndex + 1, t2);

	buffer->SetPosition3(bufferIndex + 2, v3 * m_transform);
	buffer->SetTexCoord(bufferIndex + 2, t3);
}
