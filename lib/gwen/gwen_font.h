/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_FONT_H
#define GWEN_FONT_H

#include <stl/string.h>
#include <stl/list.h>

#include "gwen_baserender.h"

namespace Gwen
{

	struct Font
	{
		typedef stl::list<Font*>	List;

		Font()
		{
			data = NULL;
			facename = "Arial";
			size = 10;
			realsize = 0;
			bold = false;
		}

		String	facename;
		float size;
		bool  bold;

		// This should be set by the renderer
		// if it tries to use a font where it's
		// NULL.
		void*	data;

		// This is the real font size, after it's
		// been scaled by Render->Scale()
		float realsize;
		
	};

} //namespace Gwen
#endif
