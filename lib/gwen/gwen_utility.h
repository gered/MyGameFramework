/*
	GWEN
	Copyright (c) 2010 Facepunch Studios
	See license in Gwen.h
*/

#pragma once
#ifndef GWEN_UTILITY_H
#define GWEN_UTILITY_H

#include <stl/vector.h>
#include <crt/snprintf.h>
#include "gwen_structures.h"

namespace Gwen
{
	class TextObject;

	namespace Utility
	{
		template <typename T>
		const T& Max( const T& x, const T& y )
		{
		  if ( y < x ) return x;
		  return y;
		}

		template <typename T>
		const T& Min( const T& x, const T& y )
		{
		  if ( y > x ) return x;
		  return y;
		}

#ifdef _MSC_VER
	#pragma warning( push )
	#pragma warning( disable : 4996 )
#endif

		template<typename T> void Replace( T& str, const T& strFind, const T& strReplace )
		{
			size_t pos = 0;
			while( (pos = str.find(strFind, pos) ) != T::npos )
			{
				str.replace( pos, strFind.length(), strReplace );
				pos += strReplace.length();
			}
		}

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

		// NOTE: commented out and replaced with the below overloads
		//       to remove a dependency on std::ostringstream
		//template <class T>
		//String ToString( const T& object )
		//{
		//	std::ostringstream os;
		//	os << object;
		//	return os.str();
		//}

		//////
		inline String ToString( int object )
		{
			char temp[22];   // good enough for a 64 bit number (including negative)
			snprintf(temp, 22, "%d", object);
			String result = temp;
			return result;
		}

		inline String ToString( unsigned int object )
		{
			char temp[22];   // good enough for a 64 bit number
			snprintf(temp, 22, "%u", object);
			String result = temp;
			return result;
		}

		inline String ToString( float object )
		{
			char temp[22];   // to be honest, I don't know if this is good enough. let's hope so... at least snprintf is being used :)
			snprintf(temp, 22, "%f", object);
			String result = temp;
			return result;
		}
		//////

		inline Gwen::Rect ClampRectToRect( Gwen::Rect inside, Gwen::Rect outside, bool clampSize = false )
		{
			if ( inside.x < outside.x )
				inside.x = outside.x; 

			if ( inside.y  < outside.y )
				inside.y = outside.y;

			if ( inside.x + inside.w > outside.x + outside.w )
			{
				if ( clampSize )
					inside.w = outside.w;
				else
					inside.x = outside.x + outside.w - inside.w;
			}
			if ( inside.y + inside.h > outside.y + outside.h )
			{
				if ( clampSize )
					inside.h = outside.h;
				else
					inside.y = outside.w + outside.h - inside.h;
			}
			
			return inside;
		}

		GWEN_EXPORT String Format( const char* fmt, ... );

		namespace Strings
		{
			typedef stl::vector<Gwen::String> List;

			GWEN_EXPORT void Split( const Gwen::String& str, const Gwen::String& seperator, Strings::List& outbits, bool bLeaveSeperators = false );
			GWEN_EXPORT bool Wildcard( const Gwen::TextObject& strWildcard, const Gwen::TextObject& strHaystack );

			GWEN_EXPORT void ToUpper( Gwen::String& str );
			GWEN_EXPORT void Strip( Gwen::String& str, const Gwen::String& chars );

			template <typename T>
			T TrimLeft( const T& str, const T& strChars )
			{
				T outstr = str;
				outstr.erase( 0, outstr.find_first_not_of( strChars ) );
				return outstr;
			}

			namespace To
			{
				GWEN_EXPORT bool Bool( const Gwen::String& str );
				GWEN_EXPORT int Int( const Gwen::String& str );
				GWEN_EXPORT float Float( const Gwen::String& str );
				GWEN_EXPORT bool Floats( const Gwen::String& str, float* f, size_t iCount );
			}
		}
	}



}
#endif
