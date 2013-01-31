#ifndef __FRAMEWORK_GRAPHICS_CUSTOMVERTEXLERPSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMVERTEXLERPSHADER_H_INCLUDED__

#include "vertexlerpshader.h"

class Text;

class CustomVertexLerpShader : public VertexLerpShader
{
public:
	CustomVertexLerpShader(const char *vertexShaderSource, const char *fragmentShaderSource);

	CustomVertexLerpShader(const Text *vertexShaderSource, const Text *fragmentShaderSource);

	virtual ~CustomVertexLerpShader();
};

#endif
