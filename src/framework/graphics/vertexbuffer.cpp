#include "../debug.h"

#include "glincludes.h"
#include "glutils.h"
#include "vertexbuffer.h"

const unsigned int FLOATS_PER_GPU_ATTRIB_SLOT = 4;
const unsigned int MAX_GPU_ATTRIB_SLOTS = 8;

VertexBuffer::VertexBuffer()
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
	m_numAttributes = 0;
	m_attribs = NULL;
	m_numGPUAttributeSlotsUsed = 0;
}

VertexBuffer::~VertexBuffer()
{
	STACK_TRACE;
}

BOOL VertexBuffer::Initialize(const VERTEX_ATTRIBS *attributes, uint32_t numAttributes, uint32_t numVertices, BUFFEROBJECT_USAGE usage)
{
	STACK_TRACE;
	return Initialize(NULL, attributes, numAttributes, numVertices, usage);
}

BOOL VertexBuffer::Initialize(GraphicsDevice *graphicsDevice, const VERTEX_ATTRIBS *attributes, uint32_t numAttributes, uint32_t numVertices, BUFFEROBJECT_USAGE usage)
{
	STACK_TRACE;
	ASSERT(m_buffer.size() == 0);
	if (m_buffer.size() > 0)
		return FALSE;
	
	if (!BufferObject::Initialize(graphicsDevice, BUFFEROBJECT_TYPE_VERTEX, usage))
		return FALSE;
	
	if (!SetSizesAndOffsets(attributes, numAttributes))
		return FALSE;
	
	Resize(numVertices);
	
	return TRUE;
}

BOOL VertexBuffer::Initialize(const VertexBuffer *source)
{
	STACK_TRACE;
	return Initialize(NULL, source);
}

BOOL VertexBuffer::Initialize(GraphicsDevice *graphicsDevice, const VertexBuffer *source)
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
	
	if (!BufferObject::Initialize(graphicsDevice, BUFFEROBJECT_TYPE_VERTEX, source->GetUsage()))
		return FALSE;
	
	uint32_t numAttribs = source->GetNumAttributes();
	VERTEX_ATTRIBS *attribs = new VERTEX_ATTRIBS[numAttribs];
	for (uint32_t i = 0; i < numAttribs; ++i)
		attribs[i] = source->GetAttributeInfo(i)->standardType;
	
	BOOL success = SetSizesAndOffsets(attribs, numAttribs);
	if (success)
	{
		Resize(source->GetNumElements());
		Copy(source, 0);
	}
	
	SAFE_DELETE_ARRAY(attribs);
	
	return success;
}

BOOL VertexBuffer::SetSizesAndOffsets(const VERTEX_ATTRIBS *attributes, uint32_t numAttributes)
{
	STACK_TRACE;
	ASSERT(attributes != NULL);
	ASSERT(numAttributes > 0);
	ASSERT(m_buffer.size() == 0);
	ASSERT(m_attribs == NULL);
	
	if (attributes == NULL)
		return FALSE;
	if (numAttributes == 0)
		return FALSE;
	if (m_buffer.size() > 0)
		return FALSE;
	if (m_attribs != NULL)
		return FALSE;
	
	uint32_t numGpuSlotsUsed = 0;
	uint32_t offset = 0;
	
	VertexBufferAttribute *attribsInfo = new VertexBufferAttribute[numAttributes];
	BOOL success = TRUE;
	
	for (uint32_t i = 0; i < numAttributes; ++i)
	{
		VERTEX_ATTRIBS attrib = attributes[i];
		
		// TODO: endianness
		uint8_t size = (uint8_t)attrib;  // low byte
		uint8_t standardTypeBitMask = (uint8_t)((uint16_t)attrib >> 8);
		
		// using integer division that rounds up (so given size = 13, result is 4, not 3)
		uint32_t thisAttribsGpuSlotSize = ((uint32_t)size + (FLOATS_PER_GPU_ATTRIB_SLOT - 1)) / FLOATS_PER_GPU_ATTRIB_SLOT;
		ASSERT(numGpuSlotsUsed + thisAttribsGpuSlotSize <= MAX_GPU_ATTRIB_SLOTS);
		if (numGpuSlotsUsed + thisAttribsGpuSlotSize > MAX_GPU_ATTRIB_SLOTS)
		{
			success = FALSE;
			break;
		}
		
		if (standardTypeBitMask > 0)
		{
			// ensure no duplicate standard attribute types are specified
			ASSERT(IsBitSet(standardTypeBitMask, m_standardTypeAttribs) == FALSE);
			if (IsBitSet(standardTypeBitMask, m_standardTypeAttribs))
			{
				success = FALSE;
				break;
			}
			
			SetBit(standardTypeBitMask, m_standardTypeAttribs);
			
			// record offset position for each standard type attribute
			switch ((VERTEX_STANDARD_ATTRIBS)attrib)
			{
				case VERTEX_STD_POS_2D:     m_position2Offset = offset; break;
				case VERTEX_STD_POS_3D:     m_position3Offset = offset; break;
				case VERTEX_STD_NORMAL:     m_normalOffset = offset; break;
				case VERTEX_STD_COLOR:      m_colorOffset = offset; break;
				case VERTEX_STD_TEXCOORD:   m_texCoordOffset = offset; break;
			}
		}

		// set attribute info
		attribsInfo[i].offset = offset;
		attribsInfo[i].size = size;
		attribsInfo[i].standardType = attrib;
		
		// advance to the next spot
		m_elementWidth += size;
		offset += size;
		numGpuSlotsUsed += thisAttribsGpuSlotSize;
	}
	
	if (!success)
	{
		SAFE_DELETE_ARRAY(attribsInfo)
		m_attribs = NULL;
		m_numAttributes = 0;
		m_elementWidth = 0;
		
		m_position2Offset = 0;
		m_position3Offset = 0;
		m_normalOffset = 0;
		m_colorOffset = 0;
		m_texCoordOffset = 0;
	}
	else
	{
		m_attribs = attribsInfo;
		m_numAttributes = numAttributes;
	}
	
	return success;
}

int32_t VertexBuffer::GetIndexOfStandardAttrib(VERTEX_STANDARD_ATTRIBS standardAttrib) const
{
	STACK_TRACE;
	for (uint32_t i = 0; i < m_numAttributes; ++i)
	{
		if ((uint32_t)m_attribs[i].standardType == (uint32_t)standardAttrib)
			return (int32_t)i;
	}

	return -1;
}

void VertexBuffer::Resize(uint32_t numVertices)
{
	STACK_TRACE;
	ASSERT(numVertices > 0);
	if (numVertices == 0)
		return;
	
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
