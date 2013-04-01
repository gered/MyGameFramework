#ifndef __FRAMEWORK_GRAPHICS_CUSTOMVERTEXSKINNINGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMVERTEXSKINNINGSHADER_H_INCLUDED__

#include "vertexskinningshader.h"

class GraphicsDevice;
class Text;

class CustomVertexSkinningShader : public VertexSkinningShader
{
public:
	CustomVertexSkinningShader();
	virtual ~CustomVertexSkinningShader();
	
	BOOL Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource);
	BOOL Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource);
};

#endif
