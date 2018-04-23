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

		enum {
			type_null = 0,
			type_str = 1,
			type_color = 2,
			type_atom = 3,
			type_length = 4,
		};

		union {
			tchar_t*			str;
			atom 				atm;
			css_color_value		color;
			css_length_value	length;
		} 	m_value;

		uint8_t 	m_type;
		uint8_t		m_important;
		
		css_value()
		{
			m_type = type_null;
			m_important = false;
		}

		css_value( atom atm, bool imp = false ) 
		{
			m_important = imp;
			m_value.atm = atm;
			m_type = type_atom;
		}

		css_value( const tchar_t* val, bool imp = false ) 
		{
			m_important = imp;
			m_value.str = t_strdup( val );
			m_type = type_str;
		}

		css_value( const css_value_length& val, bool imp = false )
		{
			m_important = imp;
			m_value.length = val;
			m_type = type_str;
		}

		css_value(const css_value& val)
		{
			m_type = type_null;
			set( val );
		}

		~css_value( ) {
			clear( );
		}

		void clear( ) {
			if( m_type==type_str ) {
				mem_free( m_value.str );
			}

			m_type = type_null;
		}

		void set( const css_value& val ) {
			clear( );

			m_type	= val.m_type;
			m_important	= val.m_important;

			if( val.m_type==type_str ) {
				m_value.str	= t_strdup(val.m_value.str);
			}
			else {
				m_value		= val.m_value;
			}
		}

		css_value& operator=(const css_value& val)
		{
			set( val );
			return *this;
		}
	};

	struct	css_property
	{
		atom			name;
		css_value		value;
	};

	class style
	{
	private:
		xVector<css_property>		m_properties;
		
	public:
		style( );
		style( const style& val );
		virtual ~style();

		///void operator = (const style& val)
		///{
		///	m_properties = val.m_properties;
		///}

		void add( const tchar_t* txt, const tchar_t* baseurl )
		{
			parse( txt, baseurl);
		}

		void add_property(atom name, const css_value& value, const tchar_t* baseurl, bool important);

		const css_value*	get_property( atom name ) const
		{
			if( name ) {
				for( uint32_t i=0; i<m_properties.length(); i++ ) {
					css_property*	a = m_properties.at( i );
					if( a->name==name ) {
						return &a->value;
					}
				}
			}

			return NULL;
		}

		void combine(const style& src);

		void clear()
		{
			m_properties.clear();
		}

	private:
		void 	parse_property( const xstring& txt, const tchar_t* baseurl );
		void 	parse( const tchar_t* txt, const tchar_t* baseurl );
		void 	parse_short_border(const tstring& prefix, const tstring& val, bool important);
		void 	parse_short_background(const tstring& val, const tchar_t* baseurl, bool important);
		void 	parse_short_font(const tstring& val, bool important);

		void 	add_parsed_property(atom name, const css_value& val, bool important);
		//void 	remove_property( atom name, bool important );
	};
}
