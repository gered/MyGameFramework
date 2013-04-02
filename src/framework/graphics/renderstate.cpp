#include "renderstate.h"

#include "glincludes.h"
#include "glutils.h"

RenderState::RenderState()
{
	Initialize();
}

RenderState::~RenderState()
{
}

void RenderState::Initialize()
{
	m_depthTesting = TRUE;
	m_depthFunction = DEPTH_LESS;
	m_faceCulling = TRUE;
	m_faceCullingMode = BACK;
	m_lineWidth = 1.0f;
}

void RenderState::Apply() const
{
	if (m_depthTesting)
	{
		GL_CALL(glEnable(GL_DEPTH_TEST));
		GL_CALL(glDepthFunc(FindDepthFunctionValue(m_depthFunction)));
	}
	else
	{
		GL_CALL(glDisable(GL_DEPTH_TEST));
	}

	SetFaceCulling();

	GL_CALL(glLineWidth(m_lineWidth));
}

void RenderState::SetFaceCulling() const
{
	if (m_faceCulling)
	{
		GL_CALL(glEnable(GL_CULL_FACE));
		switch (m_faceCullingMode)
		{
		case FRONT_AND_BACK:
			GL_CALL(glCullFace(GL_FRONT_AND_BACK));
			break;
		case FRONT:
			GL_CALL(glCullFace(GL_FRONT));
			break;
		default:
			GL_CALL(glCullFace(GL_BACK));
			break;
		}
	}
	else
		GL_CALL(glDisable(GL_CULL_FACE));
}

int RenderState::FindDepthFunctionValue(DEPTH_FUNCTION function) const
{
	switch (function)
	{
	case DEPTH_NEVER: return GL_NEVER;
	case DEPTH_LESS: return GL_LESS;
	case DEPTH_EQUAL: return GL_EQUAL;
	case DEPTH_LESS_OR_EQUAL: return GL_LEQUAL;
	case DEPTH_GREATER: return GL_GREATER;
	case DEPTH_NOT_EQUAL: return GL_NOTEQUAL;
	case DEPTH_GREATER_OR_EQUAL: return GL_GEQUAL;
	default: return GL_ALWAYS;
	}
}
