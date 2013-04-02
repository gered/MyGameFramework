#include "../debug.h"

#include "simpletexturevertexskinningshader.h"

const char* SimpleTextureVertexSkinningShader::m_vertexShaderSource = 
	"#ifdef GL_ES\n"
	"	precision mediump float;\n"
	"#endif\n"
	"\n"
	"const int MAX_BONES = 50;\n"
	"\n"
	"attribute vec4 a_position;\n"
	"attribute vec2 a_texcoord0;\n"
	"attribute float a_jointIndex;\n"
	"\n"
	"uniform mat4 u_modelViewMatrix;\n"
	"uniform mat4 u_projectionMatrix;\n"
	"uniform vec3 u_jointPositions[MAX_BONES];\n"
	"uniform vec4 u_jointRotations[MAX_BONES];\n"
	"\n"
	"varying vec2 v_texCoord;\n"
	"\n"
	"vec3 qtransform(vec4 q, vec3 v)\n"
	"{ \n"
	"	vec3 temp = cross(q.xyz, v) + q.w * v;\n"
	"	return cross(temp, -q.xyz) + dot(q.xyz,v) * q.xyz + q.w * temp;\n"
	"}\n"
	"\n"
	"void main()\n"
	"{\n"
	"	int j = int(a_jointIndex);\n"
	"\n"
	"	vec4 skinnedPosition = vec4(qtransform(u_jointRotations[j], a_position.xyz) + u_jointPositions[j], 1.0);\n"
	"\n"
	"	v_texCoord = a_texcoord0;\n"
	"	gl_Position = u_projectionMatrix * u_modelViewMatrix * skinnedPosition;\n"
	"}\n";

const char* SimpleTextureVertexSkinningShader::m_fragmentShaderSource = 
	"#ifdef GL_ES\n"
	"	precision mediump float;\n"
	"#endif\n"
	"varying vec2 v_texCoord;\n"
	"uniform sampler2D u_texture;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = texture2D(u_texture, v_texCoord);\n"
	"}\n";

SimpleTextureVertexSkinningShader::SimpleTextureVertexSkinningShader()
{
}

SimpleTextureVertexSkinningShader::~SimpleTextureVertexSkinningShader()
{
}

bool SimpleTextureVertexSkinningShader::Initialize(GraphicsDevice *graphicsDevice)
{
	if (!VertexSkinningShader::Initialize(graphicsDevice))
		return false;
	
	bool result = LoadCompileAndLinkInlineSources(m_vertexShaderSource, m_fragmentShaderSource);
	ASSERT(result == true);
	
	MapAttributeToVboAttribIndex("a_jointIndex", 0);
	MapAttributeToStandardAttribType("a_position", VERTEX_STD_POS_3D);
	MapAttributeToStandardAttribType("a_texcoord0", VERTEX_STD_TEXCOORD);
	
	return true;
}
