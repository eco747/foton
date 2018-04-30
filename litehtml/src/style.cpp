#include "html.h"
#include "style.h"
#include <functional>
#include <algorithm>
#ifndef WINCE
#include <locale>
#endif

namespace litehtml {

	/*
	attr_map style::m_valid_values =
	{
		{ atom_white_space, white_space_strings }
	};
	*/

	style::style()
	{
	}

	/*
	style::style( const style& val )
	{
		m_properties = val.m_properties;
	}
	*/

	style::~style()
	{
	}

	void style::parse( const tchar_t* txt, const tchar_t* baseurl )
	{
		while( *txt ) {
			const tchar_t* p = skip_sp( txt );
			const tchar_t* q = p;

			while( *q && *q!=';' ) {
				q++;
			}

			parse_property( xstringpart(p, (uint32_t)(q-p)), baseurl);

			if( *q==';' )
				q++;

			txt = q;
		}
	}

	void style::parse_property( const xstring& txt, const tchar_t* baseurl )
	{
		xstringpart		text( txt );

		xstringpart		left, right;
		if( !text.split( ':', &left, &right ) ) {
			//todo warning bad style definition
			return;
		}

		// trim
		left.trim( );
		right.trim( );
		
		atom 	name = (atom)__get_atom( left.str, left.len, -1 );
		if( name>0 && right.len>0 ) {

			xstringpart		p1, p2;
			if( right.rsplit( '!', &p1, &p2 ) ) {

				p2.trim( );
				if( p2.len==9 && strnicmp(p2.str,"important",9)==0 ) {
					add_property( name, p1, baseurl, true );
					return;
				}
			}
			
			add_property( name, right, baseurl, false );
		}
	}

	/*
	void style::combine( const style& src )
	{
		for(props_map::const_iterator i = src.m_properties.begin(); i != src.m_properties.end(); i++)
		{
			add_parsed_property( i->first, i->second.m_value.c_str(), i->second.m_important);
		}
	}
	*/


	/** --------------------------------------------------------------------------------------------------------
	 * check 
	 *	-initial,
	 *	-inherited,
	 *	-unset 
	 *	value for the style
	 */

	bool style::parse_global_values( const xstringpart& value, css_value* result, bool with_none )
	{
		int 	v = __get_atom( value.str, value.len, -1 );

		if( v==atom_inherit ||
			v==atom_initial || 
			v==atom_unset ) {
			result->set( (atom)v );
			return true;
		}

		if( with_none && v==atom_none ) {
			result->set( (atom)v );
			return true;
		}
		
		return false;
	}

	bool style::parse_border_style( const xstringpart& value, css_value* result )
	{
		int idx = __get_border_style( value.str, value.len, -1 );
		if(idx<=0 ) {
			return false;
		}
		
		*result	= css_value( css_value_type_border_width, idx );
		return true;
	}

	bool	style::parse_color( const xstringpart& value, css_value* result )
	{
		web_color 	clr;
		if( !web_color::from_string(value,&clr) ) {
			return false;
		}
			
		*result = css_value( clr );
		return true;
	}

	bool	style::parse_length( const xstringpart& value, css_value* result )
	{
		css_length_value length;
		if( !parse_css_length(value,&length) ) {
			return false;
		}

		*result = css_value( length );
		return true;
	}


	/** --------------------------------------------------------------------------------------------------------
	 * 1px
	 * 1px 2px
	 * initial
	 * unset
	 * inherit	
	 **/

	void style::parse_border_spacing( const xstringpart& value, const tchar_t* baseurl, bool important )
	{
		xstringpart parts[2];
		int n = value.split( 0x01, parts, 2 );

		if( n == 1 ) {
			css_length_value	length;
			if( parse_css_length(parts[0],&length) ) {
				add_prop( atom__xx_border_spacing_x, css_value(lenth), baseurl, important );
				add_prop( atom__xx_border_spacing_y, css_value(lenth), baseurl, important );
				return;
			}
			
			if( parse_global_values(parts[0],&val) ) {
				add_prop( atom__xx_border_spacing_x, val, baseurl, important );
				add_prop( atom__xx_border_spacing_y, val, baseurl, important );
				return;	
			}
		}
		else if( n==2 ) {
			css_length_value	x, y;
			if( parse_css_length(parts[0],&x) &&
				parse_css_length(parts[1],&y) ) {
				add_prop( atom__xx_border_spacing_x, css_value(x), baseurl, important );
				add_prop( atom__xx_border_spacing_y, css_value(y), baseurl, important );
				return;
			}
		}

		//todo: bad border spacing
	}


