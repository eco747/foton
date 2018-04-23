#pragma once

#include "attributes.h"
#include "xvector.h"
#include <string>

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

		atom 		m_name;
		uint8_t 	m_type;
		uint8_t		m_important;

		css_value*	m_next;
		
		css_value( atom name )
		{
			m_name 		= name;
			m_type 		= css_value_type_null;
			m_important = false;
		}

		css_value( atom name, atom val, bool imp = false ) 
		{
			m_name 		= name;
			m_important = imp;
			m_type 		= css_value_type_atom;
			m_value.atm = atom_value;
		}

		css_value( atom name, css_value_type type, int val, bool imp = false ) 
		{
			m_name 		= name;
			m_important = imp;
			m_type 		= type;
			m_value.typ = val;
		}

		css_value( atom name, const tchar_t* val, bool imp = false ) 
		{
			m_name 		= name;
			m_important = imp;
			m_value.str = t_strdup( val );
			m_type		= css_value_type_string;
		}

		css_value( atom name, const css_length_value& val, bool imp = false )
		{
			m_name 		= name;
			m_important = imp;
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

			m_name 		= val.m_name;
			m_type		= val.m_type;
			m_important	= val.m_important;

			if( val.m_type==css_value_type_string ) {
				m_value.str	= t_strdup(val.m_value.str);
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

		void 	add_prop( css_value& value, const tchar_t* baseurl );
		//void 	add_parsed_property(atom name, const css_value& val, bool important);
		//void 	remove_property( atom name, bool important );
	};
}
