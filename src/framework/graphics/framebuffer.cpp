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
	m_viewContext = NULL;
	m_framebufferName = 0;
	m_fixedWidth = 0;
	m_fixedHeight = 0;
}

bool Framebuffer::Initialize(GraphicsDevice *graphicsDevice)
{
	ASSERT(m_framebufferName == 0);
	if (m_framebufferName != 0)
		return false;
	
	if (!GraphicsContextResource::Initialize(graphicsDevice))
		return false;
	
	CreateFramebuffer();
	
	m_viewContext = NULL;
	m_fixedWidth = 0;
	m_fixedHeight = 0;
	
	return true;
}

bool Framebuffer::Initialize(GraphicsDevice *graphicsDevice, uint fixedWidth, uint fixedHeight)
{
	ASSERT(fixedWidth != 0);
	ASSERT(fixedHeight != 0);
	if (fixedWidth == 0 || fixedHeight == 0)
		return false;
	
	bool createSuccess = Initialize(graphicsDevice);
	if (!createSuccess)
		return false;
	
	m_fixedWidth = fixedWidth;
	m_fixedHeight = fixedHeight;
	
	return true;
}

void Framebuffer::Release()
{
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
	
	GraphicsContextResource::Release();
}

void Framebuffer::CreateFramebuffer()
{
	ASSERT(m_framebufferName == 0);
	GL_CALL(glGenFramebuffers(1, &m_framebufferName));
}

bool Framebuffer::AttachViewContext()
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	ASSERT(m_viewContext == NULL);
	if (m_viewContext != NULL)
		return false;
	
	m_viewContext = new ViewContext();
	bool success;
	if (IsUsingFixedDimensions())
		success = m_viewContext->Create(GetGraphicsDevice(), Rect(0, 0, m_fixedWidth, m_fixedHeight));
	else
		success = m_viewContext->Create(GetGraphicsDevice());
	if (!success)
	{
		SAFE_DELETE(m_viewContext);
		return false;
	}
	
	return true;
}

bool Framebuffer::AttachTexture(FRAMEBUFFER_DATA_TYPE type)
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	Texture *existing = GetTexture(type);
	ASSERT(existing == NULL);
	if (existing != NULL)
		return false;
	
	// also need to make sure a renderbuffer isn't already attached to this type!
	Renderbuffer *existingRenderbuffer = GetRenderbuffer(type);
	ASSERT(existingRenderbuffer == NULL);
	if (existingRenderbuffer != NULL)
		return false;
	
	// don't allow unsupported types!
	if (type == FRAMEBUFFER_DATA_NONE)
		return false;
	if (type == FRAMEBUFFER_DATA_DEPTH && !GetGraphicsDevice()->IsDepthTextureSupported())
		return false;
	if (type == FRAMEBUFFER_DATA_STENCIL)
		return false;
	
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
		return false;
	
	uint width = 0;
	uint height = 0;
	GetDimensionsForAttachment(width, height);
	
	Texture *attach = new Texture();
	bool textureSuccess = attach->Create(GetGraphicsDevice(), width, height, textureFormat);
	ASSERT(textureSuccess == true);
	if (!textureSuccess)
	{
		SAFE_DELETE(attach);
		return false;
	}
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, attach->GetTextureName(), 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);

	m_textures[type] = attach;
	
	return true;
}

bool Framebuffer::ReCreateAndAttach(FramebufferTextureMap::iterator &itor, bool releaseFirst)
{
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
		return false;
	
	uint width = 0;
	uint height = 0;
	GetDimensionsForAttachment(width, height);
	
	TEXTURE_FORMAT existingFormat = existing->GetFormat();
	
	if (releaseFirst)
		existing->Release();
	
	bool textureSuccess = existing->Create(GetGraphicsDevice(), width, height, existingFormat);
	ASSERT(textureSuccess == true);
	if (!textureSuccess)
		return false;
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, existing->GetTextureName(), 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	return true;
}

bool Framebuffer::AttachRenderbuffer(FRAMEBUFFER_DATA_TYPE type)
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	Renderbuffer *existing = GetRenderbuffer(type);
	ASSERT(existing == NULL);
	if (existing != NULL)
		return false;

	// also need to make sure a texture isn't already attached to this type!
	Texture *existingTexture = GetTexture(type);
	ASSERT(existingTexture == NULL);
	if (existingTexture != NULL)
		return false;
	
	// don't allow unsupported types!
	if (type == FRAMEBUFFER_DATA_NONE)
		return false;

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
		return false;

	uint width = 0;
	uint height = 0;
	GetDimensionsForAttachment(width, height);
	
	Renderbuffer *attach = new Renderbuffer();
	bool renderbufferSuccess = attach->Initialize(GetGraphicsDevice(), width, height, type);
	ASSERT(renderbufferSuccess == true);
	if (!renderbufferSuccess)
	{
		SAFE_DELETE(attach);
		return false;
	}
	
	GetGraphicsDevice()->BindFramebuffer(this);	
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, attach->GetRenderbufferName()));
	GetGraphicsDevice()->UnbindFramebuffer(this);

	m_renderbuffers[type] = attach;

	return true;
}

