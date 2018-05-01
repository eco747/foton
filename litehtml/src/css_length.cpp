#include "html.h"
#include "css_length.h"

namespace litehtml {

	css_length	css_length::fromString( const tstring& str, int defValue )
	{
		css_length	result;

		// TODO: Make support for calc
		if( str.substr( 0, 4 ) == _t( "calc" ) ) {
			result.set_predef( 0 );
			return result;
		}

		//int	predef;
		//predef = atom_index( str.c_str(), -1, lst );
		/*
		if(predef >= 0)
		{
			m_is_predefined = true;
			m_predef		= predef;
		}
		else
		*/
		{
			const tchar_t*	p = skip_sp( str.c_str() );
			tchar_t		temp[32];
			tchar_t*	t = temp;
			tchar_t*	e = &temp[countof( temp ) - 1];

			// try to find a number followed by a unit
			if( *p == '+' || *p == '-' ) {
				*t++ = *p++;
			}

			const tchar_t* start = p;

			while( t_isdigit( *p ) && t < e ) {
				*t++ = *p++;
			}

			if( *p == '.' && t < e ) {
				*t++ = *p++;

				while( isdigit( *p ) && t < e ) {
					*t++ = *p++;
				}
			}

			*t = 0;

			if( ( p - start ) == 0 ) {
				//todo: malformed unit
				// not a number so it is predefined
				result.set_predef( defValue );
			}
			else {
				css_units units = ( css_units )get_css_units( skip_sp( p ), -1, css_units_none );
				result.set_value( ( float )t_strtod( temp, 0 ), units );
			}
		}
	}
}
