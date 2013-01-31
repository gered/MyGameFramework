#include "../../debug.h"

#include "../../common.h"
#include "skeletalmeshsubset.h"
#include "../../graphics/indexbuffer.h"
#include <stl/string.h>

SkeletalMeshSubset::SkeletalMeshSubset()
{
	STACK_TRACE;
	m_indices = NULL;
	m_alpha = FALSE;
}

SkeletalMeshSubset::~SkeletalMeshSubset()
{
	STACK_TRACE;
	SAFE_DELETE(m_indices);
}

void SkeletalMeshSubset::Create(const stl::string &name, uint32_t numTriangles, BOOL alpha)
{
	STACK_TRACE;
	ASSERT(m_indices == NULL);
	ASSERT(numTriangles > 0);
	m_name = name;
	m_indices = new IndexBuffer(numTriangles * 3, TRUE);
	m_alpha = alpha;
}
