#pragma once
#ifndef GWEN_CONTROLS_SPLITTERBAR_H
#define GWEN_CONTROLS_SPLITTERBAR_H

#include "../gwen.h"
#include "gwen_base.h"
#include "gwen_dragger.h"

namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT SplitterBar : public ControlsInternal::Dragger
		{
			public:

				GWEN_CONTROL( SplitterBar, ControlsInternal::Dragger );

				void Layout( Skin::Base* skin );
		};
	}
}
#endif
