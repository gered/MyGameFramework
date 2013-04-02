#ifndef __FRAMEWORK_GRAPHICS_RENDERBUFFER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_RENDERBUFFER_H_INCLUDED__

#include "../common.h"
#include "framebufferdatatypes.h"
#include "graphicscontextresource.h"

class GraphicsDevice;

/**
 * Represents a renderbuffer that stores off-screen image data for a
 * framebuffer object.
 */
class Renderbuffer : public GraphicsContextResource
{
public:
	Renderbuffer();
	virtual ~Renderbuffer()                                                     { Release(); }
	
	/**
	 * Releases the resources associated with this renderbuffer.
	 */
	void Release();
	
	/**
	 * Initializes the renderbuffer with the specified properties.
	 * @param graphicsDevice this graphics device this renderbuffer is
	 *                       associated with
	 * @param width the width of the renderbuffer in pixels
	 * @param height the height of the renderbuffer in pixels
	 * @param format the type of data this renderbuffer contains
	 * @return TRUE if the renderbuffer was created successfully
	 */
	BOOL Initialize(GraphicsDevice *graphicsDevice, uint width, uint height, FRAMEBUFFER_DATA_TYPE type);
	
	/**
	 * @return the name or ID assigned to this renderbuffer by OpenGL
	 */
	uint GetRenderbufferName() const                                            { return m_renderbufferName; }
	
	/**
	 * @return the width of the renderbuffer in pixels
	 */
	uint GetWidth() const                                                       { return m_width; }
	
	/**
	 * @return the height of the renderbuffer in pixels
	 */
	uint GetHeight() const                                                      { return m_height; }
	
	/**
	 * @return the type of data this renderbuffer contains
	 */
	FRAMEBUFFER_DATA_TYPE GetType() const                                       { return m_type; }
	
	/**
	 * @return TRUE if the renderbuffer has been marked as invalid and needs to be recreated
	 */
	BOOL IsInvalidated() const                                                  { return m_renderbufferName == 0; }
		
	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();
	
	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();
	
private:
	BOOL CreateRenderbuffer();
	
	uint m_renderbufferName;
	uint m_width;
	uint m_height;
	FRAMEBUFFER_DATA_TYPE m_type;
};

#endif
