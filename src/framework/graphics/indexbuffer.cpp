#include "../debug.h"

#include "indexbuffer.h"

#include <string.h>

IndexBuffer::IndexBuffer(uint32_t numIndices, BOOL isStatic)
	: BufferObject(BUFFEROBJECT_TYPE_INDEX, isStatic ? BUFFEROBJECT_USAGE_STATIC : BUFFEROBJECT_USAGE_DYNAMIC)
{
	STACK_TRACE;
	m_currentIndex = 0;
	Resize(numIndices);
}

IndexBuffer::IndexBuffer(const IndexBuffer *source)
	: BufferObject(BUFFEROBJECT_TYPE_INDEX, source->GetUsage())
{
	STACK_TRACE;
	ASSERT(source != NULL);
	ASSERT(source->GetNumElements() > 0);

	m_currentIndex = 0;
	Resize(source->GetNumElements());

	memcpy(&m_buffer[0], source->GetBuffer(), GetNumElements() * GetElementWidthInBytes());
}

IndexBuffer::~IndexBuffer()
{
	STACK_TRACE;
}

void IndexBuffer::Set(const uint16_t *indices, uint32_t numIndices)
{
	STACK_TRACE;
	memcpy(&m_buffer[0], indices, GetNumElements() * GetElementWidthInBytes());
}

void IndexBuffer::Resize(uint32_t numIndices)
{
	STACK_TRACE;
	m_buffer.resize(numIndices, 0);

	if (!IsClientSideBuffer())
		SizeBufferObject();

	if (m_currentIndex >= GetNumElements())
		--m_currentIndex;
}

void IndexBuffer::Extend(uint32_t amount)
{
	STACK_TRACE;
	uint32_t newSize = GetNumElements() + amount;
	Resize(newSize);
}

