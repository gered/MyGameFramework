#include "../debug.h"

#include "../common.h"
#include "renderbuffer.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicsdevice.h"
#include "../math/mathhelpers.h"

Renderbuffer::Renderbuffer()
{
	STACK_TRACE;
	m_renderbufferName = 0;
	m_width = 0;
	m_height = 0;
	m_type = FRAMEBUFFER_DATA_NONE;
}

Renderbuffer::~Renderbuffer()
{
	STACK_TRACE;
	Release();
}

BOOL Renderbuffer::Create(GraphicsDevice *graphicsDevice, uint16_t width, uint16_t height, FRAMEBUFFER_DATA_TYPE type)
{
	STACK_TRACE;
	ASSERT(m_renderbufferName == 0);
	if (m_renderbufferName != 0)
		return FALSE;
	
	ASSERT(graphicsDevice != NULL);
	
	uint32_t format = 0;
#ifdef MOBILE
	switch (type)
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
	switch (type)
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
	
	m_width = width;
	m_height = height;
	m_type = type;
	
	m_graphicsDevice->BindRenderbuffer(this);
	
	// have OpenGL allocate the renderbuffer's storage
	GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, format, width, height));

	// don't leave this buffer bound, we'll let an associated Framebuffer
	// object that this will get attached to manage that for itself...
	m_graphicsDevice->UnbindRenderbuffer();
	
	return TRUE;
}

void Renderbuffer::Release()
{
	STACK_TRACE;
	if (m_renderbufferName != 0)
	{
		m_graphicsDevice->UnbindRenderBuffer(this);
		GL_CALL(glDeleteRenderbuffers(1, &m_renderbufferName));
	}

	m_graphicsDevice = NULL;
	m_renderbufferName = 0;
	m_width = 0;
	m_height = 0;
}

void Renderbuffer::OnNewContext()
{
	STACK_TRACE;
	if (m_renderbufferName == 0 && m_graphicsDevice != NULL)
	{
		BOOL success = Create(m_graphicsDevice, m_width, m_height, m_type);
		ASSERT(success == TRUE);
	}
}

void Renderbuffer::OnLostContext()
{
	STACK_TRACE;
	m_renderbufferName = 0;
}
