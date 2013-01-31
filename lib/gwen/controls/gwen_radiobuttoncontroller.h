/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_RADIOBOTTONCONTROLLER_H
#define GWEN_CONTROLS_RADIOBOTTONCONTROLLER_H

#include "gwen_base.h"
#include "gwen_label.h"
#include "gwen_radiobutton.h"


namespace Gwen 
{
	namespace Controls
	{

		class GWEN_EXPORT RadioButtonController : public Base
		{
			public:

				GWEN_CONTROL( RadioButtonController, Base );

				virtual void Render( Skin::Base* /*skin*/ ){};
				virtual void OnRadioClicked( Base* pFromPanel );

				virtual void OnChange();

				virtual LabeledRadioButton* AddOption( const Gwen::String& strText, const Gwen::String& strOptionName = "" );

				virtual LabeledRadioButton*	GetSelected(){ return m_Selected; }

				virtual const Gwen::String& GetSelectedName(){ return m_Selected->GetName(); }
				virtual const TextObject& GetSelectedLabel(){ return m_Selected->GetLabel()->GetText(); }

				Event::Caller		onSelectionChange;

			private:

				LabeledRadioButton* m_Selected;
		};
	}
}
#endif
