#include "../debug.h"

#include "sprite2dshader.h"

const char* Sprite2DShader::m_vertexShaderSource = 
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

const char* Sprite2DShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	#define LOWP lowp\n"
	"	precision mediump float;\n"
	"#else\n"
	"	#define LOWP\n"
	"#endif\n"
	"varying LOWP vec4 v_color;\n"
	"varying vec2 v_texCoords;\n"
	"uniform sampler2D u_texture;\n"
	"uniform bool u_textureHasAlphaOnly;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 finalColor;\n"
	"	if (u_textureHasAlphaOnly)\n"
	"		finalColor = vec4(v_color.xyz, (v_color.a * texture2D(u_texture, v_texCoords).a));\n"
	"	else\n"
	"		finalColor = v_color * texture2D(u_texture, v_texCoords);\n"
	"	gl_FragColor = finalColor;\n"
	"}\n";

Sprite2DShader::Sprite2DShader()
	: SpriteShader()
{
	STACK_TRACE;
	BOOL result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == TRUE);

	MapAttributeToStandardAttribType("a_position", VERTEX_STD_POS_2D);
	MapAttributeToStandardAttribType("a_color", VERTEX_STD_COLOR);
	MapAttributeToStandardAttribType("a_texcoord0", VERTEX_STD_TEXCOORD);
}

Sprite2DShader::~Sprite2DShader()
{
	STACK_TRACE;
}
