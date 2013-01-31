/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_WINDOWCONTROL_H
#define GWEN_CONTROLS_WINDOWCONTROL_H

#include "../gwen.h"
#include "gwen_base.h"
#include "gwen_label.h"
#include "gwen_button.h"
#include "gwen_dragger.h"
#include "gwen_label.h"
#include "gwen_resizablecontrol.h"
#include "gwen_modal.h"
#include "gwen_windowbuttons.h"
#include "../gwen_skin.h"


namespace Gwen 
{
	namespace Controls
	{
		class CloseButton;

		class GWEN_EXPORT WindowControl : public ResizableControl
		{
			public:

				GWEN_CONTROL( WindowControl, ResizableControl );

				virtual ~WindowControl();
				virtual void Render( Skin::Base* skin );
				virtual void RenderUnder( Skin::Base* skin );

				virtual void SetTitle( Gwen::String title );
				virtual void SetClosable(bool closeable);

				virtual void Touch();
				bool IsOnTop();

				virtual void SetHidden(bool hidden);

				void CloseButtonPressed();
				void RenderFocus( Gwen::Skin::Base* skin );
				void SetDeleteOnClose( bool b ){ m_bDeleteOnClose = b; }

				void MakeModal( bool bDrawBackground = true );
				void DestroyModal();

				Gwen::Event::Caller	onWindowClosed;

			protected:

				Gwen::ControlsInternal::Dragger*	m_TitleBar;
				Gwen::Controls::Label*				m_Title;
				Gwen::Controls::WindowCloseButton*	m_CloseButton;

				bool m_bDeleteOnClose;

				ControlsInternal::Modal* m_Modal;
		};
	}
}
#endif
