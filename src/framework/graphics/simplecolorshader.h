#ifndef __FRAMEWORK_GRAPHICS_SIMPLECOLORSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLECOLORSHADER_H_INCLUDED__

#include "standardshader.h"

/**
 * Shader which renders geometry with vertex colors but no textures.
 */
class SimpleColorShader : public StandardShader
{
public:
	SimpleColorShader();
	virtual ~SimpleColorShader();

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
