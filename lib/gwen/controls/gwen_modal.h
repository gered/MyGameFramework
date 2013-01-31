#pragma once
#ifndef GWEN_CONTROLS_MODAL_H
#define GWEN_CONTROLS_MODAL_H

#include "gwen_base.h"
#include "../gwen.h"
#include "../gwen_skin.h"


namespace Gwen 
{
	namespace ControlsInternal
	{
		class Modal : public Controls::Base
		{
			GWEN_CONTROL_INLINE( Modal, Controls::Base )
			{
				SetKeyboardInputEnabled( true );
				SetMouseInputEnabled( true );
				SetShouldDrawBackground( true );

				SetBounds( 0, 0, GetParent()->Width(), GetParent()->Height() );
			}
			
			virtual void Layout( Skin::Base* /*skin*/ )
			{
				SetBounds( 0, 0, GetCanvas()->Width(), GetCanvas()->Height() );
			}

			virtual void Render( Skin::Base* skin )
			{
				skin->DrawModalControl( this );
			}
		};
	}
}
#endif
