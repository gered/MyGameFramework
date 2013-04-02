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
	virtual ~Framebuffer()                                                      { Release(); }
	
	void Release();
	
	BOOL Initialize(GraphicsDevice *graphicsDevice);
	BOOL Initialize(GraphicsDevice *graphicsDevice, uint fixedWidth, uint fixedHeight);
	
	uint GetFramebufferName() const                                             { return m_framebufferName; }
	BOOL IsInvalidated() const                                                  { return m_framebufferName == 0; }
	BOOL IsUsingFixedDimensions() const                                         { return (m_fixedWidth != 0 && m_fixedHeight != 0); }
	
	BOOL AttachViewContext();
	BOOL AttachTexture(FRAMEBUFFER_DATA_TYPE type);
	BOOL AttachRenderbuffer(FRAMEBUFFER_DATA_TYPE type);
	BOOL ReleaseViewContext();
	BOOL ReleaseTexture(FRAMEBUFFER_DATA_TYPE type);
	BOOL ReleaseRenderbuffer(FRAMEBUFFER_DATA_TYPE type);
	
	ViewContext* GetViewContext() const                                         { return m_viewContext; }
	Texture* GetTexture(FRAMEBUFFER_DATA_TYPE type) const;
	Renderbuffer* GetRenderbuffer(FRAMEBUFFER_DATA_TYPE type) const;
	
	void OnNewContext();
	void OnLostContext();
	void OnResize();
	
private:
	void OnBind();
	void OnUnBind();
	
	void CreateFramebuffer();
	
	BOOL ReCreateAndAttach(FramebufferTextureMap::iterator &itor, BOOL releaseFirst);
	BOOL ReCreateAndAttach(FramebufferRenderbufferMap::iterator &itor, BOOL releaseFirst);
	BOOL RemoveTexture(Texture *texture);
	BOOL RemoveRenderbuffer(Renderbuffer *renderbuffer);
	
	void GetDimensionsForAttachment(uint &width, uint &height) const;
	
	uint m_framebufferName;
	uint m_fixedWidth;
	uint m_fixedHeight;
	ViewContext *m_viewContext;
	FramebufferRenderbufferMap m_renderbuffers;
	FramebufferTextureMap m_textures;
};

#endif
