#include "../debug.h"

#include "bufferobject.h"
#include "glincludes.h"
#include "glutils.h"

BufferObject::BufferObject()
{
	m_type = BUFFEROBJECT_TYPE_VERTEX;
	m_usage = BUFFEROBJECT_USAGE_STATIC;
	m_bufferId = 0;
	m_isDirty = false;
	m_sizeInBytes = 0;
}

void BufferObject::Release()
{
	if (!IsClientSideBuffer())
		FreeBufferObject();

	m_type = BUFFEROBJECT_TYPE_VERTEX;
	m_usage = BUFFEROBJECT_USAGE_STATIC;
	m_bufferId = 0;
	m_isDirty = false;
	m_sizeInBytes = 0;
	
	GraphicsContextResource::Release();
}

bool BufferObject::Initialize(GraphicsDevice *graphicsDevice, BUFFEROBJECT_TYPE type, BUFFEROBJECT_USAGE usage)
{
	bool success = true;
	if (graphicsDevice != NULL)
		success = GraphicsContextResource::Initialize(graphicsDevice);
	else
		success = GraphicsContextResource::Initialize();
	
	m_type = type;
	m_usage = usage;
	
	return success;
}

void BufferObject::CreateOnGpu()
{
	ASSERT(IsClientSideBuffer() == true);
	CreateBufferObject();
}

void BufferObject::RecreateOnGpu()
{
	ASSERT(IsClientSideBuffer() == false);
	FreeBufferObject();
	CreateBufferObject();
}

void BufferObject::FreeFromGpu()
{
	ASSERT(IsClientSideBuffer() == false);
	FreeBufferObject();
}

void BufferObject::CreateBufferObject()
{
	GL_CALL(glGenBuffers(1, &m_bufferId));
	SizeBufferObject();

	m_isDirty = true;
}

void BufferObject::FreeBufferObject()
{
	ASSERT(m_bufferId != 0);
	GL_CALL(glDeleteBuffers(1, &m_bufferId));

	m_bufferId = 0;
	m_isDirty = false;
	m_sizeInBytes = 0;
}

void BufferObject::Update()
{
	ASSERT(IsClientSideBuffer() == false);
	ASSERT(IsDirty() == true);
	ASSERT(GetNumElements() > 0);
	ASSERT(GetElementWidthInBytes() > 0);

	size_t currentSizeInBytes = GetNumElements() * GetElementWidthInBytes();

	GLenum usage = 0;
	if (m_usage == BUFFEROBJECT_USAGE_STATIC)
		usage = GL_STATIC_DRAW;
	else if (m_usage == BUFFEROBJECT_USAGE_STREAM)
		usage = GL_STREAM_DRAW;
	else if (m_usage == BUFFEROBJECT_USAGE_DYNAMIC)
		usage = GL_DYNAMIC_DRAW;
	ASSERT(usage != 0);

	GLenum target = 0;
	if (m_type == BUFFEROBJECT_TYPE_INDEX)
		target = GL_ELEMENT_ARRAY_BUFFER;
	else if (m_type == BUFFEROBJECT_TYPE_VERTEX)
		target = GL_ARRAY_BUFFER;
	ASSERT(target != 0);

	GL_CALL(glBindBuffer(target, m_bufferId));

	if (m_sizeInBytes != currentSizeInBytes)
	{
		// means that the buffer object hasn't been allocated. So let's allocate and update at the same time
		// figure out the size...
		m_sizeInBytes = currentSizeInBytes;

		// and then allocate + update
		GL_CALL(glBufferData(target, m_sizeInBytes, GetBuffer(), usage));
	}
	else
	{
		// possible performance enhancement? passing a NULL pointer to
		// glBufferData tells the driver that we don't care about the buffer's
		// previous contents allowing it to do some extra optimizations which is
		// fine since our glBufferSubData call is going to completely replace 
		// the contents anyway
		GL_CALL(glBufferData(target, m_sizeInBytes, NULL, usage));

		GL_CALL(glBufferSubData(target, 0, m_sizeInBytes, GetBuffer()));
	}

	GL_CALL(glBindBuffer(target, 0));

	m_isDirty = false;
}

void BufferObject::SizeBufferObject()
{
	ASSERT(IsClientSideBuffer() == false);
	ASSERT(GetNumElements() > 0);
	ASSERT(GetElementWidthInBytes() > 0);

	GLenum usage = 0;
	if (m_usage == BUFFEROBJECT_USAGE_STATIC)
		usage = GL_STATIC_DRAW;
	else if (m_usage == BUFFEROBJECT_USAGE_STREAM)
		usage = GL_STREAM_DRAW;
	else if (m_usage == BUFFEROBJECT_USAGE_DYNAMIC)
		usage = GL_DYNAMIC_DRAW;
	ASSERT(usage != 0);

	GLenum target = 0;
	if (m_type == BUFFEROBJECT_TYPE_INDEX)
		target = GL_ELEMENT_ARRAY_BUFFER;
	else if (m_type == BUFFEROBJECT_TYPE_VERTEX)
		target = GL_ARRAY_BUFFER;
	ASSERT(target != 0);

	m_sizeInBytes = GetNumElements() * GetElementWidthInBytes();

	// resize the buffer object without initializing it's data
	GL_CALL(glBindBuffer(target, m_bufferId));
	GL_CALL(glBufferData(target, m_sizeInBytes, NULL, usage));
	GL_CALL(glBindBuffer(target, 0));

	m_isDirty = true;
}

void BufferObject::OnNewContext()
{
	RecreateOnGpu();
}

void BufferObject::OnLostContext()
{
}
