/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_TEXTOBJECT_H
#define GWEN_TEXTOBJECT_H

#include "gwen_utility.h"

namespace Gwen
{
	/*

		TextObjects can be either a UnicodeString or a String

		Just makes things easier instead of having a function taking both.

	*/
	class TextObject
	{
		public:

			TextObject(){}

			TextObject( const Gwen::String& text )
			{
				m_String = text;
			}

			TextObject( const char* text )
			{
				m_String = text;
			}

			operator const Gwen::String&(){ return m_String; }
		
			void operator = ( const char* str )
			{
				m_String = str;
			}

			void operator = ( const Gwen::String& str )
			{
				m_String = str;
			}

			bool operator == ( const TextObject& to ) const
			{
				return m_String == to.m_String;
			}

			const Gwen::String& Get() const
			{
				return m_String;
			}

			const char* c_str() const 
			{
				return m_String.c_str();
			}


			int length() const { return m_String.length(); }

			Gwen::String			m_String;
	};
}
#endif
