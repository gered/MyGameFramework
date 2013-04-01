#ifndef __FRAMEWORK_GRAPHICS_CUSTOMVERTEXLERPSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMVERTEXLERPSHADER_H_INCLUDED__

#include "vertexlerpshader.h"

class GraphicsDevice;
class Text;

class CustomVertexLerpShader : public VertexLerpShader
{
public:
	CustomVertexLerpShader();
	virtual ~CustomVertexLerpShader();

	BOOL Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource);
	BOOL Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource);
};

#endif