	/**
	 * style
	 * inherit
	 * initial
	 * unset
	 * style/color
	 * width/style
	 * width/style/color
	 **/

	
	/** --------------------------------------------------------------------------------------------------------
	 *
	 */

	void 	style::parse_border( const xstringpart& value, const tchar_t* baseurl, bool important, int borders )
	{
		xstringpart	parts[3];
		int n = value.split( 0x01, parts, 3 );
	
		if( n==1 ) {
			css_value 	val;
			if( parse_border_style(parts[0],&val) ) {
				if( borders&1)	add_prop( atom_border_left_style, 	val, baseurl, important );
				if( borders&2)	add_prop( atom_border_right_style, 	val, baseurl, important );
				if( borders&4)	add_prop( atom_border_top_style, 	val, baseurl, important );
				if( borders&8)	add_prop( atom_border_bottom_style, val, baseurl, important );
				
				return;
			}

			if( parse_global_values(parts[0],&val) ) {
				if( borders&1)	add_prop( atom_border_left_style, 	val, baseurl, important );
				if( borders&2)	add_prop( atom_border_right_style, 	val, baseurl, important );
				if( borders&4)	add_prop( atom_border_top_style, 	val, baseurl, important );
				if( borders&8)	add_prop( atom_border_bottom_style, val, baseurl, important );
				return;
			}

		}
		else if( n==2 ) {
			css_value	val1, val2;
			if( parse_border_style(parts[0],&val1) && 
				parse_color(parts[1],&val2) ) {

				if( borders&1)	add_prop( atom_border_left_style, 	val1, baseurl, important );
				if( borders&2)	add_prop( atom_border_right_style, 	val1, baseurl, important );
				if( borders&4)	add_prop( atom_border_top_style, 	val1, baseurl, important );
				if( borders&8)	add_prop( atom_border_bottom_style, val1, baseurl, important );
		
				if( borders&1)	add_prop( atom_border_left_color, 	val2, baseurl, important);
				if( borders&2)	add_prop( atom_border_right_color, 	val2, baseurl, important);
				if( borders&4)	add_prop( atom_border_top_color, 	val2, baseurl, important);
				if( borders&8)	add_prop( atom_border_bottom_color, val2, baseurl, important);
				
				return;
			}
						
			if( parse_length(parts[0],&val1) &&
				parse_border_style(parts[1],&val2)  ) {
				
				if( borders&1)	add_prop( atom_border_left_width, 	val1, baseurl, important);
				if( borders&2)	add_prop( atom_border_right_width, 	val1, baseurl, important);
				if( borders&4)	add_prop( atom_border_top_width, 	val1, baseurl, important);
				if( borders&8)	add_prop( atom_border_bottom_width, val1, baseurl, important);

				if( borders&1)	add_prop( atom_border_left_style, 	val2, baseurl, important );
				if( borders&2)	add_prop( atom_border_right_style, 	val2, baseurl, important );
				if( borders&4)	add_prop( atom_border_top_style, 	val2, baseurl, important );
				if( borders&8)	add_prop( atom_border_bottom_style, val2, baseurl, important );

				return;
			}
		}
		else if( n==3 ) {
			css_value	val1, val2, val3;
			if( parse_length(parts[0],&val1) &&
				parse_border_style(parts[1],&val2) &&
				parse_color(parts[2],&val3)  ) {
				
				if( borders&1)	add_prop( atom_border_left_width, 	val1, baseurl, important);
				if( borders&2)	add_prop( atom_border_right_width, 	val1, baseurl, important);
				if( borders&4)	add_prop( atom_border_top_width, 	val1, baseurl, important);
				if( borders&8)	add_prop( atom_border_bottom_width, val1, baseurl, important);

				if( borders&1)	add_prop( atom_border_left_style, 	val2, baseurl, important );
				if( borders&2)	add_prop( atom_border_right_style, 	val2, baseurl, important );
				if( borders&4)	add_prop( atom_border_top_style, 	val2, baseurl, important );
				if( borders&8)	add_prop( atom_border_bottom_style, val2, baseurl, important );

				if( borders&1)	add_prop( atom_border_left_color, 	val3, baseurl, important);
				if( borders&2)	add_prop( atom_border_right_color, 	val3, baseurl, important);
				if( borders&4)	add_prop( atom_border_top_color, 	val3, baseurl, important);
				if( borders&8)	add_prop( atom_border_bottom_color, val3, baseurl, important);
				
				return;
			}
		}
		
		//todo: malformed border
	}

