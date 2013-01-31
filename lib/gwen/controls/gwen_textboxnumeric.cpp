/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/


#include "../gwen.h"
#include "gwen_textbox.h"
#include "../gwen_skin.h"
#include "../gwen_utility.h"
#include "../gwen_platform.h"


using namespace Gwen;
using namespace Gwen::Controls;

GWEN_CONTROL_CONSTRUCTOR( TextBoxNumeric )
{
	SetText( "0" );
}

bool TextBoxNumeric::IsTextAllowed( const Gwen::String& str, int iPos )
{
	const String& strString = GetText().Get();

	if ( str.length() == 0 )
		return true;

	for (size_t i=0; i<str.length(); i++)
	{
		if ( str[i] == '-' )
		{
			// Has to be at the start
			if ( i != 0 || iPos != 0 )
				return false;

			// Can only be one
			if ( stl::count( strString.begin(), strString.end(), '-' ) > 0 )
				return false;

			continue;
		}

		if ( str[i] == '0' ) continue;
		if ( str[i] == '1' ) continue;
		if ( str[i] == '2' ) continue;
		if ( str[i] == '3' ) continue;
		if ( str[i] == '4' ) continue;
		if ( str[i] == '5' ) continue;
		if ( str[i] == '6' ) continue;
		if ( str[i] == '7' ) continue;
		if ( str[i] == '8' ) continue;
		if ( str[i] == '9' ) continue;

		if ( str[i] == '.' )
		{
			// Already a fullstop
			if ( stl::count( strString.begin(), strString.end(), '.' ) > 0 )
				return false;

			continue;
		}

		return false;
	}

	return true;
}

float TextBoxNumeric::GetFloatFromText()
{
	double temp = Gwen::Utility::Strings::To::Float( GetText().Get() );
	return temp;
}