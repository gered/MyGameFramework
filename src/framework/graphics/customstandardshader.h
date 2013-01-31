#ifndef __FRAMEWORK_GRAPHICS_CUSTOMSTANDARDSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMSTANDARDSHADER_H_INCLUDED__

#include "standardshader.h"

class Text;

/**
 * Generic shader class providing standard uniforms for convenience.
 */
class CustomStandardShader : public StandardShader
{
public:
	/**
	 * Creates a new shader object using the given vertex and fragment shader
	 * sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	CustomStandardShader(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Creates a new shader object using the given vertex and fragment shader
	 * sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	CustomStandardShader(const Text *vertexShaderSource, const Text *fragmentShaderSource);

	virtual ~CustomStandardShader();
};

#endif
