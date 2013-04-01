#include "../debug.h"

#include "../common.h"
#include "framebuffer.h"
#include "framebufferdatatypes.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicsdevice.h"
#include "renderbuffer.h"
#include "texture.h"
#include "viewcontext.h"
#include "../gamewindow.h"
#include "../math/rect.h"
#include <stl/map.h>

Framebuffer::Framebuffer()
{
	STACK_TRACE;
	m_viewContext = NULL;
	m_framebufferName = 0;
	m_fixedWidth = 0;
	m_fixedHeight = 0;
}

BOOL Framebuffer::Initialize(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	ASSERT(m_framebufferName == 0);
	if (m_framebufferName != 0)
		return FALSE;
	
	if (!GraphicsContextResource::Initialize(graphicsDevice))
		return FALSE;
	
	CreateFramebuffer();
	
	m_viewContext = NULL;
	m_fixedWidth = 0;
	m_fixedHeight = 0;
	
	return TRUE;
}

BOOL Framebuffer::Initialize(GraphicsDevice *graphicsDevice, uint16_t fixedWidth, uint16_t fixedHeight)
{
	STACK_TRACE;
	ASSERT(fixedWidth != 0);
	ASSERT(fixedHeight != 0);
	if (fixedWidth == 0 || fixedHeight == 0)
		return FALSE;
	
	BOOL createSuccess = Initialize(graphicsDevice);
	if (!createSuccess)
		return FALSE;
	
	m_fixedWidth = fixedWidth;
	m_fixedHeight = fixedHeight;
	
	return TRUE;
}

void Framebuffer::Release()
{
	STACK_TRACE;
	if (m_framebufferName != 0)
	{
		for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		{
			Renderbuffer *renderbuffer = i->second;
			SAFE_DELETE(renderbuffer);
		}
		m_renderbuffers.clear();
		
		for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
		{
			Texture *texture = i->second;
			SAFE_DELETE(texture);
		}
		m_textures.clear();
		
		GL_CALL(glDeleteFramebuffers(1, &m_framebufferName));
	}
	
	if (GetGraphicsDevice() != NULL)
	{
		if (GetGraphicsDevice()->GetViewContext() == m_viewContext)
			GetGraphicsDevice()->SetViewContext(NULL);
		
		SAFE_DELETE(m_viewContext);
	}
	
	m_framebufferName = 0;
	m_fixedWidth = 0;
	m_fixedHeight = 0;
}

void Framebuffer::CreateFramebuffer()
{
	STACK_TRACE;
	ASSERT(m_framebufferName == 0);
	GL_CALL(glGenFramebuffers(1, &m_framebufferName));
}

BOOL Framebuffer::AttachViewContext()
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	ASSERT(m_viewContext == NULL);
	if (m_viewContext != NULL)
		return FALSE;
	
	m_viewContext = new ViewContext();
	BOOL success;
	if (IsUsingFixedDimensions())
		success = m_viewContext->Create(GetGraphicsDevice(), Rect(0, 0, m_fixedWidth, m_fixedHeight));
	else
		success = m_viewContext->Create(GetGraphicsDevice());
	if (!success)
	{
		SAFE_DELETE(m_viewContext);
		return FALSE;
	}
	
	return TRUE;
}

