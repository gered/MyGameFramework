#include "../debug.h"

#include "simpletexturevertexlerpshader.h"

const char* SimpleTextureVertexLerpShader::m_vertexShaderSource = 
	"attribute vec4 a_position1;\n"
	"attribute vec4 a_position2;\n"
	"attribute vec2 a_texcoord0;\n"
	"uniform mat4 u_modelViewMatrix;\n"
	"uniform mat4 u_projectionMatrix;\n"
	"uniform float u_lerp;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_texCoords;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	v_texCoords = a_texcoord0;\n"
	"	gl_Position =  u_projectionMatrix * u_modelViewMatrix * mix(a_position1, a_position2, u_lerp);\n"
	"}\n";

const char* SimpleTextureVertexLerpShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	precision mediump float;\n"
	"#endif\n"
	"varying vec2 v_texCoords;\n"
	"uniform sampler2D u_texture;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(u_texture, v_texCoords);\n"
	"}\n";

SimpleTextureVertexLerpShader::SimpleTextureVertexLerpShader()
{
}

SimpleTextureVertexLerpShader::~SimpleTextureVertexLerpShader()
{
}

bool SimpleTextureVertexLerpShader::Initialize(GraphicsDevice *graphicsDevice)
{
	if (!VertexLerpShader::Initialize(graphicsDevice))
		return false;
	
	bool result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == true);

	MapAttributeToVboAttribIndex("a_position1", 0);
	MapAttributeToVboAttribIndex("a_position2", 1);
	MapAttributeToStandardAttribType("a_texcoord0", VERTEX_STD_TEXCOORD);
	
	return true;
}
