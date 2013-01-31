#ifndef __FRAMEWORK_GRAPHICS_CUSTOMVERTEXSKINNINGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_CUSTOMVERTEXSKINNINGSHADER_H_INCLUDED__

#include "vertexskinningshader.h"

class Text;

class CustomVertexSkinningShader : public VertexSkinningShader
{
public:
	CustomVertexSkinningShader(const char *vertexShaderSource, const char *fragmentShaderSource);
	
	CustomVertexSkinningShader(const Text *vertexShaderSource, const Text *fragmentShaderSource);
	
	virtual ~CustomVertexSkinningShader();
};

#endif
