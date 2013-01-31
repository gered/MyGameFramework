/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_HSVCOLORPICKER_H
#define GWEN_CONTROLS_HSVCOLORPICKER_H

#include "gwen_base.h"
#include "../gwen.h"
#include "../gwen_skin.h"
#include "gwen_colorcontrols.h"
#include "gwen_colorpicker.h"


namespace Gwen 
{
	namespace Controls
	{
		class GWEN_EXPORT HSVColorPicker : public Controls::Base
		{
		public:
			GWEN_CONTROL( HSVColorPicker, Controls::Base );

			Gwen::Color GetColor();
			Gwen::Color GetDefaultColor() { return m_Before->GetColor(); }
			void SetColor( Gwen::Color color, bool onlyHue = false, bool reset = false );

			void ColorBoxChanged( Gwen::Controls::Base* pControl );
			void ColorSliderChanged( Gwen::Controls::Base* pControl );
			void NumericTyped( Gwen::Controls::Base* control );

			void UpdateControls( Gwen::Color newColor );
			
			Event::Caller	onColorChanged;

		protected:
			ColorLerpBox* m_LerpBox;
			ColorSlider* m_ColorSlider;
			ControlsInternal::ColorDisplay* m_Before;
			ControlsInternal::ColorDisplay* m_After;
		};
	}
}
#endif
