#pragma once

#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <sstream>

#include "os_types.h"
#include "types.h"
#include "background.h"
#include "borders.h"
#include "html_tag.h"
#include "css_color.h"
#include "media_query.h"

namespace litehtml {
	struct list_marker {
		xstring			image;
		const tchar_t*	baseurl;
		list_style_type	marker_type;
		web_color		color;
		position		pos;
	};

	// call back interface to draw text, images and other elements
	class document_container {
	public:
		virtual uint_ptr			create_font( const tchar_t* faceName, int size, int weight, bool italic, unsigned int decoration, font_metrics* fm ) = 0;
		virtual void				delete_font( uint_ptr hFont ) = 0;
		virtual int					text_width( const tchar_t* text, uint_ptr hFont ) = 0;
		virtual void				draw_text( uint_ptr hdc, const tchar_t* text, uint_ptr hFont, web_color color, const position& pos ) = 0;
		virtual int					pt_to_px( int pt ) = 0;
		virtual int					get_default_font_size() const = 0;
		virtual const tchar_t*		get_default_font_name() const = 0;
		virtual void				draw_list_marker( uint_ptr hdc, const list_marker& marker ) = 0;
		virtual void				load_image( const tchar_t* src, const tchar_t* baseurl, bool redraw_on_ready ) = 0;
		virtual void				get_image_size( const tchar_t* src, const tchar_t* baseurl, size& sz ) = 0;
		virtual void				draw_background( uint_ptr hdc, const background_paint& bg ) = 0;
		virtual void				draw_borders( uint_ptr hdc, const borders& borders, const position& draw_pos, bool root ) = 0;

		virtual	void				set_caption( const tchar_t* caption ) = 0;
		virtual	void				set_base_url( const tchar_t* base_url ) = 0;
		virtual void				link( const std::shared_ptr<document>& doc, const element::ptr& el ) = 0;
		virtual void				on_anchor_click( const tchar_t* url, const element::ptr& el ) = 0;
		virtual	void				set_cursor( const tchar_t* cursor ) = 0;
		virtual	void				transform_text( tstring& text, text_transform tt ) = 0;
		virtual void				import_css( xstring& text, const xstring& url, xstring& baseurl ) = 0;
		virtual void				set_clip( const position& pos, const border_radiuses& bdr_radius, bool valid_x, bool valid_y ) = 0;
		virtual void				del_clip() = 0;
		virtual void				get_client_rect( position& client ) const = 0;
		virtual element*			create_element( const tchar_t* tag_name, const attr_map& attributes, document* doc ) = 0;
		virtual void				get_media_features( media_features& media ) const = 0;
		virtual void				get_language( tstring& language, tstring& culture ) const = 0;
		virtual tstring				resolve_color( const tstring& /*color*/ ) const  {
			return tstring();
		}
	};

	void trim( tstring& s );
	void lcase( tstring& s );

	int	 value_index( const tstring& val, const tstring& strings, int defValue = -1, tchar_t delim = _t( ';' ) );
	bool value_in_list( const tstring& val, const tstring& strings, tchar_t delim = _t( ';' ) );

	int	 atom_index( const tchar_t* val, int defValue, ... );
	int	 atom_index( const tchar_t* val, int defValue, va_list lst );
	bool atom_in_list( atom val, ... );

	tstring::size_type find_close_bracket( const tstring& s, tstring::size_type off, tchar_t open_b = _t( '(' ), tchar_t close_b = _t( ')' ) );
	void split_string( const tstring& str, string_vector& tokens, const tstring& delims, const tstring& delims_preserve = _t( "" ), const tstring& quote = _t( "\"" ) );
	void join_string( tstring& str, const string_vector& tokens, const tstring& delims );

	inline int round_f( float val )
	{
		int int_val = ( int ) val;

		if( val - int_val >= 0.5 ) {
			int_val++;
		}

		return int_val;
	}

	inline int round_d( double val )
	{
		int int_val = ( int ) val;

		if( val - int_val >= 0.5 ) {
			int_val++;
		}

		return int_val;
	}

	inline const tchar_t*	skip_sp( const tchar_t* p )
	{
		tchar_t		ch;

		while( ( ch = *p ) == ' ' || ch == '\t' ) {
			p++;
		}

		return p;
	}

	inline const tchar_t*	skip_nsp( const tchar_t* p )
	{
		tchar_t		ch;

		while( ( ch = *p ) != 0 && ch != ' ' && ch != '\t' ) {
			p++;
		}

		return p;
	}

	void		atom_init( );
	atom		atom_create( const tchar_t* text, bool create_it = true );
	tstring		atom_get( atom v );

	void*		mem_alloc( size_t s );
	void		mem_free( void* p );
	tchar_t*	t_strdup( const tchar_t* s );
}