BOOL Framebuffer::AttachTexture(FRAMEBUFFER_DATA_TYPE type)
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	Texture *existing = GetTexture(type);
	ASSERT(existing == NULL);
	if (existing != NULL)
		return FALSE;
	
	// also need to make sure a renderbuffer isn't already attached to this type!
	Renderbuffer *existingRenderbuffer = GetRenderbuffer(type);
	ASSERT(existingRenderbuffer == NULL);
	if (existingRenderbuffer != NULL)
		return FALSE;
	
	// don't allow unsupported types!
	if (type == FRAMEBUFFER_DATA_NONE)
		return FALSE;
	if (type == FRAMEBUFFER_DATA_DEPTH && !GetGraphicsDevice()->IsDepthTextureSupported())
		return FALSE;
	if (type == FRAMEBUFFER_DATA_STENCIL)
		return FALSE;
	
	// determine texture format and framebuffer attachment type
	TEXTURE_FORMAT textureFormat;
	GLenum attachmentType;
	switch (type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
			textureFormat = TEXTURE_FORMAT_RGB;
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_COLOR_RGBA:
			textureFormat = TEXTURE_FORMAT_RGBA;
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_DEPTH:
			textureFormat = TEXTURE_FORMAT_DEPTH;
			attachmentType = GL_DEPTH_ATTACHMENT;
			break;
		default:
			textureFormat = TEXTURE_FORMAT_NONE;
			attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;
	
	uint16_t width = 0;
	uint16_t height = 0;
	GetDimensionsForAttachment(width, height);
	
	Texture *attach = new Texture();
	BOOL textureSuccess = attach->Create(GetGraphicsDevice(), width, height, textureFormat);
	ASSERT(textureSuccess == TRUE);
	if (!textureSuccess)
	{
		SAFE_DELETE(attach);
		return FALSE;
	}
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, attach->GetTextureName(), 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);

	m_textures[type] = attach;
	
	return TRUE;
}

BOOL Framebuffer::ReCreateAndAttach(FramebufferTextureMap::iterator &itor, BOOL releaseFirst)
{
	STACK_TRACE;
	Texture *existing = itor->second;
	
	// determine attachment type
	GLenum attachmentType;
	switch (existing->GetFormat())
	{
		case TEXTURE_FORMAT_RGB:
		case TEXTURE_FORMAT_RGBA:   attachmentType = GL_COLOR_ATTACHMENT0; break;
		case TEXTURE_FORMAT_DEPTH:  attachmentType = GL_DEPTH_ATTACHMENT; break;
		default:                    attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;
	
	uint16_t width = 0;
	uint16_t height = 0;
	GetDimensionsForAttachment(width, height);
	
	TEXTURE_FORMAT existingFormat = existing->GetFormat();
	
	if (releaseFirst)
		existing->Release();
	
	BOOL textureSuccess = existing->Create(GetGraphicsDevice(), width, height, existingFormat);
	ASSERT(textureSuccess == TRUE);
	if (!textureSuccess)
		return FALSE;
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, existing->GetTextureName(), 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	return TRUE;
}

BOOL Framebuffer::AttachRenderbuffer(FRAMEBUFFER_DATA_TYPE type)
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	Renderbuffer *existing = GetRenderbuffer(type);
	ASSERT(existing == NULL);
	if (existing != NULL)
		return FALSE;

	// also need to make sure a texture isn't already attached to this type!
	Texture *existingTexture = GetTexture(type);
	ASSERT(existingTexture == NULL);
	if (existingTexture != NULL)
		return FALSE;
	
	// don't allow unsupported types!
	if (type == FRAMEBUFFER_DATA_NONE)
		return FALSE;

	// determine framebuffer attachment type
	GLenum attachmentType;
	switch (type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
		case FRAMEBUFFER_DATA_COLOR_RGBA:  attachmentType = GL_COLOR_ATTACHMENT0; break;
		case FRAMEBUFFER_DATA_DEPTH:       attachmentType = GL_DEPTH_ATTACHMENT; break;
		case FRAMEBUFFER_DATA_STENCIL:     attachmentType = GL_STENCIL_ATTACHMENT; break;
		default:                           attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;

	uint16_t width = 0;
	uint16_t height = 0;
	GetDimensionsForAttachment(width, height);
	
	Renderbuffer *attach = new Renderbuffer();
	BOOL renderbufferSuccess = attach->Initialize(GetGraphicsDevice(), width, height, type);
	ASSERT(renderbufferSuccess == TRUE);
	if (!renderbufferSuccess)
	{
		SAFE_DELETE(attach);
		return FALSE;
	}
	
	GetGraphicsDevice()->BindFramebuffer(this);	
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, attach->GetRenderbufferName()));
	GetGraphicsDevice()->UnbindFramebuffer(this);

	m_renderbuffers[type] = attach;

	return TRUE;
}

