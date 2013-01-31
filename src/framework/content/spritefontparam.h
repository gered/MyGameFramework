#ifndef __FRAMEWORK_CONTENT_SPRITEFONTPARAM_H_INCLUDED__
#define __FRAMEWORK_CONTENT_SPRITEFONTPARAM_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"

struct SpriteFontParam : public ContentParam
{
	uint8_t size;

	SpriteFontParam(uint8_t size)
	{
		this->size = size;
	}
};

#endif
