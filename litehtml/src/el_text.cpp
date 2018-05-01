#include "html.h"
#include "el_text.h"
#include "document.h"

namespace litehtml {
	el_text::el_text( const tchar_t* text, document* doc )
		: element( doc )
	{
		if( text ) {
			m_text = text;
		}

		m_text_transform	= text_transform_none;
		m_use_transformed	= false;
		m_draw_spaces		= true;
	}

	el_text::~el_text()
	{
	}

	void el_text::get_content_size( size& sz, int /*max_width*/ )
	{
		sz = m_size;
	}

	void el_text::get_text( tstring& text )
	{
		text += m_text;
	}

	const tchar_t* el_text::get_style_property( atom name, bool inherited, const tchar_t* def /*= 0*/ )
	{
		if( inherited ) {
			element::ptr el_parent = parent();

			if( el_parent ) {
				return el_parent->get_style_property( name, inherited, def );
			}
		}

		return def;
	}

	void el_text::parse_styles( bool /*is_reparse*/ )
	{
		m_text_transform	= ( text_transform )get_text_transform( get_style_property( atom_text_transform, true,	_t( "none" ) ), -1, text_transform_none );

		if( m_text_transform != text_transform_none ) {
			m_transformed_text	= m_text;
			m_use_transformed = true;
			get_document()->container()->transform_text( m_transformed_text, m_text_transform );
		}

		if( is_white_space() ) {
			m_transformed_text = _t( " " );
			m_use_transformed = true;
		}
		else {
			if( m_text == _t( "\t" ) ) {
				m_transformed_text = _t( "    " );
				m_use_transformed = true;
			}

			if( m_text == _t( "\n" ) || m_text == _t( "\r" ) ) {
				m_transformed_text = _t( "" );
				m_use_transformed = true;
			}
		}

		font_metrics fm;
		uint_ptr font = 0;
		element::ptr el_parent = parent();

		if( el_parent ) {
			font = el_parent->get_font( &fm );
		}

		if( is_break() ) {
			m_size.height	= 0;
			m_size.width	= 0;
		}
		else {
			m_size.height	= fm.height;
			m_size.width	= get_document()->container()->text_width( m_use_transformed ? m_transformed_text.c_str() : m_text.c_str(), font );
		}

		m_draw_spaces = fm.draw_spaces;
	}

	int el_text::get_base_line()
	{
		element::ptr el_parent = parent();

		if( el_parent ) {
			return el_parent->get_base_line();
		}

		return 0;
	}

	void el_text::draw( uint_ptr hdc, int x, int y, const position* clip )
	{
		if( is_white_space() && !m_draw_spaces ) {
			return;
		}

		position pos = m_pos;
		pos.x	+= x;
		pos.y	+= y;

		if( pos.does_intersect( clip ) ) {
			element::ptr el_parent = parent();

			if( el_parent ) {
				document::ptr doc = get_document();
				uint_ptr font = el_parent->get_font();
				web_color color = el_parent->get_color( atom_color, true, doc->get_def_color() );
				doc->container()->draw_text( hdc, m_use_transformed ? m_transformed_text.c_str() : m_text.c_str(), font, color, pos );
			}
		}
	}

	int el_text::line_height() const
	{
		element::ptr el_parent = parent();

		if( el_parent ) {
			return el_parent->line_height();
		}

		return 0;
	}

	uint_ptr el_text::get_font( font_metrics* fm /*= 0*/ )
	{
		element::ptr el_parent = parent();

		if( el_parent ) {
			return el_parent->get_font( fm );
		}

		return 0;
	}

	style_display el_text::get_display() const
	{
		return style_display_inline_text;
	}

	white_space el_text::get_white_space() const
	{
		element::ptr el_parent = parent();

		if( el_parent ) {
			return el_parent->get_white_space();
		}

		return white_space_normal;
	}

	element_position el_text::get_element_position( css_offsets* offsets ) const
	{
		element::ptr p = parent();

		while( p && p->get_display() == style_display_inline ) {
			if( p->get_element_position() == element_position_relative ) {
				if( offsets ) {
					*offsets = p->get_css_offsets();
				}

				return element_position_relative;
			}

			p = p->parent();
		}

		return element_position_static;
	}

	css_offsets el_text::get_css_offsets() const
	{
		element::ptr p = parent();

		while( p && p->get_display() == style_display_inline ) {
			if( p->get_element_position() == element_position_relative ) {
				return p->get_css_offsets();
			}

			p = p->parent();
		}

		return css_offsets();
	}
}
