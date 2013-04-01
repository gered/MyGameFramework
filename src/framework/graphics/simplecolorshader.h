#ifndef __FRAMEWORK_GRAPHICS_SIMPLECOLORSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLECOLORSHADER_H_INCLUDED__

#include "standardshader.h"

class GraphicsDevice;

/**
 * Shader which renders geometry with vertex colors but no textures.
 */
class SimpleColorShader : public StandardShader
{
public:
	SimpleColorShader();
	virtual ~SimpleColorShader();
	
	BOOL Initialize(GraphicsDevice *graphicsDevice);

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
