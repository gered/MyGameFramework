/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_CONTROLS_PROPERTY_BASEPROPERTY_H
#define GWEN_CONTROLS_PROPERTY_BASEPROPERTY_H

#include "../gwen_base.h"
#include "../../gwen.h"
#include "../../gwen_skin.h"
#include "../../gwen_utility.h"


namespace Gwen 
{
	namespace Controls
	{
		namespace Property
		{
			class GWEN_EXPORT Base : public Gwen::Controls::Base
			{
				public:

					GWEN_CONTROL_INLINE( Base, Gwen::Controls::Base )
					{
						SetHeight( 17 );
					}

					virtual TextObject GetPropertyValue() = 0;

					virtual void SetPropertyValue( const TextObject& v, bool bFireChangeEvents = false ) = 0;

					virtual bool IsEditing() = 0;

					virtual void DoChanged()
					{
						Event::Information info;
						info.String = GetPropertyValue();

						onChange.Call( this, info );
					}

					void OnPropertyValueChanged( Gwen::Controls::Base* /*control*/ )
					{
						DoChanged();
					}

					Event::Caller	onChange;
			};
		}
	}
}
#endif
