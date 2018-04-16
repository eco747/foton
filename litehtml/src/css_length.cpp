#include "html.h"
#include "css_length.h"

namespace litehtml {

	void css_length::fromString( const tstring& str, int defValue )
	{
		// TODO: Make support for calc
		if( str.substr(0, 4) == _t("calc") )
		{
			m_is_predefined = true;
			m_predef		= 0;
			return;
		}

		//int	predef;
		//predef = atom_index(str.c_str(), -1, lst );

		/*if(predef >= 0)
		{
			m_is_predefined = true;
			m_predef		= predef;
		}
		else */
		{
			m_is_predefined = false;


			const tchar_t*	p = skip_sp( str.c_str() );

			tchar_t		temp[32];
			tchar_t*	t = temp;
			tchar_t*	e = &temp[countof(temp)-1];

			if( *p=='+' || *p=='-' ) {
				*t++ = *p++;
			}

			const tchar_t* start = p;
			while( t_isdigit(*p) && t<e ) {
				*t++ = *p++;
			}

			if( *p=='.' && t<e ) {
				*t++ = *p++;

				while( isdigit(*p) && t<e ) {
					*t++ = *p++;
				}
			}

			*t = 0;

			if( (p-start)==0 ) {
				//todo: malformed unit

				// not a number so it is predefined
				m_is_predefined = true;
				m_predef = defValue;
			}
			else {
				m_value = (float)t_strtod( temp, 0 );
				m_units	= (css_units) get_css_units( skip_sp(p), -1, css_units_none );
			}
		}
	}
}
