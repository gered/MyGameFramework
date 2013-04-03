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

bool IndexBuffer::Initialize(uint numIndices, BUFFEROBJECT_USAGE usage)
{
	return Initialize(NULL, numIndices, usage);
}

bool IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, uint numIndices, BUFFEROBJECT_USAGE usage)
{
	ASSERT(m_buffer.size() == 0);
	if (m_buffer.size() > 0)
		return false;

	ASSERT(numIndices > 0);
	if (numIndices == 0)
		return false;
		
	if (!BufferObject::Initialize(graphicsDevice, BUFFEROBJECT_TYPE_INDEX, usage))
		return false;
	
	Resize(numIndices);
	
	if (graphicsDevice != NULL)
		CreateOnGpu();
	
	return true;
}

bool IndexBuffer::Initialize(const IndexBuffer *source)
{
	return Initialize(NULL, source);
}

bool IndexBuffer::Initialize(GraphicsDevice *graphicsDevice, const IndexBuffer *source)
{
	ASSERT(m_buffer.size() == 0);
	if (m_buffer.size() > 0)
		return false;

	ASSERT(source != NULL);
	if (source == NULL)
		return false;
	
	ASSERT(source->GetNumElements() > 0);
	if (source->GetNumElements() == 0)
		return false;
	
	if (!BufferObject::Initialize(graphicsDevice, BUFFEROBJECT_TYPE_INDEX, source->GetUsage()))
		return false;
	
	Resize(source->GetNumElements());
	
	memcpy(&m_buffer[0], source->GetBuffer(), GetNumElements() * GetElementWidthInBytes());
	
	if (graphicsDevice != NULL)
		CreateOnGpu();
	
	return true;
}

void IndexBuffer::Set(const uint16_t *indices, uint numIndices)
{
	memcpy(&m_buffer[0], indices, GetNumElements() * GetElementWidthInBytes());
}

void IndexBuffer::Resize(uint numIndices)
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

void IndexBuffer::Extend(uint amount)
{
	uint newSize = GetNumElements() + amount;
	Resize(newSize);
}

