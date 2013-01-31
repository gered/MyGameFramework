/*
	Control added by Gered King (May 2012)
*/


#include "gwen_panel.h"

using namespace Gwen;
using namespace Gwen::Controls;


GWEN_CONTROL_CONSTRUCTOR( Panel )
{
	m_iPanelStyle = Gwen::Skin::Panel::Normal;
}

void Panel::Render( Skin::Base* skin )
{
	skin->DrawPanel(this, m_iPanelStyle);
}
