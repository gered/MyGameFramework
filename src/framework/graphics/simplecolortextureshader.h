#ifndef __FRAMEWORK_GRAPHICS_SIMPLECOLORTEXTURESHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLECOLORTEXTURESHADER_H_INCLUDED__

#include "standardshader.h"

/**
 * Shader which renders geometry using vertex colors modulated with a texture.
 */
class SimpleColorTextureShader : public StandardShader
{
public:
	SimpleColorTextureShader();
	virtual ~SimpleColorTextureShader();

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
