#include "../debug.h"

#include "simplecolortextureshader.h"

const char* SimpleColorTextureShader::m_vertexShaderSource = 
	"attribute vec4 a_position;\n"
	"attribute vec4 a_color;\n"
	"attribute vec2 a_texcoord0;\n"
	"uniform mat4 u_modelViewMatrix;\n"
	"uniform mat4 u_projectionMatrix;\n"
	"varying vec4 v_color;\n"
	"varying vec2 v_texCoords;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	v_color = a_color;\n"
	"	v_texCoords = a_texcoord0;\n"
	"	gl_Position =  u_projectionMatrix * u_modelViewMatrix * a_position;\n"
	"}\n";

const char* SimpleColorTextureShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	#define LOWP lowp\n"
	"	precision mediump float;\n"
	"#else\n"
	"	#define LOWP\n"
	"#endif\n"
	"varying LOWP vec4 v_color;\n"
	"varying vec2 v_texCoords;\n"
	"uniform sampler2D u_texture;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = v_color * texture2D(u_texture, v_texCoords);\n"
	"}\n";

SimpleColorTextureShader::SimpleColorTextureShader()
	: StandardShader()
{
	STACK_TRACE;
	BOOL result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == TRUE);

	MapAttributeToStandardAttribType("a_position", VERTEX_STD_POS_3D);
	MapAttributeToStandardAttribType("a_color", VERTEX_STD_COLOR);
	MapAttributeToStandardAttribType("a_texcoord0", VERTEX_STD_TEXCOORD);
}

SimpleColorTextureShader::~SimpleColorTextureShader()
{
	STACK_TRACE;
}
