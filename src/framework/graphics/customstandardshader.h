#ifndef __FRAMEWORK_GRAPHICS_CUSTOMSTANDARDSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMSTANDARDSHADER_H_INCLUDED__

#include "standardshader.h"

class GraphicsDevice;
class Text;

/**
 * Generic shader class providing standard uniforms for convenience.
 */
class CustomStandardShader : public StandardShader
{
public:
	CustomStandardShader();
	virtual ~CustomStandardShader();
	
	/**
	 * Initializes the shader object using the given vertex and fragment shader
	 * sources.
	 * @param graphicsDevice the graphics device to associate this shader with
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 * @return true if successful, false if not
	 */
	bool Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Initializes the shader object using the given vertex and fragment shader
	 * sources.
	 * @param graphicsDevice the graphics device to associate this shader with
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 * @return true if successful, false if not
	 */
	bool Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource);
};

#endif
