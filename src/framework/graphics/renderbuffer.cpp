#include "../debug.h"

#include "../common.h"
#include "renderbuffer.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicsdevice.h"
#include "../math/mathhelpers.h"

Renderbuffer::Renderbuffer()
{
	m_renderbufferName = 0;
	m_width = 0;
	m_height = 0;
	m_type = FRAMEBUFFER_DATA_NONE;
}

BOOL Renderbuffer::Initialize(GraphicsDevice *graphicsDevice, uint width, uint height, FRAMEBUFFER_DATA_TYPE type)
{
	ASSERT(m_renderbufferName == 0);
	if (m_renderbufferName != 0)
		return FALSE;
	
	if (!GraphicsContextResource::Initialize(graphicsDevice))
		return FALSE;
	
	m_width = width;
	m_height = height;
	m_type = type;
	
	BOOL success = CreateRenderbuffer();
	if (!success)
	{
		m_width = 0;
		m_height = 0;
		m_type = FRAMEBUFFER_DATA_NONE;
	}
	
	return success;
}

void Renderbuffer::Release()
{
	if (m_renderbufferName != 0)
	{
		GetGraphicsDevice()->UnbindRenderBuffer(this);
		GL_CALL(glDeleteRenderbuffers(1, &m_renderbufferName));
	}

	m_renderbufferName = 0;
	m_width = 0;
	m_height = 0;
	
	GraphicsContextResource::Release();
}

BOOL Renderbuffer::CreateRenderbuffer()
{
	ASSERT(m_renderbufferName == 0);
	
	uint format = 0;
#ifdef MOBILE
	switch (m_type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
			format = GL_RGB565;
			break;
		case FRAMEBUFFER_DATA_COLOR_RGBA:
			format = GL_RGBA4;
			break;
		case FRAMEBUFFER_DATA_DEPTH:
			format = GL_DEPTH_COMPONENT16;
			break;
		case FRAMEBUFFER_DATA_STENCIL:
			format = GL_STENCIL_INDEX8;
		default: break;
	};
#else
	switch (m_type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
			format = GL_RGB;
			break;
		case FRAMEBUFFER_DATA_COLOR_RGBA:
			format = GL_RGBA;
			break;
		case FRAMEBUFFER_DATA_DEPTH:
			format = GL_DEPTH_COMPONENT;
			break;
		case FRAMEBUFFER_DATA_STENCIL:
			format = GL_STENCIL_INDEX;
			break;
		default: break;
	};
#endif
	ASSERT(format != 0);
	if (format == 0)
		return FALSE;
	
	GL_CALL(glGenRenderbuffers(1, &m_renderbufferName));
	
	GetGraphicsDevice()->BindRenderbuffer(this);
	
	// have OpenGL allocate the renderbuffer's storage
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, m_width, m_height));
	
	// don't leave this buffer bound, we'll let an associated Framebuffer
	// object that this will get attached to manage that for itself...
	GetGraphicsDevice()->UnbindRenderbuffer();
	
	return TRUE;
}

void Renderbuffer::OnNewContext()
{
	if (m_renderbufferName == 0 && GetGraphicsDevice() != NULL)
	{
		BOOL success = CreateRenderbuffer();
		ASSERT(success == TRUE);
	}
}

void Renderbuffer::OnLostContext()
{
	m_renderbufferName = 0;
}
