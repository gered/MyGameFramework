#ifndef __FRAMEWORK_GRAPHICS_FRAMEBUFFER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_FRAMEBUFFER_H_INCLUDED__

#include "../common.h"
#include "framebufferdatatypes.h"
#include "graphicscontextresource.h"
#include <stl/map.h>

class GraphicsDevice;
class Renderbuffer;
class Texture;
class ViewContext;

typedef stl::map<FRAMEBUFFER_DATA_TYPE, Renderbuffer*> FramebufferRenderbufferMap;
typedef stl::map<FRAMEBUFFER_DATA_TYPE, Texture*> FramebufferTextureMap;

class Framebuffer : public GraphicsContextResource
{
	friend class GraphicsDevice;
	
public:
	Framebuffer();
	virtual ~Framebuffer();
	
	BOOL Create(GraphicsDevice *graphicsDevice);
	BOOL Create(GraphicsDevice *graphicsDevice, uint16_t fixedWidth, uint16_t fixedHeight);
	void Release();
	
	uint32_t GetFramebufferName() const                    { return m_framebufferName; }
	BOOL IsInvalidated() const                             { return m_framebufferName == 0; }
	BOOL IsUsingFixedDimensions() const                    { return (m_fixedWidth != 0 && m_fixedHeight != 0); }
	
	BOOL AttachViewContext();
	BOOL AttachTexture(FRAMEBUFFER_DATA_TYPE type);
	BOOL AttachRenderbuffer(FRAMEBUFFER_DATA_TYPE type);
	BOOL ReleaseViewContext();
	BOOL ReleaseTexture(FRAMEBUFFER_DATA_TYPE type);
	BOOL ReleaseRenderbuffer(FRAMEBUFFER_DATA_TYPE type);
	
	ViewContext* GetViewContext() const                    { return m_viewContext; }
	Texture* GetTexture(FRAMEBUFFER_DATA_TYPE type) const;
	Renderbuffer* GetRenderbuffer(FRAMEBUFFER_DATA_TYPE type) const;
	
	void OnNewContext();
	void OnLostContext();
	void OnResize();
	
private:
	void OnBind();
	void OnUnBind();
	
	BOOL ReCreateAndAttach(FramebufferTextureMap::iterator &itor, BOOL releaseFirst);
	BOOL ReCreateAndAttach(FramebufferRenderbufferMap::iterator &itor, BOOL releaseFirst);
	BOOL RemoveTexture(Texture *texture);
	BOOL RemoveRenderbuffer(Renderbuffer *renderbuffer);
	
	void GetDimensionsForAttachment(uint16_t &width, uint16_t &height) const;
	
	uint32_t m_framebufferName;
	GraphicsDevice *m_graphicsDevice;
	uint16_t m_fixedWidth;
	uint16_t m_fixedHeight;
	ViewContext *m_viewContext;
	FramebufferRenderbufferMap m_renderbuffers;
	FramebufferTextureMap m_textures;
};

#endif