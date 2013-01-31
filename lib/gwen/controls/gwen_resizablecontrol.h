/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_RESIZABLECONTROL_H
#define GWEN_CONTROLS_RESIZABLECONTROL_H

#include "gwen_base.h"
#include "gwen_label.h"
#include "gwen_button.h"
#include "gwen_dragger.h"
#include "gwen_label.h"
#include "gwen_resizer.h"
#include "../gwen.h"
#include "../gwen_skin.h"

namespace Gwen 
{
	namespace Controls
	{

		class GWEN_EXPORT ResizableControl : public Base
		{
			public:

				GWEN_CONTROL( ResizableControl, Base );

				virtual void SetClampMovement( bool shouldClamp ) { m_bClampMovement = shouldClamp; }
				virtual bool GetClampMovement() { return m_bClampMovement; }

				virtual void SetMinimumSize( const Gwen::Point& minSize ) { m_MinimumSize = minSize; }
				virtual Gwen::Point GetMinimumSize() { return m_MinimumSize; }

				virtual void DisableResizing();

				virtual bool SetBounds( int x, int y, int w, int h );

				virtual void OnResized(){};

				Event::Caller	onResize;

				virtual ControlsInternal::Resizer* GetResizer( int iResizer ){ return m_Resizer[iResizer]; } 

			protected:

				void OnResizedInternal( Controls::Base* pControl );

				Gwen::Point m_MinimumSize;
				bool m_bClampMovement;
				bool m_bResizable;

				ControlsInternal::Resizer* m_Resizer[10];

				
		};
	}
}
#endif
