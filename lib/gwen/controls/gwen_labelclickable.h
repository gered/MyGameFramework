/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_LABELCLICKABLE_H
#define GWEN_CONTROLS_LABELCLICKABLE_H

#include "gwen_base.h"
#include "gwen_button.h"

namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT LabelClickable : public Button
		{
			public:

				GWEN_CONTROL( LabelClickable, Button );
				
				virtual void Render( Skin::Base* skin );

		};
	}
}
#endif
