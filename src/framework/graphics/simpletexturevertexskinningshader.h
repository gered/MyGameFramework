#ifndef __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXSKINNINGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXSKINNINGSHADER_H_INCLUDED__

#include "vertexskinningshader.h"

class GraphicsDevice;

class SimpleTextureVertexSkinningShader : public VertexSkinningShader
{
public:
	SimpleTextureVertexSkinningShader();
	virtual ~SimpleTextureVertexSkinningShader();
	
	bool Initialize(GraphicsDevice *graphicsDevice);
	
private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
