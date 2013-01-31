#ifndef __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXLERPSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SIMPLETEXTUREVERTEXLERPSHADER_H_INCLUDED__

#include "vertexlerpshader.h"

class SimpleTextureVertexLerpShader : public VertexLerpShader
{
public:
	SimpleTextureVertexLerpShader();
	virtual ~SimpleTextureVertexLerpShader();

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
