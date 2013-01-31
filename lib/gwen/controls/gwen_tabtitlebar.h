/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_TABTITLEBAR_H
#define GWEN_CONTROLS_TABTITLEBAR_H

#include "gwen_base.h"
#include "gwen_label.h"
#include "gwen_tabbutton.h"
#include "../gwen_draganddrop.h"
#include "../gwen_skin.h"

namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT TabTitleBar : public Label
		{
			public:

				GWEN_CONTROL_INLINE( TabTitleBar, Label )
				{
					SetMouseInputEnabled( true );
					SetTextPadding( Gwen::Padding( 5, 2, 5, 2 ) );
					SetPadding( Gwen::Padding( 1, 2, 1, 2 ) );

					DragAndDrop_SetPackage( true, "TabWindowMove" );
				}

				void Render( Skin::Base* skin )
				{
					skin->DrawTabTitleBar( this );
				}

				void DragAndDrop_StartDragging( Gwen::DragAndDrop::Package* pPackage, int x, int y )
				{
					DragAndDrop::SourceControl = GetParent();
					DragAndDrop::SourceControl->DragAndDrop_StartDragging( pPackage, x, y );
				}

				void UpdateFromTab( TabButton* pButton )
				{
					SetText( pButton->GetText() );
					SizeToContents();
				}

			private:

		};
	}
}
#endif
