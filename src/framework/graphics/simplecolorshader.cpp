#include "../debug.h"

#include "simplecolorshader.h"

const char* SimpleColorShader::m_vertexShaderSource = 
	"attribute vec4 a_position;\n"
	"attribute vec4 a_color;\n"
	"uniform mat4 u_modelViewMatrix;\n"
	"uniform mat4 u_projectionMatrix;\n"
	"varying vec4 v_color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	v_color = a_color;\n"
	"	gl_Position =  u_projectionMatrix * u_modelViewMatrix * a_position;\n"
	"}\n";

const char* SimpleColorShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	#define LOWP lowp\n"
	"	precision mediump float;\n"
	"#else\n"
	"	#define LOWP\n"
	"#endif\n"
	"varying LOWP vec4 v_color;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = v_color;\n"
	"}\n";

SimpleColorShader::SimpleColorShader()
{
}

SimpleColorShader::~SimpleColorShader()
{
}

bool SimpleColorShader::Initialize(GraphicsDevice *graphicsDevice)
{
	if (!StandardShader::Initialize(graphicsDevice))
		return false;
	
	bool result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == true);

	MapAttributeToStandardAttribType("a_position", VERTEX_STD_POS_3D);
	MapAttributeToStandardAttribType("a_color", VERTEX_STD_COLOR);
	
	return true;
}