	void  	style::parse_border_width( const xstringpart& value, const tchar_t* baseurl, bool important ) 
	{
		xstringpart	parts[4];
		int n = value.split( 0x01, parts, 4 )

		css_value 	val[4];
		if( n==4 ) {
			if( parse_length(part[0],&val[0]) && parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) && parse_length(part[3],&val[3]) )
			{
				add_prop( atom_border_top_width,	val[0], base_url, important );
				add_prop( atom_border_right_width,	val[1], base_url, important );
				add_prop( atom_border_bottom_width,	val[2], base_url, important );
				add_prop( atom_border_left_width,	val[3], base_url, important );			

				return;
			}
		}
		else if( n==3 ) {
			if( parse_length(part[0],&val[0]) && parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) )
			{
				add_prop( atom_border_top_width,	val[0], base_url, important );
				add_prop( atom_border_right_width,	val[1], base_url, important );
				add_prop( atom_border_left_width,	val[1], base_url, important );
				add_prop( atom_border_bottom_width,	val[2], base_url, important );

				return;
			}
		}
		else if( n==2 ) {
			if( parse_length(part[0],&val[0]) && parse_length(part[1],&val[1]) )
			{
				add_prop( atom_border_top_width,	val[0], base_url, important );
				add_prop( atom_border_bottom_width,	val[0], base_url, important );
				add_prop( atom_border_right_width,	val[1], base_url, important );
				add_prop( atom_border_left_width,	val[1], base_url, important );

				return;
			}
		}
		else if( n==1 ) {
			if( parse_length(part[0],&val[0]) ) {
				add_prop( atom_border_top_width,	val[0], base_url, important );
				add_prop( atom_border_bottom_width,	val[0], base_url, important );
				add_prop( atom_border_right_width,	val[0], base_url, important );
				add_prop( atom_border_left_width,	val[0], base_url, important );

				return;
			}
		}

		//todo: malformed border-width
	}

	void  	style::parse_border_style( const xstringpart& value, const tchar_t* baseurl, bool important ) 
	{
		xstringpart	parts[4];
		int n = value.split( 0x01, parts, 4 )

		css_value 	val[4];
		if( n==4 ) {
			if( parse_border_style(part[0],&val[0]) &&
				parse_border_style(part[1],&val[1]) &&
				parse_border_style(part[2],&val[2]) && 
				parse_border_style(part[3],&val[3]) )
			{
				add_prop( atom_border_top_style,	val[0], base_url, important );
				add_prop( atom_border_right_style,	val[1], base_url, important );
				add_prop( atom_border_bottom_style,	val[2], base_url, important );
				add_prop( atom_border_left_style,	val[3], base_url, important );			

				return;
			}
		}
		else if( n==3 ) {
			if( parse_border_style(part[0],&val[0]) && 
				parse_border_style(part[1],&val[1]) &&
				parse_border_style(part[2],&val[2]) )
			{
				add_prop( atom_border_top_style,	val[0], base_url, important );
				add_prop( atom_border_right_style,	val[1], base_url, important );
				add_prop( atom_border_left_style,	val[1], base_url, important );
				add_prop( atom_border_bottom_style,	val[2], base_url, important );

				return;
			}
		}
		else if( n==2 ) {
			if( parse_border_style(part[0],&val[0]) && 
				parse_border_style(part[1],&val[1]) )
			{
				add_prop( atom_border_top_style,	val[0], base_url, important );
				add_prop( atom_border_bottom_style,	val[0], base_url, important );
				add_prop( atom_border_right_style,	val[1], base_url, important );
				add_prop( atom_border_left_style,	val[1], base_url, important );

				return;
			}
		}
		else if( n==1 ) {
			if( parse_border_style(part[0],&val[0]) ) {
				add_prop( atom_border_top_style,	val[0], base_url, important );
				add_prop( atom_border_bottom_style,	val[0], base_url, important );
				add_prop( atom_border_right_style,	val[0], base_url, important );
				add_prop( atom_border_left_style,	val[0], base_url, important );

				return;
			}
		}

		//todo: malformed border-color
	}

	void  	style::parse_border_color( const xstringpart& value, const tchar_t* baseurl, bool important ) 
	{
		xstringpart	parts[4];
		int n = value.split( 0x01, parts, 4 )

		css_value 	val[4];
		if( n==4 ) {
			if( parse_color(part[0],&val[0]) &&
				parse_color(part[1],&val[1]) &&
				parse_color(part[2],&val[2]) && 
				parse_color(part[3],&val[3]) )
			{
				add_prop( atom_border_top_color,	val[0], base_url, important );
				add_prop( atom_border_right_color,	val[1], base_url, important );
				add_prop( atom_border_bottom_color,	val[2], base_url, important );
				add_prop( atom_border_left_color,	val[3], base_url, important );			

				return;
			}
		}
		else if( n==3 ) {
			if( parse_color(part[0],&val[0]) && 
				parse_color(part[1],&val[1]) &&
				parse_color(part[2],&val[2]) )
			{
				add_prop( atom_border_top_color,	val[0], base_url, important );
				add_prop( atom_border_right_color,	val[1], base_url, important );
				add_prop( atom_border_left_color,	val[1], base_url, important );
				add_prop( atom_border_bottom_color,	val[2], base_url, important );

				return;
			}
		}
		else if( n==2 ) {
			if( parse_color(part[0],&val[0]) && 
				parse_color(part[1],&val[1]) )
			{
				add_prop( atom_border_top_color,	val[0], base_url, important );
				add_prop( atom_border_bottom_color,	val[0], base_url, important );
				add_prop( atom_border_right_color,	val[1], base_url, important );
				add_prop( atom_border_left_color,	val[1], base_url, important );

				return;
			}
		}
		else if( n==1 ) {
			if( parse_color(part[0],&val[0]) ) {
				add_prop( atom_border_top_color,	val[0], base_url, important );
				add_prop( atom_border_bottom_color,	val[0], base_url, important );
				add_prop( atom_border_right_color,	val[0], base_url, important );
				add_prop( atom_border_left_color,	val[0], base_url, important );

				return;
			}
		}

		//todo: malformed border-width
	}

	void  	style::parse_margin( const xstringpart& value, const tchar_t* baseurl, bool important ) 
	{
		xstringpart	parts[4];
		int n = value.split( 0x01, parts, 4 )

		css_value 	val[4];
		if( n==4 ) {
			if( parse_length(part[0],&val[0]) &&
				parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) && 
				parse_length(part[3],&val[3]) )
			{
				add_prop( atom_margin_top,		val[0], base_url, important );
				add_prop( atom_margin_right,	val[1], base_url, important );
				add_prop( atom_margin_bottom,	val[2], base_url, important );
				add_prop( atom_margin_left,		val[3], base_url, important );			

				return;
			}
		}
		else if( n==3 ) {
			if( parse_length(part[0],&val[0]) && 
				parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) )
			{
				add_prop( atom_margin_top,		val[0], base_url, important );
				add_prop( atom_margin_right,	val[1], base_url, important );
				add_prop( atom_margin_left,		val[1], base_url, important );
				add_prop( atom_margin_bottom,	val[2], base_url, important );

				return;
			}
		}
		else if( n==2 ) {
			if( parse_length(part[0],&val[0]) && 
				parse_length(part[1],&val[1]) )
			{
				add_prop( atom_margin_top,		val[0], base_url, important );
				add_prop( atom_margin_bottom,	val[0], base_url, important );
				add_prop( atom_margin_right,	val[1], base_url, important );
				add_prop( atom_margin_left,		val[1], base_url, important );

				return;
			}
		}
		else if( n==1 ) {
			if( parse_length(part[0],&val[0]) ) {
				add_prop( atom_margin_top,		val[0], base_url, important );
				add_prop( atom_margin_bottom,	val[0], base_url, important );
				add_prop( atom_margin_right,	val[0], base_url, important );
				add_prop( atom_margin_left,		val[0], base_url, important );

				return;
			}
		}

		//todo: malformed border-width
	}

	void  	style::parse_padding( const xstringpart& value, const tchar_t* baseurl, bool important ) 
	{
		xstringpart	parts[4];
		int n = value.split( 0x01, parts, 4 )

		css_value 	val[4];
		if( n==4 ) {
			if( parse_length(part[0],&val[0]) &&
				parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) && 
				parse_length(part[3],&val[3]) )
			{
				add_prop( atom_padding_top,		val[0], base_url, important );
				add_prop( atom_padding_right,	val[1], base_url, important );
				add_prop( atom_padding_bottom,	val[2], base_url, important );
				add_prop( atom_padding_left,	val[3], base_url, important );			

				return;
			}
		}
		else if( n==3 ) {
			if( parse_length(part[0],&val[0]) && 
				parse_length(part[1],&val[1]) &&
				parse_length(part[2],&val[2]) )
			{
				add_prop( atom_padding_top,		val[0], base_url, important );
				add_prop( atom_padding_right,	val[1], base_url, important );
				add_prop( atom_padding_left,	val[1], base_url, important );
				add_prop( atom_padding_bottom,	val[2], base_url, important );

				return;
			}
		}
		else if( n==2 ) {
			if( parse_length(part[0],&val[0]) && 
				parse_length(part[1],&val[1]) )
			{
				add_prop( atom_padding_top,		val[0], base_url, important );
				add_prop( atom_padding_bottom,	val[0], base_url, important );
				add_prop( atom_padding_right,	val[1], base_url, important );
				add_prop( atom_padding_left,	val[1], base_url, important );

				return;
			}
		}
		else if( n==1 ) {
			if( parse_length(part[0],&val[0]) ) {
				add_prop( atom_padding_top,		val[0], base_url, important );
				add_prop( atom_padding_bottom,	val[0], base_url, important );
				add_prop( atom_padding_right,	val[0], base_url, important );
				add_prop( atom_padding_left,	val[0], base_url, important );

				return;
			}
		}

		//todo: malformed border-width
	}
	

	void 	style::parse_border_radius( const xstringpart& value, const tchar_t* baseurl, bool important, int border1, int border2 ) {
		xstringpart	parts[2];
		int n = value.split( 0x01, parts, 2 )

		if( n==1 ) {
			css_value 	val;
			if( parse_length( parts[0],&val) ) {
				add_prop( border1, val, baseurl, important);
				add_prop( border2, val, baseurl, important);
				return;
			}
		}
		else if( n==2 ) {
			css_value 	val1, val2;
			if( parse_length( parts[0],&val1) &&
				parse_length( parts[1],&val2) ) {
				add_prop( border1, val1, baseurl, important);
				add_prop( border2, val2, baseurl, important);
				return;
			}
		}

		return;
	}

	void style::parse_background_image( const xstringbase& value, const tchar_t* baseurl, bool important ) 
	{
		xstringbase		v( value );
		v.trim_left( );

		if( v.i_equ('url',3) ) {
			
			v.move_start( 3 );
			
			xstringpart  url;
			if( v.extract( '(', ')', &url ) ) {

				url.trim( );

				tchar_t	ch;

				if( ur.len>0 && ((ch=*url.str)=='\'' || ch=='"') ) {
					if( url.extract( ch, ch, &url ) ) {
						add_prop( atom_background_image, url, baseurl, important);
						return;
					}
				}
			}
		}
		else {
			css_value	result;
			if( parse_global_values( value, &result, true ) ) {
				//	none
				//	inherit;
				//	initial;
				//	unset;
				add_prop( atom_background_image, result, baseurl, important);
			}
		}

		//	todo: malformed background-image
	}


	void style::add_property(atom name, const xstringpart& value, const tchar_t* baseurl, bool important )
	{
		if(!name || !part.len ) {
			return;
		}

		switch( name )
		{
			// Add baseurl for background image
			case 	atom_background_image:
			{
				parse_background_image( value, baseurl, important );
				//if(baseurl) {
				//	add_parsed_property(atom_background_baseurl, baseurl, important);
				//}
				return;
			}
		
			// Parse border spacing properties
			case 	atom_border_spacing:
			{
				parse_border_spacing( value, baseurl, important );
				return;
			}
		
			// Parse borders shorthand properties
			case atom_border:
			{
				parse_border( value, baseurl, important, 0x0f );
				return;
			}
		
			case atom_border_left: 
			{
				parse_border( value, baseurl, important, 0x01 );
				return;
			}
				 
			case atom_border_right:
			{
				parse_border( value, baseurl, important, 0x02 );
				return;
			}
		
			case atom_border_top:
			{
				parse_border( value, baseurl, important, 0x04 );
				return;
			}
				 
			case atom_border_bottom: {
				parse_border( value, baseurl, important, 0x08 );
				return;
			}
		

			// Parse border radius shorthand properties
		 	case atom_border_bottom_left_radius: 
			{
				parse_border_radius( value, baseurl, important, atom_border_bottom_left_radius_x, atom_border_bottom_left_radius_y );
				return;
			}

			case atom_border_bottom_right_radius:
			{
				parse_border_radius( value, baseurl, important, atom_border_bottom_right_radius_x, atom_border_bottom_right_radius_y );
				return;
			}

			case atom_border_top_right_radius:
			{
				parse_border_radius( value, baseurl, important, atom_border_top_right_radius_x, atom_border_top_right_radius_y );
				return;
			}

			case atom_border_top_left_radius:
			{
				parse_border_radius( value, baseurl, important, atom_border_top_left_radius_x, atom_border_top_left_radius_y );
				return;
			}

			// Parse border-radius shorthand properties
			case atom_border_radius:
			{
				//todo
				/*
				n = value.split( '/', parts, 2 );
				if(n == 1)
				{

					add_prop(atom_border_radius_x, tokens[0].c_str(), baseurl, important);
					add_prop(atom_border_radius_y, tokens[0].c_str(), baseurl, important);
				}
				else if(tokens.size() >= 2)
				{
					add_prop(atom_border_radius_x, tokens[0].c_str(), baseurl, important);
					add_prop(atom_border_radius_y, tokens[1].c_str(), baseurl, important);
				}
				*/

				return;
			}
		
			case atom_border_radius_x:
			{
				/*
				string_vector tokens;
				split_string(value, tokens, _t(" "));
				if(tokens.size() == 1)
				{
					add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_x,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_x,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_x,		tokens[0].c_str(), baseurl, important);
				}
				else if(tokens.size() == 2)
				{
					add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_x,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_x,		tokens[1].c_str(), baseurl, important);
				}
				else if(tokens.size() == 3)
				{
					add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_x,		tokens[2].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_x,		tokens[1].c_str(), baseurl, important);
				}
				else if(tokens.size() == 4)
				{
					add_property(atom_border_top_left_radius_x,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_x,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_x,		tokens[2].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_x,		tokens[3].c_str(), baseurl, important);
				}
				*/

				return;
			}
		
			case atom_border_radius_y:
			{
				/*
				string_vector tokens;
				split_string(value, tokens, _t(" "));
				if(tokens.size() == 1)
				{
					add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_y,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_y,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_y,		tokens[0].c_str(), baseurl, important);
				}
				else if(tokens.size() == 2)
				{
					add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_y,		tokens[0].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_y,		tokens[1].c_str(), baseurl, important);
				}
				else if(tokens.size() == 3)
				{
					add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_y,		tokens[2].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_y,		tokens[1].c_str(), baseurl, important);
				}
				else if(tokens.size() == 4)
				{
					add_property(atom_border_top_left_radius_y,			tokens[0].c_str(), baseurl, important);
					add_property(atom_border_top_right_radius_y,		tokens[1].c_str(), baseurl, important);
					add_property(atom_border_bottom_right_radius_y,		tokens[2].c_str(), baseurl, important);
					add_property(atom_border_bottom_left_radius_y,		tokens[3].c_str(), baseurl, important);
				}
				*/

				return;
			}
		
			// Parse list-style shorthand properties
			case atom_list_style:
			{
				/*
				add_parsed_property(atom_list_style_type,			_t("disc"),		important);
				add_parsed_property(atom_list_style_position,		_t("outside"),	important);
		//<eco:		add_parsed_property(atom_list_style_image,			_t(""),			important);
		//<eco:		add_parsed_property(atom_list_style_baseurl,		_t(""),			important);

				string_vector tokens;
				split_string(value, tokens, _t(" "), _t(""), _t("("));
				for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
				{
					int idx = get_list_style_type(tok->c_str(), -1, -1 );
					if(idx >= 0) {
						add_parsed_property(atom_list_style_type, *tok, important);
					}
					else
					{
						idx = get_list_style_position(tok->c_str(), -1, -1);
						if(idx >= 0) {
							add_parsed_property(atom_list_style_position, *tok, important);
						}
						else if(!t_strncmp(value, _t("url"), 3)) {
							//todo: check
							add_parsed_property(atom_list_style_image, *tok, important);
							if(baseurl)
							{
								add_parsed_property(atom_list_style_baseurl, baseurl, important);
							}
						}
					}
				}
				*/
			
				return;
			}
		
			// Add baseurl for background image
			case atom_list_style_image:
			{
				/*
				add_parsed_property(name, value, important);
				if(baseurl)
				{
					add_parsed_property(atom_list_style_baseurl, baseurl, important);
				}
				*/

				return;
			}

			// Parse background shorthand properties
			case atom_background:
			{
				parse_short_background(value, baseurl, important);
				return;
			}
		
			// Parse margin shorthand properties
			case atom_margin:
			{
				parse_margin( value, base_url, important );
				return;
			}
			// Parse padding shorthand properties
			case atom_padding:
			{
				parse_padding( value, base_url, important );
				return;
			}
		
			// Parse border-width/style/color shorthand properties
			case atom_border_width:
			{
				parse_border_width( value, baseurl, important );
				return;
			}

			case atom_border_style: 
			{
				parse_border_style( value, baseurl, important );
				return;
			}
			
			case atom_border_color:
			{
				parse_border_color( value, baseurl, important );
				return;
			}

			// Parse font shorthand properties
			case atom_font:
			{
				parse_short_font(value, important);
				return;
			}
		
			default: 
			{
				add_property(name, value, important);
				return;
			}
		}
	}

	void style::parse_short_background( const tstring& val, const tchar_t* baseurl, bool important )
	{
		//	todo: only if important
		add_prop(atom_background_color,			_t("transparent"),	important);
		add_prop(atom_background_image,			_t(""),				important);
		add_prop(atom_background_baseurl,		_t(""),				important);
		add_prop(atom_background_repeat,			_t("repeat"),		important);
		add_prop(atom_background_origin,			_t("padding-box"),	important);
		add_prop(atom_background_clip,			_t("border-box"),	important);
		add_prop(atom_background_attachment,		_t("scroll"),		important);

		if(val == _t("none")) {
			return;
		}

		string_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("("));
		bool origin_found = false;

		for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
		{
			if(tok->substr(0, 3) == _t("url"))
			{
				add_parsed_property(atom_background_image, *tok, important);
				if(baseurl)
				{
					add_parsed_property( atom_background_baseurl, baseurl, important);
				}

			}
			else if( get_background_repeat(tok->c_str(), -1, -1 )>=0 )
			{
				add_parsed_property( atom_background_repeat, *tok, important);
			}
			else if( get_background_attachment(tok->c_str(), -1, -1)>=0 )
			{
				add_parsed_property(atom_background_attachment, *tok, important);
			}
			else if( get_background_box(tok->c_str(), -1, -1)>=0 )
			{
				if(!origin_found) {
					add_parsed_property(atom_background_origin, *tok, important);
					origin_found = true;
				}
				else {
					add_parsed_property(atom_background_clip,*tok, important);
				}
			}
			else if(	get_background_position(tok->c_str(), -1, -1 ) ||
						iswdigit((*tok)[0]) ||
						(*tok)[0] == _t('-')	||
						(*tok)[0] == _t('.')	||
						(*tok)[0] == _t('+'))
			{
				if(m_properties.find(atom_background_position) != m_properties.end())
				{
					//todo: optimize
					m_properties[atom_background_position].m_value = m_properties[atom_background_position].m_value + _t(" ") + *tok;
				}
				else
				{
					add_parsed_property(atom_background_position, *tok, important);
				}
			}
			else if (web_color::is_color(tok->c_str()))
			{
				add_parsed_property(atom_background_color, *tok, important);
			}
		}
	}

	void style::parse_short_font( const tstring& val, bool important )
	{
		add_prop( atom_font_style,	_t("normal"),	important);
		add_prop( atom_font_variant,	_t("normal"),	important);
		add_prop( atom_font_weight,	_t("normal"),	important);
		add_prop( atom_font_size,		_t("medium"),	important);
		add_prop( atom_line_height,	_t("normal"),	important);

		string_vector tokens;
		split_string(val, tokens, _t(" "), _t(""), _t("\""));

		int idx = 0;
		bool was_normal = false;
		bool is_family = false;
		tstring font_family;
		for(string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++)
		{
			idx = get_font_style(tok->c_str(), -1, -1);

			if(!is_family) {
				if(idx >= 0) {
					if(idx == 0 && !was_normal) {
						add_parsed_property(atom_font_weight,		*tok, important);
						add_parsed_property(atom_font_variant,		*tok, important);
						add_parsed_property(atom_font_style,		*tok, important);
					}
					else {
						add_parsed_property(atom_font_style,		*tok, important);
					}
				}
				else {
					if( get_font_weight(tok->c_str(), tok->length(), -1)>=0 ) {
						add_parsed_property(atom_font_weight,		*tok, important);
					}
					else {
						if(get_font_variant(tok->c_str(), tok->length(), -1)>=0 ) {
							add_parsed_property(atom_font_variant,	*tok, important);
						}
						else if( iswdigit((*tok)[0]) ) {
							string_vector szlh;
							split_string(*tok, szlh, _t("/"));

							if(szlh.size() == 1) {
								add_parsed_property(atom_font_size,	szlh[0], important);
							}
							else if(szlh.size() >= 2)
							{
								add_parsed_property(atom_font_size,	szlh[0], important);
								add_parsed_property(atom_line_height,	szlh[1], important);
							}
						}
						else {
							is_family = true;
							font_family += *tok;
						}
					}
				}
			}
			else {
				font_family += *tok;
			}
		}

		add_parsed_property( atom_font_family, font_family, important );
	}

	void style::add_parsed_property(atom name, const css_value& val, bool important )
	{
		bool is_valid = true;

		//todo: why here ???
		if( name==atom_white_space ) {
			if( get_white_space( val.c_str(), val.length(), -1 )<0 ) {
				is_valid = false;
			}
		}

		if (is_valid) {
			props_map::iterator prop = m_properties.find(name);
			if (prop != m_properties.end())
			{
				if (!prop->second.m_important || (important && prop->second.m_important))
				{
					prop->second.m_value = val;
					prop->second.m_important = important;
				}
			}
			else
			{
				m_properties[name] = css_value(val.c_str(), important);
			}
		}
	}

	const css_value*	style::get_property( atom name ) const
	{
		if( name ) {
			for( uint32_t i=0; i<m_properties.length(); i++ ) {
				css_property*	a = m_properties[i];
				if( a->name==name ) {
					return &a->value;
				}
			}
		}

		return NULL;
	}


	/* no used
	void style::remove_property( atom name, bool important )
	{
		props_map::iterator prop = m_properties.find(name);
		if(prop != m_properties.end())
		{
			if( !prop->second.m_important || (important && prop->second.m_important) )
			{
				m_properties.erase(prop);
			}
		}
	}
	*/
}
