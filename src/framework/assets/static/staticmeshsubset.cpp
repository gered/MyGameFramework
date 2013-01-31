#include "../../debug.h"

#include "staticmeshsubset.h"

#include "../../graphics/texture.h"
#include "../../graphics/vertexbuffer.h"

StaticMeshSubset::StaticMeshSubset(uint32_t numTriangles, Texture *texture)
{
	STACK_TRACE;
	m_vertices = new VertexBuffer(BUFFEROBJECT_USAGE_STATIC);
	ASSERT(m_vertices != NULL);
	m_vertices->AddAttribute(VERTEX_POS_3D);
	m_vertices->AddAttribute(VERTEX_NORMAL);
	m_vertices->AddAttribute(VERTEX_TEXCOORD);
	m_vertices->Create(numTriangles * 3);
	m_texture = texture;
}

StaticMeshSubset::~StaticMeshSubset()
{
	STACK_TRACE;
	SAFE_DELETE(m_vertices);
}
