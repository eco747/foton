#pragma once

#include "attributes.h"
#include "xvector.h"
#include <string>

#define		mem_free	free
#define		mem_alloc	malloc

namespace litehtml
{
	class	css_value
	{
	public:
		static	css_value	undefined;

	public:

		union {
			tchar_t*			str;
			atom 				atm;
			int 				typ;
			css_color_value		clr;
			css_length_value	len;
		} 	m_value;

		uint8_t 	m_type;
		
		css_value( )
		{
			m_type 		= css_value_type_null;
		}

		css_value( atom val, bool imp = false ) 
		{
			m_type 		= css_value_type_atom;
			m_value.atm = atom_value;
		}

		css_value( css_value_type type, int val, bool imp = false ) 
		{
			m_type 		= type;
			m_value.typ = val;
		}

		css_value( const tchar_t* val, bool imp = false ) 
		{
			m_value.str = _strdup( val );
			m_type		= css_value_type_string;
		}

		css_value( const css_length_value& val, bool imp = false )
		{
			m_value.len = val;
			m_type = css_value_type_css_length;
		}

		css_value( const css_value& val )
		{
			m_type	= css_value_type_null;	// avoid clear
			set( val );
		}

		~css_value( ) {
			clear( );
		}

		void clear( ) {
			if( m_type==css_value_type_string ) {
				mem_free( m_value.str );
			}

			m_type = css_value_type_null;
		}

		void set( const css_value& val ) {
			clear( );

			m_type		= val.m_type;

			if( val.m_type==css_value_type_string ) {
				m_value.str	= _strdup(val.m_value.str);
			}
			else {
				m_value		= val.m_value;
			}
		}

		css_value& operator = (const css_value& val)
		{
			set( val );
			return *this;
		}

		bool 	is_atom( atom item ) const {
			return m_type==css_value_type_atom && m_value.atm==item;
		}
	};

	class css_property
	{
		atom 			m_name;
		uint8_t			m_important;
		css_property*	m_next;
			


	};

	class style
	{
	private:
		css_value*	m_first_value;
		css_value*	m_last_value;

	private:
		style( const style& val );
		
	public:
		style( );
		virtual ~style();

		//void operator = (const style& val)
		//{
		//	m_properties = val.m_properties;
		//}

		void 	add( const tchar_t* txt, const tchar_t* baseurl )
		{
			parse( txt, baseurl);
		}

		void	add_property( atom name, const xstringpart& value, const tchar_t* baseurl, bool important );

		const css_value*	get_property( atom name ) const
		{
			if( name ) {
				for( css_value* e=m_first_value; e; e=e->m_next ) {
					if( e->m_name==name ) {
						return e;
					}
				}
			}

			return NULL;
		}

		//void combine(const style& src);
		//void clear() { m_properties.clear(); }

	private:
		void 	parse_property( const xstring& txt, const tchar_t* baseurl );
		void 	parse( const tchar_t* txt, const tchar_t* baseurl );
		void 	parse_short_border(const tstring& prefix, const tstring& val, bool important);
		void 	parse_short_background(const tstring& val, const tchar_t* baseurl, bool important);
		void 	parse_short_font(const tstring& val, bool important);

		void 	add_prop( atom name, css_value& value, const tchar_t* baseurl, bool important );
		//void 	add_parsed_property(atom name, const css_value& val, bool important);
		//void 	remove_property( atom name, bool important );
	};
}
