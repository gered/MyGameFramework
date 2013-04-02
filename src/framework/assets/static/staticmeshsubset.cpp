#include "../../debug.h"

#include "staticmeshsubset.h"

#include "../../graphics/texture.h"
#include "../../graphics/vertexbuffer.h"

StaticMeshSubset::StaticMeshSubset(uint32_t numTriangles, Texture *texture)
{
	VERTEX_ATTRIBS attribs[] = {
		VERTEX_POS_3D,
		VERTEX_NORMAL,
		VERTEX_TEXCOORD
	};
	
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(attribs, 3, numTriangles * 3, BUFFEROBJECT_USAGE_STATIC);
	m_texture = texture;
}

StaticMeshSubset::~StaticMeshSubset()
{
	SAFE_DELETE(m_vertices);
}