BOOL Framebuffer::ReCreateAndAttach(FramebufferRenderbufferMap::iterator &itor, BOOL releaseFirst)
{
	STACK_TRACE;
	Renderbuffer *existing = itor->second;
	
	// determine framebuffer attachment type
	GLenum attachmentType;
	switch (existing->GetType())
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
		case FRAMEBUFFER_DATA_COLOR_RGBA:  attachmentType = GL_COLOR_ATTACHMENT0; break;
		case FRAMEBUFFER_DATA_DEPTH:       attachmentType = GL_DEPTH_ATTACHMENT; break;
		case FRAMEBUFFER_DATA_STENCIL:     attachmentType = GL_STENCIL_ATTACHMENT; break;
		default:                           attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;

	uint16_t width = 0;
	uint16_t height = 0;
	GetDimensionsForAttachment(width, height);
	
	FRAMEBUFFER_DATA_TYPE existingType = existing->GetType();
	
	if (releaseFirst)
		existing->Release();

	BOOL renderbufferSuccess = existing->Initialize(GetGraphicsDevice(), width, height, existingType);
	ASSERT(renderbufferSuccess == TRUE);
	if (!renderbufferSuccess)
		return FALSE;
	
	GetGraphicsDevice()->BindFramebuffer(this);	
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, existing->GetRenderbufferName()));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	return TRUE;
}

BOOL Framebuffer::ReleaseViewContext()
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	ASSERT(m_viewContext != NULL);
	if (m_viewContext == NULL)
		return FALSE;
	
	if (GetGraphicsDevice()->GetViewContext() == m_viewContext)
		GetGraphicsDevice()->SetViewContext(NULL);
	
	SAFE_DELETE(m_viewContext);
	
	return TRUE;
}

BOOL Framebuffer::ReleaseTexture(FRAMEBUFFER_DATA_TYPE type)
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	Texture *existing = GetTexture(type);
	ASSERT(existing != NULL);
	if (existing == NULL)
		return FALSE;
	
	// determine attachment type
	GLenum attachmentType;
	switch (type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_COLOR_RGBA:
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_DEPTH:
			attachmentType = GL_DEPTH_ATTACHMENT;
			break;
		default:
			attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;

	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	BOOL removeSuccess = RemoveTexture(existing);
	ASSERT(removeSuccess == TRUE);
	if (!removeSuccess)
		return FALSE;
	
	return TRUE;
}

BOOL Framebuffer::ReleaseRenderbuffer(FRAMEBUFFER_DATA_TYPE type)
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return FALSE;
	
	Renderbuffer *existing = GetRenderbuffer(type);
	ASSERT(existing != NULL);
	if (existing == NULL)
		return FALSE;

	// determine attachment type
	GLenum attachmentType;
	switch (type)
	{
		case FRAMEBUFFER_DATA_COLOR_RGB:
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_COLOR_RGBA:
			attachmentType = GL_COLOR_ATTACHMENT0;
			break;
		case FRAMEBUFFER_DATA_DEPTH:
			attachmentType = GL_DEPTH_ATTACHMENT;
			break;
		default:
			attachmentType = 0;
	}
	ASSERT(attachmentType != 0);
	if (attachmentType == 0)
		return FALSE;
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	BOOL removeSuccess = RemoveRenderbuffer(existing);
	ASSERT(removeSuccess == TRUE);
	if (!removeSuccess)
		return FALSE;

	return TRUE;
}

Texture* Framebuffer::GetTexture(FRAMEBUFFER_DATA_TYPE type) const
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return NULL;
	
	Texture *result = NULL;
	
	FramebufferTextureMap::const_iterator i = m_textures.find(type);

	// color could be RGB or RGBA -- can't have both! so one needs to match
	// the other if present!
	if (i == m_textures.end() && (type == FRAMEBUFFER_DATA_COLOR_RGB || FRAMEBUFFER_DATA_COLOR_RGBA))
	{
		FRAMEBUFFER_DATA_TYPE otherColorType = (type == FRAMEBUFFER_DATA_COLOR_RGB ? FRAMEBUFFER_DATA_COLOR_RGBA : FRAMEBUFFER_DATA_COLOR_RGB);
		i = m_textures.find(otherColorType);
	}
	
	// *now* we can test if found or not
	if (i != m_textures.end())
		result = i->second;
	
	return result;
}

