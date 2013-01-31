/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_PROPERTIES_H
#define GWEN_CONTROLS_PROPERTIES_H

#include "gwen_base.h"
#include "gwen_label.h"
#include "property/gwen_baseproperty.h"
#include "property/gwen_text.h"
#include "gwen_splitterbar.h"
#include "../gwen.h"
#include "../gwen_skin.h"


namespace Gwen 
{
	namespace Controls
	{

		class PropertyRow;

		class GWEN_EXPORT Properties : public Base
		{
			public:

				GWEN_CONTROL( Properties, Base );

				virtual void PostLayout( Gwen::Skin::Base* skin );

				PropertyRow* Add( const TextObject& text, const TextObject& value = "" );
				PropertyRow* Add( const TextObject& text, Property::Base* pProp, const TextObject& value = "" );
				PropertyRow* Find( const TextObject& text );

				virtual int GetSplitWidth();

				virtual void Clear();

			protected:

				virtual void OnSplitterMoved( Controls::Base * control );

				Controls::SplitterBar*	m_SplitterBar;

		};

		class GWEN_EXPORT PropertyRow : public Base
		{
			public:

				GWEN_CONTROL( PropertyRow, Base );

				virtual Label* GetLabel(){ return m_Label; }
				virtual void SetProperty( Property::Base* prop );
				virtual Property::Base* GetProperty(){ return m_Property; }

				virtual void Layout( Gwen::Skin::Base* skin );
				virtual void Render( Gwen::Skin::Base* skin );

				virtual bool IsEditing(){ return m_Property && m_Property->IsEditing(); }
				virtual bool IsHovered(){ return BaseClass::IsHovered() || (m_Property && m_Property->IsHovered()); }
				virtual void OnEditingChanged();
				virtual void OnHoverChanged();

				Event::Caller	onChange;

			protected:

				void OnPropertyValueChanged( Gwen::Controls::Base* control );

				Label*			m_Label;
				Property::Base*	m_Property;

				bool			m_bLastEditing;
				bool			m_bLastHover;

		};
	}
}
#endif
