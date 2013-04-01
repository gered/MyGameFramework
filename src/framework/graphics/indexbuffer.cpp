#include "../debug.h"

#include "indexbuffer.h"

#include <string.h>

IndexBuffer::IndexBuffer()
{
	STACK_TRACE;
	m_currentIndex = 0;
}

IndexBuffer::~IndexBuffer()
{
	STACK_TRACE;
}

BOOL IndexBuffer::Initialize(uint32_t numIndices, BUFFEROBJECT_USAGE usage)
{
	STACK_TRACE;
	return Initialize(NULL, numIndices, usage);
}

BOOL IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, uint32_t numIndices, BUFFEROBJECT_USAGE usage)
{
	STACK_TRACE;
	ASSERT(m_buffer.size() == 0);
	if (m_buffer.size() > 0)
		return FALSE;

	ASSERT(numIndices > 0);
	if (numIndices == 0)
		return FALSE;
		
	if (!BufferObject::Initialize(graphicsDevice, BUFFEROBJECT_TYPE_INDEX, usage))
		return FALSE;
	
	Resize(numIndices);
	
	return TRUE;
}

BOOL IndexBuffer::Initialize(const IndexBuffer *source)
{
	STACK_TRACE;
	return Initialize(NULL, source);
}

BOOL IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, const IndexBuffer *source)
{
	STACK_TRACE;
	ASSERT(m_buffer.size() == 0);
	if (m_buffer.size() > 0)
		return FALSE;

	ASSERT(source != NULL);
	if (source == NULL)
		return FALSE;
	
	ASSERT(source->GetNumElements() > 0);
	if (source->GetNumElements() == 0)
		return FALSE;
	
	Resize(source->GetNumElements());
	
	memcpy(&m_buffer[0], source->GetBuffer(), GetNumElements() * GetElementWidthInBytes());	
}

void IndexBuffer::Set(const uint16_t *indices, uint32_t numIndices)
{
	STACK_TRACE;
	memcpy(&m_buffer[0], indices, GetNumElements() * GetElementWidthInBytes());
}

void IndexBuffer::Resize(uint32_t numIndices)
{
	STACK_TRACE;
	ASSERT(numIndices > 0);
	if (numIndices == 0)
		return;
	
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

