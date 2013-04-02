#ifndef __FRAMEWORK_GRAPHICS_DEBUGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_DEBUGSHADER_H_INCLUDED__

#include "standardshader.h"

class GraphicsDevice;

/**
 * Shader for rendering debug geometry. Will render using solid colors
 * with thicker lines and larger point sizes.
 */
class DebugShader : public StandardShader
{
public:
	DebugShader();
	virtual ~DebugShader();
	
	bool Initialize(GraphicsDevice *graphicsDevice);

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
