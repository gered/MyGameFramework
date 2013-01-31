/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/


#pragma once
#ifndef GWEN_TOOLTIP_H
#define GWEN_TOOLTIP_H

#include "controls/gwen_base.h"
#include "gwen.h"
#include "gwen_skin.h"

using namespace Gwen;
using namespace Gwen::Controls;

namespace ToolTip
{
	GWEN_EXPORT bool TooltipActive();

	GWEN_EXPORT void Enable	( Controls::Base* pControl );
	GWEN_EXPORT void Disable ( Controls::Base* pControl );

	GWEN_EXPORT void ControlDeleted	( Controls::Base* pControl );

	GWEN_EXPORT void RenderToolTip	( Skin::Base* skin );
}

#endif
