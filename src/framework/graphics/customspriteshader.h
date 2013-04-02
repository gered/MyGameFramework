#ifndef __FRAMEWORK_GRAPHICS_CUSTOMSPRITESHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMSPRITESHADER_H_INCLUDED__

#include "spriteshader.h"

class GraphicsDevice;
class Text;

/**
 * Shader class for rendering 2D or 3D (via billboards) sprites using custom 
 * GLSL sources.
 */
class CustomSpriteShader : public SpriteShader
{
public:
	CustomSpriteShader();
	virtual ~CustomSpriteShader();
	
	/**
	 * Initializes the shader object for rendering sprites using the given vertex
	 * and fragment shader sources.
	 * @param graphicsDevice the graphics device to associate this shader with
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 * @return true if successful, false if not
	 */
	bool Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Initializes the shader object for rendering sprites using the given vertex
	 * and fragment shader sources.
	 * @param graphicsDevice the graphics device to associate this shader with
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 * @return true if successful, false if not
	 */
	bool Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource);
};

#endif
