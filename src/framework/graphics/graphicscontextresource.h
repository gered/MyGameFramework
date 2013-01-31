#ifndef __FRAMEWORK_GRAPHICS_GRAPHICSCONTEXTRESOURCE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GRAPHICSCONTEXTRESOURCE_H_INCLUDED__

/**
 * Interface for objects whose lifecycle is tied to the current graphics
 * context and should be made aware when the context is lost and/or
 * recreated.
 */
class GraphicsContextResource
{
public:
	/**
	 * New OpenGL graphics context creation callback.
	 */
	virtual void OnNewContext() = 0;

	/**
	 * Lost OpenGL graphics context callback.
	 */
	virtual void OnLostContext() = 0;
};

#endif
