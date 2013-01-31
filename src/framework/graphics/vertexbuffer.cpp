#include "../debug.h"

#include "glincludes.h"
#include "glutils.h"
#include "vertexbuffer.h"

const unsigned int FLOATS_PER_GPU_ATTRIB_SLOT = 4;
const unsigned int MAX_GPU_ATTRIB_SLOTS = 8;

VertexBuffer::VertexBuffer(BUFFEROBJECT_USAGE usage)
	: BufferObject(BUFFEROBJECT_TYPE_VERTEX, usage)
{
	STACK_TRACE;
	m_numVertices = 0;
	m_currentVertex = 0;
	m_standardTypeAttribs = 0;
	m_elementWidth = 0;
	m_colorOffset = 0;
	m_position2Offset = 0;
	m_position3Offset = 0;
	m_normalOffset = 0;
	m_texCoordOffset = 0;
	m_numGPUAttributeSlotsUsed = 0;
}

VertexBuffer::~VertexBuffer()
{
	STACK_TRACE;
}

BOOL VertexBuffer::AddAttribute(uint32_t size, VERTEX_ATTRIBS standardType)
{
	STACK_TRACE;
	ASSERT(standardType == VERTEX_GENERIC || HasStandardAttrib(standardType) == FALSE);
	ASSERT(size <= 16);

	if (standardType != VERTEX_GENERIC && HasStandardAttrib(standardType))
		return FALSE;
	if (size > 16)
		return FALSE;

	// using integer division that rounds up (so given size = 13, result is 4, not 3)
	uint32_t numGPUAttributeSlotsUsed = (size + (FLOATS_PER_GPU_ATTRIB_SLOT - 1)) / FLOATS_PER_GPU_ATTRIB_SLOT;
	ASSERT(m_numGPUAttributeSlotsUsed + numGPUAttributeSlotsUsed <= MAX_GPU_ATTRIB_SLOTS);
	if (m_numGPUAttributeSlotsUsed + numGPUAttributeSlotsUsed > MAX_GPU_ATTRIB_SLOTS)
		return FALSE;

	VertexBufferAttribute newAttrib;
	newAttrib.standardType = standardType;
	newAttrib.size = size;
	newAttrib.offset = m_elementWidth;

	switch (standardType)
	{
	case VERTEX_POS_2D:
		ASSERT(size == 2);
		if (size != 2)
			return FALSE;
		m_position2Offset = newAttrib.offset;
		break;
	case VERTEX_POS_3D:
		ASSERT(size == 3);
		if (size != 3)
			return FALSE;
		m_position3Offset = newAttrib.offset;
		break;
	case VERTEX_NORMAL:
		ASSERT(size == 3);
		if (size != 3)
			return FALSE;
		m_normalOffset = newAttrib.offset;
		break;
	case VERTEX_COLOR:
		ASSERT(size == 4);
		if (size != 4)
			return FALSE;
		m_colorOffset = newAttrib.offset;
		break;
	case VERTEX_TEXCOORD:
		ASSERT(size == 2);
		if (size != 2)
			return FALSE;
		m_texCoordOffset = newAttrib.offset;
		break;
	case VERTEX_GENERIC:
		break;
	}

	m_attribs.push_back(newAttrib);

	m_elementWidth += size;
	m_numGPUAttributeSlotsUsed += numGPUAttributeSlotsUsed;
	SetBit(standardType, m_standardTypeAttribs);

	return TRUE;
}

