#include "../debug.h"

#include "indexbuffer.h"

#include <string.h>

IndexBuffer::IndexBuffer()
{
	m_currentIndex = 0;
}

void IndexBuffer::Release()
{
	m_buffer.clear();
	stl::vector<uint16_t>().swap(m_buffer);
	m_currentIndex = 0;
	
	BufferObject::Release();
}

BOOL IndexBuffer::Initialize(uint32_t numIndices, BUFFEROBJECT_USAGE usage)
{
	return Initialize(NULL, numIndices, usage);
}

BOOL IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, uint32_t numIndices, BUFFEROBJECT_USAGE usage)
{
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
	return Initialize(NULL, source);
}

BOOL IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, const IndexBuffer *source)
{
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
	
	return TRUE;
}

void IndexBuffer::Set(const uint16_t *indices, uint32_t numIndices)
{
	memcpy(&m_buffer[0], indices, GetNumElements() * GetElementWidthInBytes());
}

void IndexBuffer::Resize(uint32_t numIndices)
{
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
	uint32_t newSize = GetNumElements() + amount;
	Resize(newSize);
}

