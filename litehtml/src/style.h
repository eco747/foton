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
			tchar_t*			str_val;
			int 				int_val;
			css_color_value		color_val;
			css_length_value	length_val;
		};

		uint8_t 	type;
		
		css_value( )
		{
			type 		= css_value_type_null;
		}

		css_value( atom val ) 
		{
			type 		= css_value_type_atom;
			int_val		= val;
		}

		css_value( css_value_type t, int val ) 
		{
			type 		= t;
			int_val		= val;
		}

		css_value( const tchar_t* val ) 
		{
			type		= css_value_type_string;
			str_val 	= _strdup( val );
		}

		css_value( const css_length_value& val )
		{
			type 		= css_value_type_css_length;
			lenght_val	= val;
		}

		css_value( const css_value& val )
		{
			type	= css_value_type_null;	// avoid clear
			set( val );
		}

		~css_value( ) {
			clear( );
		}

		void clear( ) {
			if( type==css_value_type_string ) {
				mem_free( str_val );
			}

			type = css_value_type_null;
		}

		void set( const css_value& val ) {
			clear( );

			type		= val.m_type;
			if( val.type==css_value_type_string ) {
				str_val	= _strdup(val.str_val);
			}
			else {
				int_val	= val.int_val;
			}
		}

		css_value& operator = (const css_value& val)
		{
			set( val );
			return *this;
		}

		bool 	is_atom( atom item ) const {
			return type==css_value_type_atom && atom_val==item;
		}

		bool 	is( css_value_type t ) const {
			return type==type;
		}
	};

	class css_property
		: public xListNode<css_property>
	{
		atom 			m_name;
		css_value 		m_value;
		uint8_t			m_important;
		css_property*	m_next;
	};

	class style
	{
	private:
		xList<css_property>	m_props;
		
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
				for( css_property* p=m_props.first; p; p=p->m_next ) {
					if( p->m_name==name ) {
						return p->value;
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