bool Framebuffer::ReCreateAndAttach(FramebufferRenderbufferMap::iterator &itor, bool releaseFirst)
{
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
		return false;

	uint width = 0;
	uint height = 0;
	GetDimensionsForAttachment(width, height);
	
	FRAMEBUFFER_DATA_TYPE existingType = existing->GetType();
	
	if (releaseFirst)
		existing->Release();

	bool renderbufferSuccess = existing->Initialize(GetGraphicsDevice(), width, height, existingType);
	ASSERT(renderbufferSuccess == true);
	if (!renderbufferSuccess)
		return false;
	
	GetGraphicsDevice()->BindFramebuffer(this);	
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, existing->GetRenderbufferName()));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	return true;
}

bool Framebuffer::ReleaseViewContext()
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	ASSERT(m_viewContext != NULL);
	if (m_viewContext == NULL)
		return false;
	
	if (GetGraphicsDevice()->GetViewContext() == m_viewContext)
		GetGraphicsDevice()->SetViewContext(NULL);
	
	SAFE_DELETE(m_viewContext);
	
	return true;
}

bool Framebuffer::ReleaseTexture(FRAMEBUFFER_DATA_TYPE type)
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	Texture *existing = GetTexture(type);
	ASSERT(existing != NULL);
	if (existing == NULL)
		return false;
	
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
		return false;

	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, 0, 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	bool removeSuccess = RemoveTexture(existing);
	ASSERT(removeSuccess == true);
	if (!removeSuccess)
		return false;
	
	return true;
}

bool Framebuffer::ReleaseRenderbuffer(FRAMEBUFFER_DATA_TYPE type)
{
	ASSERT(m_framebufferName != 0);
	if (m_framebufferName == 0)
		return false;
	
	Renderbuffer *existing = GetRenderbuffer(type);
	ASSERT(existing != NULL);
	if (existing == NULL)
		return false;

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
		return false;
	
	GetGraphicsDevice()->BindFramebuffer(this);
	GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, 0));
	GetGraphicsDevice()->UnbindFramebuffer(this);
	
	bool removeSuccess = RemoveRenderbuffer(existing);
	ASSERT(removeSuccess == true);
	if (!removeSuccess)
		return false;

	return true;
}

Texture* Framebuffer::GetTexture(FRAMEBUFFER_DATA_TYPE type) const
{
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
	if (m_framebufferName == 0 && GetGraphicsDevice() != NULL)
	{
		// recreate the framebuffer
		CreateFramebuffer();
		
		if (m_viewContext != NULL)
			m_viewContext->OnNewContext();
		
		// now recreate & reattach all the attachment points that were set
		
		for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
		{
			bool success = ReCreateAndAttach(i, false);
			ASSERT(success == true);
			if (!success)
			{
				Release();
				return;
			}
		}
		
		for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		{
			bool success = ReCreateAndAttach(i, false);
			ASSERT(success == true);
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
			bool success = ReCreateAndAttach(i, true);
			ASSERT(success == true);
			if (!success)
			{
				Release();
				return;
			}
		}
		
		for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
		{
			bool success = ReCreateAndAttach(i, true);
			ASSERT(success == true);
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
}

void Framebuffer::OnUnBind()
{
}

bool Framebuffer::RemoveTexture(Texture *texture)
{
	for (FramebufferTextureMap::iterator i = m_textures.begin(); i != m_textures.end(); ++i)
	{
		if (i->second == texture)
		{
			SAFE_DELETE(texture);
			m_textures.erase(i);
			return true;
		}
	}
	
	return false;
}

bool Framebuffer::RemoveRenderbuffer(Renderbuffer *renderbuffer)
{
	for (FramebufferRenderbufferMap::iterator i = m_renderbuffers.begin(); i != m_renderbuffers.end(); ++i)
	{
		if (i->second == renderbuffer)
		{
			SAFE_DELETE(renderbuffer);
			m_renderbuffers.erase(i);
			return true;
		}
	}
	
	return false;
}

void Framebuffer::GetDimensionsForAttachment(uint &width, uint &height) const
{
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
