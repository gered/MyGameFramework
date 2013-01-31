/*
	Control added by Gered King (May 2012)
*/

#pragma once
#ifndef GWEN_CONTROLS_PANEL_H
#define GWEN_CONTROLS_PANEL_H

#include "gwen_base.h"
#include "../gwen.h"
#include "../gwen_skin.h"


namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT Panel : public Controls::Base
		{
			public:

				GWEN_CONTROL( Panel, Controls::Base );

				virtual void Render( Skin::Base* skin );

				void SetStyle(unsigned char iPanelStyle) { m_iPanelStyle = iPanelStyle; }
				unsigned char GetStyle() const { return m_iPanelStyle; }

			private:
				unsigned char m_iPanelStyle;
		};
	}
}
#endif

