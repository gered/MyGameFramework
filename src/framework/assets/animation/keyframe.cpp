#include "../../debug.h"

#include "keyframe.h"

#include "../../math/vector3.h"

Keyframe::Keyframe(uint32_t numVertices)
{
	AllocateMemory(numVertices);
}

Keyframe::Keyframe(const stl::string &name, uint32_t numVertices)
{
	m_name = name;
	AllocateMemory(numVertices);
}

void Keyframe::AllocateMemory(uint32_t numVertices)
{
	m_numVertices = numVertices;
	m_vertices = new Vector3[m_numVertices];
	ASSERT(m_vertices != NULL);
	m_normals = new Vector3[m_numVertices];
	ASSERT(m_normals != NULL);
}

Keyframe::~Keyframe()
{
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE_ARRAY(m_normals);
}
