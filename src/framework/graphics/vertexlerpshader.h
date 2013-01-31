#ifndef __FRAMEWORK_GRAPHICS_VERTEXLERPSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VERTEXLERPSHADER_H_INCLUDED__

#include "../common.h"
#include "standardshader.h"
#include <stl/string.h>

class VertexLerpShader : public StandardShader
{
public:
	virtual ~VertexLerpShader();

	void SetLerp(float t);

protected:
	VertexLerpShader();

	const stl::string& GetLerpUniform() const              { return m_lerpUniform; }

	void SetLerpUniform(const stl::string &name)           { m_lerpUniform = name; }

private:
	stl::string m_lerpUniform;
};

#endif
