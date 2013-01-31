/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_MENUSTRIP_H
#define GWEN_CONTROLS_MENUSTRIP_H

#include "../gwen_baserender.h"
#include "gwen_base.h"
#include "gwen_menu.h"
#include "gwen_menuitem.h"

namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT MenuStrip : public Menu
		{
			GWEN_CONTROL( MenuStrip, Menu );

				virtual void Render( Skin::Base* skin );
				virtual void RenderUnder( Skin::Base* /*skin*/ ){}
				virtual void Layout( Skin::Base* skin );

			protected:

				virtual void OnAddItem( MenuItem* item );
				virtual bool ShouldHoverOpenMenu();
				virtual void Close() {}


		};
	}

}
#endif