Renderbuffer* Framebuffer::GetRenderbuffer(FRAMEBUFFER_DATA_TYPE type) const
{
	STACK_TRACE;
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return NULL;
	
	Renderbuffer *result = NULL;
	
	FramebufferRenderbufferMap::const_iterator i = m_renderbuffers.find(type);
	
	// color could be RGB or RGBA -- can't have both! so one needs to match
	// the other if present!
	if (i == m_renderbuffers.end() && (type == FRAMEBUFFER_DATA_COLOR_RGB || FRAMEBUFFER_DATA_COLOR_RGBA))
	{
		FRAMEBUFFER_DATA_TYPE otherColorType = (type == FRAMEBUFFER_DATA_COLOR_RGB ? FRAMEBUFFER_DATA_COLOR_RGBA : FRAMEBUFFER_DATA_COLOR_RGB);
		i = m_renderbuffers.find(otherColorType);
	}
	
	// *now* we can test if found or not
	if (i != m_renderbuffers.end())
		result = i->second;
	
	return result;
}

void Framebuffer::OnNewContext()
{
	STACK_TRACE;
	if (m_framebufferName == 0 && GetGraphicsDevice() != NULL)
	{
		// recreate the framebuffer
		CreateFramebuffer();
		
		if (m_viewContext != NULL)
			m_viewContext->OnNewContext();
		
		// now recreate & reattach all the attachment points that were set
		
		for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
		{
			BOOL success = ReCreateAndAttach(i, FALSE);
			ASSERT(success == TRUE);
			if (!success)
			{
				Release();
				return;
			}
		}
		
		for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		{
			BOOL success = ReCreateAndAttach(i, FALSE);
			ASSERT(success == TRUE);
			if (!success)
			{
				Release();
				return;
			}
		}
	}
}

void Framebuffer::OnLostContext()
{
	STACK_TRACE;
	m_framebufferName = 0;
	if (m_viewContext != NULL)
		m_viewContext->OnLostContext();
	for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
		i->second->OnLostContext();
	for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		i->second->OnLostContext();
}

void Framebuffer::OnResize()
{
	STACK_TRACE;
	if (m_framebufferName != 0 && GetGraphicsDevice() != NULL)
	{
		if (m_viewContext != NULL)
		{
			GameWindow *window = GetGraphicsDevice()->GetWindow();
			m_viewContext->OnResize(window->GetRect(), window->GetScreenOrientation());
		}
		
		// now recreate & reattach all the attachment points that were set
		
		for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
		{
			BOOL success = ReCreateAndAttach(i, TRUE);
			ASSERT(success == TRUE);
			if (!success)
			{
				Release();
				return;
			}
		}
		
		for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		{
			BOOL success = ReCreateAndAttach(i, TRUE);
			ASSERT(success == TRUE);
			if (!success)
			{
				Release();
				return;
			}
		}		
	}
}

void Framebuffer::OnBind()
{
	STACK_TRACE;
}

void Framebuffer::OnUnBind()
{
	STACK_TRACE;
}

BOOL Framebuffer::RemoveTexture(Texture *texture)
{
	STACK_TRACE;
	for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
	{
		if (i->second == texture)
		{
			SAFE_DELETE(texture);
			m_textures.erase(i);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL Framebuffer::RemoveRenderbuffer(Renderbuffer *renderbuffer)
{
	STACK_TRACE;
	for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
	{
		if (i->second == renderbuffer)
		{
			SAFE_DELETE(renderbuffer);
			m_renderbuffers.erase(i);
			return TRUE;
		}
	}
	
	return FALSE;
}

void Framebuffer::GetDimensionsForAttachment(uint16_t &width, uint16_t &height) const
{
	STACK_TRACE;
	if (IsUsingFixedDimensions())
	{
		width = m_fixedWidth;
		height = m_fixedHeight;
	}
	else
	{
		const ViewContext *viewContext = NULL;
		if (m_viewContext != NULL)
			viewContext = m_viewContext;
		else
			viewContext = GetGraphicsDevice()->GetViewContext();

		ASSERT(viewContext != NULL);
		width = viewContext->GetViewportWidth();
		height = viewContext->GetViewportHeight();
	}
}