BOOL VertexBuffer::AddAttribute(VERTEX_ATTRIBS standardType)
{
	STACK_TRACE;
	ASSERT(standardType != VERTEX_GENERIC);
	if (standardType == VERTEX_GENERIC)
		return FALSE;

	ASSERT(HasStandardAttrib(standardType) == FALSE);
	if (HasStandardAttrib(standardType))
		return FALSE;

	BOOL result = FALSE;

	switch (standardType)
	{
	case VERTEX_POS_2D:
		result = AddAttribute(ATTRIB_SIZE_VEC2, VERTEX_POS_2D);
		break;
	case VERTEX_POS_3D:
		result = AddAttribute(ATTRIB_SIZE_VEC3, VERTEX_POS_3D);
		break;
	case VERTEX_NORMAL:
		result = AddAttribute(ATTRIB_SIZE_VEC3, VERTEX_NORMAL);
		break;
	case VERTEX_COLOR:
		result = AddAttribute(ATTRIB_SIZE_VEC4, VERTEX_COLOR);
		break;
	case VERTEX_TEXCOORD:
		result = AddAttribute(ATTRIB_SIZE_VEC2, VERTEX_TEXCOORD);
		break;
	case VERTEX_GENERIC:
		break;
	}

	return result;
}

BOOL VertexBuffer::CopyAttributesFrom(const VertexBuffer *source)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	if (source == NULL)
		return FALSE;

	ASSERT(source->GetNumAttributes() != 0);
	ASSERT(m_buffer.size() == 0);

	if (source->GetNumAttributes() == 0)
		return FALSE;
	if (m_buffer.size() > 0)
		return FALSE;

	m_attribs.clear();
	for (uint32_t i = 0; i < source->GetNumAttributes(); ++i)
	{
		const VertexBufferAttribute *sourceAttrib = source->GetAttributeInfo(i);
		AddAttribute(sourceAttrib->size, sourceAttrib->standardType);
	}

	return TRUE;
}

int32_t VertexBuffer::GetIndexOfStandardAttrib(VERTEX_ATTRIBS standardAttrib) const
{
	STACK_TRACE;
	ASSERT(standardAttrib != VERTEX_GENERIC);
	if (standardAttrib == VERTEX_GENERIC)
		return -1;

	for (uint32_t i = 0; i < m_attribs.size(); ++i)
	{
		if (m_attribs[i].standardType == standardAttrib)
			return (int32_t)i;
	}

	return -1;
}

BOOL VertexBuffer::Create(uint32_t numVertices)
{
	STACK_TRACE;
	ASSERT(m_attribs.size() > 0);
	ASSERT(m_buffer.size() == 0);
	ASSERT(m_elementWidth > 0);

	if (m_attribs.size() == 0)
		return FALSE;
	if (m_buffer.size() > 0)
		return FALSE;
	if (m_elementWidth == 0)
		return FALSE;

	Resize(numVertices);

	return TRUE;
}

BOOL VertexBuffer::CreateCopyOf(const VertexBuffer *source)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	if (source == NULL)
		return FALSE;

	ASSERT(source->GetNumElements() != 0);
	ASSERT(m_buffer.size() == 0);

	if (source->GetNumElements() == 0)
		return FALSE;
	if (m_buffer.size() > 0)
		return FALSE;

	BOOL attribCopyResult = CopyAttributesFrom(source);
	if (!attribCopyResult)
		return FALSE;

	Resize(source->GetNumElements());

	memcpy(&m_buffer[0], source->GetBuffer(), GetNumElements() * GetElementWidthInBytes());

	return TRUE;
}


void VertexBuffer::Resize(uint32_t numVertices)
{
	STACK_TRACE;
	m_buffer.resize(numVertices * m_elementWidth, 0.0f);
	m_numVertices = numVertices;

	if (!IsClientSideBuffer())
		SizeBufferObject();

	if (m_currentVertex >= m_numVertices)
		--m_currentVertex;
}

void VertexBuffer::Extend(uint32_t amount)
{
	STACK_TRACE;
	uint32_t newSize = GetNumElements() + amount;
	Resize(newSize);
}

void VertexBuffer::Copy(const VertexBuffer *source, uint32_t destIndex)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	ASSERT(source->GetNumElements() > 0);
	ASSERT(source->GetStandardAttribs() == m_standardTypeAttribs);
	ASSERT(destIndex >= 0);
	ASSERT(destIndex + source->GetNumElements() <= GetNumElements());

	uint32_t destOffset = GetVertexPosition(destIndex);
	memcpy(&m_buffer[destOffset], source->GetBuffer(), GetNumElements() * GetElementWidthInBytes());

	SetDirty();
}
