#include "../debug.h"

#include "textureparameters.h"

#include "glincludes.h"
#include "glutils.h"

TextureParameters::TextureParameters()
{
	STACK_TRACE;
	Initialize();
}

TextureParameters::TextureParameters(MINIFICATION_FILTER minFilter, MAGNIFICATION_FILTER magFilter)
{
	STACK_TRACE;
	Initialize();

	m_minFilter = minFilter;
	m_magFilter = magFilter;
}

TextureParameters::~TextureParameters()
{
	STACK_TRACE;
}

void TextureParameters::Initialize()
{
	STACK_TRACE;
	m_minFilter = MIN_NEAREST;
	m_magFilter = MAG_LINEAR;
	m_wrapS = REPEAT;
	m_wrapT = REPEAT;
}

void TextureParameters::Apply() const
{
	STACK_TRACE;
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FindMinificationFilterValue(m_minFilter)));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FindMagnificationFilterValue(m_magFilter)));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, FindWrapModeValue(m_wrapS)));
	GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, FindWrapModeValue(m_wrapT)));
}

int TextureParameters::FindMinificationFilterValue(MINIFICATION_FILTER filter) const
{
	STACK_TRACE;
	switch (filter)
	{
	case MIN_NEAREST: return GL_NEAREST;
	case MIN_LINEAR: return GL_LINEAR;
	case MIN_NEAREST_MIPMAP_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
	case MIN_LINEAR_MIPMAP_NEAREST: return GL_LINEAR_MIPMAP_NEAREST;
	case MIN_NEAREST_MIPMAP_LINEAR: return GL_NEAREST_MIPMAP_LINEAR;
	default: return GL_LINEAR_MIPMAP_LINEAR;
	}
}

int TextureParameters::FindMagnificationFilterValue(MAGNIFICATION_FILTER filter) const
{
	STACK_TRACE;
	switch (filter)
	{
	case MAG_NEAREST: return GL_NEAREST;
	default: return GL_LINEAR;
	}
}

int TextureParameters::FindWrapModeValue(WRAP_MODE mode) const
{
	STACK_TRACE;
	switch (mode)
	{
	case CLAMP_TO_EDGE: return GL_CLAMP_TO_EDGE;
	default: return GL_REPEAT;
	}
}
