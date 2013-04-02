#include "blendstate.h"

#include "glincludes.h"
#include "glutils.h"

BlendState::BlendState()
{
	Initialize();
}

BlendState::BlendState(BLEND_FACTOR sourceFactor, BLEND_FACTOR destinationFactor)
{
	Initialize();

	m_blending = true;
	m_sourceBlendFactor = sourceFactor;
	m_destBlendFactor = destinationFactor;
}

BlendState::~BlendState()
{
}

void BlendState::Initialize()
{
	m_blending = false;
	m_sourceBlendFactor = ONE;
	m_destBlendFactor = ZERO;
}

void BlendState::Apply() const
{
	if (m_blending)
	{
		GL_CALL(glEnable(GL_BLEND));
		int source = FindBlendFactorValue(m_sourceBlendFactor);
		int dest = FindBlendFactorValue(m_destBlendFactor);
		GL_CALL(glBlendFunc(source, dest));
	}
	else
		GL_CALL(glDisable(GL_BLEND));
}

int BlendState::FindBlendFactorValue(BLEND_FACTOR factor) const
{
	switch (factor)
	{
	case ZERO: return GL_ZERO;
	case ONE: return GL_ONE;
	case SRC_COLOR: return GL_SRC_COLOR;
	case INVERSE_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
	case DST_COLOR: return GL_DST_COLOR;
	case INVERSE_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
	case SRC_ALPHA: return GL_SRC_ALPHA;
	case INVERSE_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
	case DST_ALPHA: return GL_DST_ALPHA;
	case INVERSE_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
	default: return GL_SRC_ALPHA_SATURATE;
	}
}
