#ifndef __FRAMEWORK_GRAPHICS_CUSTOMSPRITESHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMSPRITESHADER_H_INCLUDED__

#include "spriteshader.h"

class Text;

/**
 * Shader class for rendering 2D or 3D (via billboards) sprites using custom 
 * GLSL sources.
 */
class CustomSpriteShader : public SpriteShader
{
public:
	/**
	 * Creates a new shader object for rendering sprites using the given vertex
	 * and fragment shader sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	CustomSpriteShader(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	* Creates a new shader object for rendering sprites using the given vertex
	* and fragment shader sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	CustomSpriteShader(const Text *vertexShaderSource, const Text *fragmentShaderSource);

	virtual ~CustomSpriteShader();
};

#endif
