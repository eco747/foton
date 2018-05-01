#pragma once
#include "types.h"

namespace litehtml {
	struct 	css_length_value {
		union {
			float	value;
			int		predef;			// predefined size, depend of the thing being measured.
		};

		uint8_t		units;			//css_unit
		uint8_t		is_predefined;	//bool
	};

	bool	parse_css_length( const xstringpart& value, css_length_value* result );

	/**
	 *
	 */

	class css_length {
		css_length_value	m_self;

	public:
		css_length() {
			m_self.value			= 0;
			m_self.predef			= 0;
			m_self.units			= css_units_none;
			m_self.is_predefined	= false;
		}

		css_length( const css_length& val ) {
			m_self = val.m_self;
		}

		css_length&	operator = ( const css_length& val ) {
			m_self = val.m_self;
			return *this;
		}

		css_length&	operator = ( const css_length_value& val ) {
			m_self = val;
			return *this;
		}

		css_length&	operator=( float val );

		bool		is_predefined() const {
			return 	!!m_self.is_predefined;
		}

		void		set_predef( int val ) {
			m_self.predef = val;
			m_self.is_predefined = true;
		}

		int			predef() const {
			if( m_self.is_predefined ) {
				return m_self.predef;
			}

			return 0;
		}

		void		set_value( float val, css_units units ) {
			m_self.value	= val;
			m_self.units	= units;
			m_self.is_predefined = false;
		}

		float		value( ) const {
			if( !m_self.is_predefined ) {
				return m_self.value;
			}

			return 0;
		}

		css_units	units( ) const {
			return ( css_units )m_self.units;
		}

		int		calc_percent( int width ) const {
			if( is_predefined() ) {
				return 0;
			}

			if( m_self.units == css_units_perc ) {
				return ( int )( ( double ) width * ( double ) m_self.value / 100.0 );
			}

			return ( int )m_self.value;
		}

		//static css_length	fromString( const tstring& str, int predef_value );
	};

	//inline css_length&	css_length::operator = (float val)
	//{
	//	m_value = val;
	//	m_units = css_units_px;
	//	m_is_predefined = false;
	//	return *this;
	//}
}
