#ifndef __FRAMEWORK_GRAPHICS_SIMPLETEXTURESHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLETEXTURESHADER_H_INCLUDED__

#include "standardshader.h"

/**
 * Shader which renders geometry with texturing but no vertex colors.
 */
class SimpleTextureShader : public StandardShader
{
public:
	SimpleTextureShader();
	virtual ~SimpleTextureShader();

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
