#ifndef __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXSKINNINGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXSKINNINGSHADER_H_INCLUDED__

#include "vertexskinningshader.h"

class SimpleTextureVertexSkinningShader : public VertexSkinningShader
{
public:
	SimpleTextureVertexSkinningShader();
	virtual ~SimpleTextureVertexSkinningShader();
	
private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
