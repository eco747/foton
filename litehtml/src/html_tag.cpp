#include <algorithm>
#include <locale>

#include "html.h"
#include "html_tag.h"
#include "document.h"
#include "iterators.h"
#include "stylesheet.h"
#include "table.h"
#include "el_before_after.h"

namespace litehtml {

	/**
	 * constructor
	 **/

	html_tag::html_tag( document* doc )
		: element( doc )
	{
		m_box_sizing			= box_sizing_content_box;
		m_z_index				= 0;
		m_overflow				= overflow_visible;
		m_box					= 0;
		m_text_align			= text_align_left;
		m_el_position			= element_position_static;
		m_display				= style_display_inline;
		m_vertical_align		= vertical_align_baseline;
		m_list_style_type		= list_style_type_none;
		m_list_style_position	= list_style_position_outside;
		m_float					= element_float_none;
		m_clear					= element_clear_none;
		m_font					= 0;
		m_font_size				= 0;
		m_white_space			= white_space_normal;
		m_lh_predefined			= false;
		m_line_height			= 0;
		m_visibility			= visibility_visible;
		m_table_infos			= NULL;
		//m_table_infos->m_border_spacing_x		= 0;
		//m_table_infos->m_border_spacing_y		= 0;
		//m_table_infos->m_border_collapse		= border_collapse_separate;
		//m_floats_left		= NULL;
		//m_floats_left.last		= NULL;
		//m_floats_right.first	= NULL;
		//m_floats_right.last		= NULL;
		//m_attributes.first			= NULL;
		//m_attributes.last				= NULL;
		m_next_element			= NULL;
		m_next_positionned 		= NULL;
		//m_prev_element		= NULL;
		//m_children.first			= NULL;
		//m_children.last			= NULL;
	}

	/**
	 *
	 */

	html_tag::~html_tag()
	{
		// todo: clear elements
	}

	/**
	 *
	 */

	bool html_tag::appendChild( element* el )
	{
		if( !el ) {
			return false;
		}

		el->set_parent( this );

		if( m_children.first == NULL ) {
			m_children.first = m_children.last = el;
		}
		else {
			m_children.last->m_next_element = el;
			//el->m_prev_element 	= m_children.last;
			m_children.last = el;
		}

		return true;
	}

	/**
	 * todo: see if it's needed
	 */

	bool html_tag::removeChild( element* el )
	{
		if( !el || el->parent() != this ) {
			return false;
		}

		el->set_parent( NULL );

		if( m_children.first == el ) {
			if( el->m_next_element ) {
				el->m_next_element->m_prev_element = el->m_prev_element;
			}
			else {
				m_children.last = el->m_prev_element;
			}

			if( el->m_prev_element ) {
				el->m_prev_element->m_next_element = el->m_next_element;
			}
			else {
				m_children.first = el->m_next_element;
			}
		}

		return true;
	}

	/**
	 *
	 */

	void html_tag::clearRecursive()
	{
		for( element* el = m_children.first; el; el = el->m_next_element ) {
			el->clearRecursive( );
			el->set_parent( NULL );
		}

		m_children.first = m_children.last = NULL;
	}

	/**
	 *
	 */

	atom html_tag::get_tagName() const
	{
		return m_tag;
	}

	/**
	 *
	 */

	void html_tag::set_attr( atom name, const tchar_t* val )
	{
		if( name && val ) {
			attribute*	at = new attribute();
			at->m_name	= name;
			at->m_value = val;

			if( !m_attributes.first ) {
				m_attributes.first = m_attributes.last = at;
			}
			else {
				m_attributes.last->m_next = at;
				m_attributes.last = at;
			}

			//review: why ?
			//if( name==atom_class ) {
			//	m_class_values.resize( 0 );
			//	split_string( val, m_class_values, _t(" ") );
			//}
		}
	}

	/**
	 *
	 */

	const tchar_t* html_tag::get_attr( atom name, const tchar_t* def )
	{
		for( attribute* at = m_attributes.first; at; at = at->m_m_next ) {
			if( at->m_name == name ) {
				return at->m_value.c_str();
			}
		}

		//attr_map::const_iterator attr = m_attrs.find(name);
		//if(attr != m_attrs.end())
		//{
		//	return attr->second.c_str();
		//}
		//
		//return def;
	}

	/**
	 *
	 */

	void html_tag::get_content_size( size& sz, int max_width )
	{
		sz.height	= 0;

		if( m_display == style_display_block ) {
			sz.width	= max_width;
		}
		else {
			sz.width	= 0;
		}
	}

	/**
	 *
	 */

	uint_ptr html_tag::get_font( font_metrics* fm )
	{
		if( fm ) {
			*fm = m_font_metrics;
		}

		return m_font;
	}

	/**
	 *
	 */

	const css_value*	html_tag::get_style_property( atom name, bool inherited )
	{
		const css_value* ret = m_style.get_property( name );
		element*   el_parent = parent();

		if( el_parent ) {
			if( ( !ret && inherited ) || ( ret && ret->is_atom( atom_inherit ) ) ) {
				return	el_parent->get_style_property( name, inherited );
			}
		}

		return ret;
	}

	/**
	 *
	 */

	void html_tag::parse_styles( bool is_reparse )
	{
		const tchar_t* style = get_attr( atom_style );

		if( style ) {
			m_style.add( style, NULL );
		}

		init_font();
		document*	doc = get_document();
		const css_value* v;
		m_el_position	= element_position_fixed;
		m_text_align	= text_align_left;
		m_overflow		= overflow_visible;
		m_white_space	= white_space_normal;
		m_display		= style_display_inline;
		m_visibility	= visibility_visible;
		m_box_sizing	= box_sizing_content_box;
		m_z_index		= 0;
		m_vertical_align = vertical_align_baseline;
		m_float 		= element_float_none;
		m_clear 		= element_clear_none;
		m_line_height	= m_font_metrics.height;

		//	todo: lazy read values ???

		// read position style
		if( ( v = get_style_property( atom_position, false ) ) != NULL )	{
			if( v->is( css_value_type_element_position ) ) {
				m_el_position = ( element_position )v->int_val;
			}
		}

		//	read text-align style
		if( ( v = get_style_property( atom_text_align, false ) ) != NULL )	{
			if( v->is( css_value_type_text_align ) ) {
				m_el_position = ( element_position )v->int_val;
			}
		}

		//	read overflow style
		if( ( v = get_style_property( atom_overflow, false ) ) != NULL )	{
			if( v->is( css_value_type_overflow ) ) {
				m_overflow = ( overflow )v->int_val;
			}

			//	read white-space
			if( ( v = get_style_property( atom_white_space, false ) ) != NULL )	{
				if( v->is( css_value_type_white_space ) ) {
					m_white_space = ( white_space )v->int_val;
				}
			}

			//	read display
			if( ( v = get_style_property( atom_display, false ) ) != NULL )	{
				if( v->is( css_value_type_style_display ) ) {
					m_display	= ( style_display )v->int_val;
				}
			}

			//	read visibility
			if( ( v = get_style_property( atom_visibility, false ) ) != NULL )	{
				if( v->is( css_value_type_visibility ) ) {
					m_visibility = ( visibility )v->int_val;
				}
			}

			//	read box-sizing
			if( ( v = get_style_property( atom_box_sizing, false ) ) != NULL )	{
				if( v->is( css_value_type_box_sizing ) ) {
					m_box_sizing	= ( box_sizing )v->int_val;
				}
			}

			//	read z-index
			if( m_el_position != element_position_static ) {
				if( ( v = get_style_property( atom_z_index, false ) ) != NULL )	{
					if( v->is( css_value_type_int ) ) {
						m_z_index = v->int_val;
					}
				}
			}

			//	read baseline
			if( ( v = get_style_property( atom_vertical_align, false ) ) != NULL )	{
				if( v->is( css_value_type_vertical_align ) ) {
					m_vertical_align	= ( vertical_align )v->int_val;
				}
			}

			// read float
			if( ( v = get_style_property( atom_float, false ) ) != NULL )	{
				if( v->is( css_value_type_element_float ) ) {
					m_float = ( element_float )v->int_val;
				}
			}

			// read clear
			if( ( v = get_style_property( atom_clear, false ) ) != NULL )	{
				if( v->is( css_value_type_element_clear ) ) {
					m_clear = ( element_clear )v->int_val;
				}
			}

			//	adjust display
			if( m_float != element_float_none ) {
				// reset display in to block for floating elements
				if( m_display != style_display_none ) {
					m_display = style_display_block;
				}
			}
			else if( m_display == style_display_table ||
					 m_display == style_display_table_caption ||
					 m_display == style_display_table_cell ||
					 m_display == style_display_table_column ||
					 m_display == style_display_table_column_group ||
					 m_display == style_display_table_footer_group ||
					 m_display == style_display_table_header_group ||
					 m_display == style_display_table_row ||
					 m_display == style_display_table_row_group ) {
				doc->add_tabular( this );
			}
			else if( m_display == style_display_flex ) {
				doc->add_flex( this );
			}
			// fix inline boxes with absolute/fixed positions
			else if( m_display != style_display_none && is_inline_box() ) {
				if( m_el_position == element_position_absolute || m_el_position == element_position_fixed ) {
					m_display = style_display_block;
				}
			}

			// read text-indent
			if( ( v = get_style_property( atom_text_indent, false ) ) != NULL )	{
				if( v->is( css_value_type_css_length ) ) {
					m_css_text_indent = v->length_val;
					doc->cvt_units( m_css_text_indent, m_font_size );
				}
			}

			//	read width
			if( ( v = get_style_property( atom_width, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_width.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_width = v->length_val;
					doc->cvt_units( m_css_width, m_font_size );
				}
			}

			//	read height
			if( ( v = get_style_property( atom_height, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_height.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_height = v->length_val;
					doc->cvt_units( m_css_height, m_font_size );
				}
			}

			//	read min-width
			if( ( v = get_style_property( atom_min_width, false ) ) != NULL )	{
				if( v->is( css_value_type_css_length ) ) {
					m_css_min_width = v->length_val;
					doc->cvt_units( m_css_min_width, m_font_size );
				}
			}

			//	read min-height
			if( ( v = get_style_property( atom_min_height, false ) ) != NULL )	{
				if( v->is( css_value_type_css_length ) ) {
					m_css_min_height = v->length_val;
					doc->cvt_units( m_css_min_height, m_font_size );
				}
			}

			//	read max-width
			if( ( v = get_style_property( atom_max_width, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_max_width.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_max_width = v->length_val;
					doc->cvt_units( m_css_max_width, m_font_size );
				}
			}

			//	read max-height
			if( ( v = get_style_property( atom_max_height, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_max_height.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_max_height = v->length_val;
					doc->cvt_units( m_css_max_height, m_font_size );
				}
			}

			//	read left
			if( ( v = get_style_property( atom_left, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_offsets.left.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_offsets.left = v->length_val;
					doc->cvt_units( m_css_offsets.left, m_font_size );
				}
			}

			//	read top
			if( ( v = get_style_property( atom_top, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_offsets.top.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_offsets.top = v->length_val;
					doc->cvt_units( m_css_offsets.top, m_font_size );
				}
			}

			//	read right
			if( ( v = get_style_property( atom_right, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_offsets.right.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_offsets.right = v->length_val;
					doc->cvt_units( m_css_offsets.right, m_font_size );
				}
			}

			//	read bottom
			if( ( v = get_style_property( atom_bottom, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_offsets.bottom.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_offsets.bottom = v->length_val;
					doc->cvt_units( m_css_offsets.bottom, m_font_size );
				}
			}

			//	read margin-left
			if( ( v = get_style_property( atom_left, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_margins.left.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_margins.left = v->length_val;
					doc->cvt_units( m_css_margins.left, m_font_size );
				}
			}

			//	read margin-top
			if( ( v = get_style_property( atom_top, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_margins.top.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_margins.top = v->length_val;
					doc->cvt_units( m_css_margins.top, m_font_size );
				}
			}

			//	read margin-right
			if( ( v = get_style_property( atom_right, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_margins.right.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_margins.right = v->length_val;
					doc->cvt_units( m_css_margins.right, m_font_size );
				}
			}

			//	read margin-bottom
			if( ( v = get_style_property( atom_bottom, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_margins.bottom.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_margins.bottom = v->length_val;
					doc->cvt_units( m_css_margins.bottom, m_font_size );
				}
			}

			//	read padding-left
			if( ( v = get_style_property( atom_left, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_padding.left.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_padding.left = v->length_val;
					doc->cvt_units( m_css_padding.left, m_font_size );
				}
			}

			//	read padding-top
			if( ( v = get_style_property( atom_top, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_padding.top.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_padding.top = v->length_val;
					doc->cvt_units( m_css_padding.top, m_font_size );
				}
			}

			//	read padding-right
			if( ( v = get_style_property( atom_right, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_padding.right.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_padding.right = v->length_val;
					doc->cvt_units( m_css_padding.right, m_font_size );
				}
			}

			//	read padding-bottom
			if( ( v = get_style_property( atom_bottom, false ) ) != NULL )	{
				if( v->is_atom( atom_auto ) ) {
					m_css_padding.bottom.set_predef( atom_auto );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_padding.bottom = v->length_val;
					doc->cvt_units( m_css_padding.bottom, m_font_size );
				}
			}

			//	read border-left-width
			if( ( v = get_style_property( atom_border_left_width, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_borders.left.width.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.left.width = v->length_val;
					doc->cvt_units( m_css_borders.left.width, m_font_size );
				}
			}

			//	read border-top-width
			if( ( v = get_style_property( atom_border_top_width, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_borders.top.width.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.top.width = v->length_val;
					doc->cvt_units( m_css_borders.top.width, m_font_size );
				}
			}

			//	read border-right-width
			if( ( v = get_style_property( atom_border_right_width, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_borders.right.width.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.right.width = v->length_val;
					doc->cvt_units( m_css_borders.right.width, m_font_size );
				}
			}

			//	read border-bottom-width
			if( ( v = get_style_property( atom_border_bottom_width, false ) ) != NULL )	{
				if( v->is_atom( atom_none ) ) {
					m_css_borders.bottom.width.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.bottom.width = v->length_val;
					doc->cvt_units( m_css_borders.bottom.width, m_font_size );
				}
			}

			//	read border-left-color
			if( ( v = get_style_property( atom_border_left_color, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_color ) ) {
					m_css_borders.left.color = v->color_val;
				}
			}

			//	read border-top-color
			if( ( v = get_style_property( atom_border_top_color, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_color ) ) {
					m_css_borders.top.color = v->color_val;
				}
			}

			//	read border-right-color
			if( ( v = get_style_property( atom_border_right_color, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_color ) ) {
					m_css_borders.right.color = v->color_val;
				}
			}

			//	read border-bottom-color
			if( ( v = get_style_property( atom_border_bottom_color, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_color ) ) {
					m_css_borders.bottom.color = v->color_val;
				}
			}

			//	read border-left-style
			if( ( v = get_style_property( atom_border_left_style, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_border_style ) ) {
					m_css_borders.left.style = v->int_val;
				}
			}

			//	read border-top-style
			if( ( v = get_style_property( atom_border_top_style, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_border_style ) ) {
					m_css_borders.top.style = v->int_val;
				}
			}

			//	read border-right-style
			if( ( v = get_style_property( atom_border_right_style, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_border_style ) ) {
					m_css_borders.right.style = v->int_val;
				}
			}

			//	read border-bottom-style
			if( ( v = get_style_property( atom_border_bottom_style, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is( css_value_type_border_style ) ) {
					m_css_borders.bottom.style = v->int_val;
				}
			}

			//	read border-top-left-radius-x
			if( ( v = get_style_property( atom_border_top_left_radius_x, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.top_left.x.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.top_left.x = v->length_val;
					doc->cvt_units( m_css_borders.radius.top_left.x, m_font_size );
				}
			}

			//	read border-top-left-radius-y
			if( ( v = get_style_property( atom_border_top_left_radius_y, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.top_left.y.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.top_left.y = v->length_val;
					doc->cvt_units( m_css_borders.radius.top_left.y, m_font_size );
				}
			}

			//	read border-top-right-radius-x
			if( ( v = get_style_property( atom_border_top_right_radius_x, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.top_right.x.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.top_right.x = v->length_val;
					doc->cvt_units( m_css_borders.radius.top_right.x, m_font_size );
				}
			}

			//	read border-top-right-radius-y
			if( ( v = get_style_property( atom_border_top_right_radius_y, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.top_right.y.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.top_right.y = v->length_val;
					doc->cvt_units( m_css_borders.radius.top_right.y, m_font_size );
				}
			}

			//	read border-bottom-left-radius-x
			if( ( v = get_style_property( atom_border_bottom_left_radius_x, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.bottom_left.x.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.bottom_left.x = v->length_val;
					doc->cvt_units( m_css_borders.radius.bottom_left.x, m_font_size );
				}
			}

			//	read border-bottom-left-radius-y
			if( ( v = get_style_property( atom_border_bottom_left_radius_y, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.bottom_left.y.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.bottom_left.y = v->length_val;
					doc->cvt_units( m_css_borders.radius.bottom_left.y, m_font_size );
				}
			}

			//	read border-bottom-right-radius-x
			if( ( v = get_style_property( atom_border_bottom_right_radius_x, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.bottom_right.x.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.bottom_right.x = v->length_val;
					doc->cvt_units( m_css_borders.radius.bottom_right.x, m_font_size );
				}
			}

			//	read border-bottom-right-radius-y
			if( ( v = get_style_property( atom_border_bottom_right_radius_y, false ) ) != NULL )	{
				//todo: unset,inherit,initial
				if( v->is_atom( atom_none ) ) {
					m_css_borders.radius.bottom_right.y.set_predef( atom_none );
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_css_borders.radius.bottom_right.y = v->length_val;
					doc->cvt_units( m_css_borders.radius.bottom_right.y, m_font_size );
				}
			}

			//	read line-height

			if( ( v = get_style_property( atom_line_height, false ) ) != NULL )	{
				//todo: normal
				if( v->is_atom( atom_normal ) ) {
					;	//nothing to do
					m_lh_predefined = true;
				}
				else if( v->is( css_value_type_css_length ) ) {
					m_lh_predefined = false;

					// ex: 1.6
					if( v->length_val.units == css_units_none ) {
						m_line_height = ( int )( v->length_val.value * m_font_size );
					}
					else {
						m_line_height =  doc->cvt_units( v->length_val,	m_font_size, m_font_size );
					}
				}
			}

			/*
			if(m_display == style_display_list_item) {

				const tchar_t* list_type = get_style_property(atom_list_style_type, true, atom_disc );
				m_list_style_type = get_list_style_type( list_type, -1, list_style_type_disc );

				const tchar_t* list_pos = get_style_property(atom_list_style_position, true, atom_outside );
				m_list_style_position = get_list_style_position( list_pos, -1, list_style_position_outside );

				const tchar_t* list_image = get_style_property(atom_list_style_image, true, 0);

				if(list_image && list_image[0]) {
					xstring url;
					css::parse_css_url(list_image, url);

					const tchar_t* list_image_baseurl = get_style_property(atom_list_style_baseurl, true, 0);
					doc->container()->load_image(url.c_str(), list_image_baseurl, true);
				}
			}
			*/
			parse_background();

			if( !is_reparse ) {
				for( element* el = m_children.first; el; el = el->m_next_element ) {
					el->parse_styles();
				}
			}
		}

		/**
		 *
		 */
		int html_tag::render( int x, int y, int max_width, bool second_pass ) {
			if( m_display == style_display_table ||
					m_display == style_display_inline_table ) {
				return render_table( x, y, max_width, second_pass );
			}

			return render_box( x, y, max_width, second_pass );
		}
		/**
		 *
		 */
		bool html_tag::is_white_space() const {
			return false;
		}
		/**
		 *
		 */
		int html_tag::get_font_size() const {
			return m_font_size;
		}
		/**
		 *
		 */
		int html_tag::get_base_line() {
			if( is_replaced() ) {
				return 0;
			}

			int bl = 0;

			if( !m_boxes.empty() ) {
				bl = m_boxes.m_last->baseline() + content_margins_bottom();	//back
			}

			return bl;
		}
		/**
		 *
		 */
		void html_tag::init() {
			if( m_display == style_display_table || m_display == style_display_inline_table ) {
				__asm int 3;
				/*
				if( !m_table_infos ) {
					m_table_infos = std::unique_ptr<table_infos>(new table_infos());
				}

				if (m_table_infos->m_grid) {
					m_table_infos->m_grid->clear();
				}
				else {
					m_table_infos->m_grid = std::unique_ptr<table_grid>(new table_grid());
				}

				go_inside_table 		table_selector;
				table_rows_selector		row_selector;
				table_cells_selector	cell_selector;

				elements_iterator row_iter(this, &table_selector, &row_selector);

				element* row = row_iter.next(false);
				while (row)
				{
					m_table_infos->m_grid->begin_row(row);

					elements_iterator cell_iter(row, &table_selector, &cell_selector);
					element* cell = cell_iter.next();

					while (cell) {
						m_table_infos->m_grid->add_cell(cell);
						cell = cell_iter.next(false);
					}

					row = row_iter.next(false);
				}

				m_table_infos->m_grid->finish();
				*/
			}

			for( element* el = m_children.first; el; el = el->m_next_element ) {
				el->init();
			}
		}
	}

	/**
	 *
	 */

	int html_tag::select( const css_selector& selector, bool apply_pseudo )
	{
		element* el_parent = parent();

		if( !el_parent ) {
			return select_no_match;
		}

		int right_res = select( selector.m_right, apply_pseudo );

		if( right_res == select_no_match ) {
			return select_no_match;
		}

		if( !selector.m_left ) {
			return right_res;
		}

		switch( selector.m_combinator ) {
			case combinator_descendant: {
				bool is_pseudo = false;
				element* res = find_ancestor( *selector.m_left, apply_pseudo, &is_pseudo );

				if( !res ) {
					return select_no_match;
				}

				if( is_pseudo ) {
					right_res |= select_match_pseudo_class;
				}
			}
			break;

			case combinator_child: {
				int res = el_parent->select( *selector.m_left, apply_pseudo );

				if( res == select_no_match ) {
					return select_no_match;
				}
				else {
					if( right_res != select_match_pseudo_class ) {
						right_res |= res;
					}
				}
			}
			break;

			case combinator_adjacent_sibling: {
				bool is_pseudo = false;
				element* res = el_parent->find_adjacent_sibling( this, *selector.m_left, apply_pseudo, &is_pseudo );

				if( !res ) {
					return select_no_match;
				}
				else {
					if( is_pseudo ) {
						right_res |= select_match_pseudo_class;
					}
				}
			}
			break;

			case combinator_general_sibling: {
				bool is_pseudo = false;
				element* res =  el_parent->find_sibling( this, *selector.m_left, apply_pseudo, &is_pseudo );

				if( !res ) {
					return select_no_match;
				}
				else {
					if( is_pseudo ) {
						right_res |= select_match_pseudo_class;
					}
				}
			}
			break;

			default: {
				right_res = select_no_match;
			}
			break;
		}

		return right_res;
	}

	/**
	 *
	 */

	int html_tag::select( const css_element_selector& selector, bool apply_pseudo )
	{
		//if(!selector.m_tag.empty() && selector.m_tag != _t("*"))
		if( selector.m_tag != atom_null && selector.m_tag != atom_star ) {
			if( selector.m_tag != m_tag ) {
				return select_no_match;
			}
		}

		int 		res = select_match;
		element* 	el_parent = parent();

		//for(css_attribute_selector::vector::const_iterator i = selector.m_attrs.begin(); i != selector.m_attrs.end(); i++)

		for( css_attribute_selector* i = m_attributes.first; i; i = i->m_next ) {
			const tchar_t* attr_value = get_attr( i->attribute );

			switch( i->condition ) {
				case select_exists:
					if( !attr_value ) {
						return select_no_match;
					}

					break;

				case select_equal:
					if( !attr_value ) {
						return select_no_match;
					}

					if( i->attribute == atom_class ) {
						//	todo: atoms optim.
						/*
						const string_vector & tokens1 = m_class_values;
						const string_vector & tokens2 = i->class_val;
						bool found = true;

						for(string_vector::const_iterator str1 = tokens2.begin(); str1 != tokens2.end() && found; str1++) {
							bool f = false;
							for(string_vector::const_iterator str2 = tokens1.begin(); str2 != tokens1.end() && !f; str2++) {
								if( !t_strcasecmp(str1->c_str(), str2->c_str()) ) {
									f = true;
								}
							}

							if(!f) {
								found = false;
							}
						}

						if(!found) {
							return select_no_match;
						}
						*/
					}
					else {
						if( t_strcasecmp( i->val.c_str(), attr_value ) ) {
							return select_no_match;
						}
					}

					break;

				case select_contain_str:
					if( !attr_value ) {
						return select_no_match;
					}

					if( !t_strstr( attr_value, i->val.c_str() ) ) {
						return select_no_match;
					}

					break;

				case select_start_str:
					if( !attr_value ) {
						return select_no_match;
					}

					if( t_strncmp( attr_value, i->val.c_str(), i->val.length() ) ) {
						return select_no_match;
					}

					break;

				case select_end_str:
					if( !attr_value ) {
						return select_no_match;
					}

					if( t_strncmp( attr_value, i->val.c_str(), i->val.length() ) ) {
						const tchar_t* s = attr_value + t_strlen( attr_value ) - i->val.length() - 1;

						if( s < attr_value ) {
							return select_no_match;
						}

						if( i->val != s ) {
							return select_no_match;
						}
					}

					break;

				case select_pseudo_element:
					if( i->val == _t( "after" ) ) {
						res |= select_match_with_after;
					}
					else if( i->val == _t( "before" ) ) {
						res |= select_match_with_before;
					}
					else {
						return select_no_match;
					}

					break;

				case select_pseudo_class:
					if( apply_pseudo ) {
						if( !el_parent ) {
							return select_no_match;
						}

						tstring selector_param;
						tstring	selector_name;
						tstring::size_type begin	= i->val.find_first_of( _t( '(' ) );
						tstring::size_type end		= ( begin == tstring::npos ) ? tstring::npos : find_close_bracket( i->val, begin );

						if( begin != tstring::npos && end != tstring::npos ) {
							selector_param = i->val.substr( begin + 1, end - begin - 1 );
						}

						if( begin != tstring::npos ) {
							selector_name = i->val.substr( 0, begin );
							trim( selector_name );
						}
						else {
							selector_name = i->val;
						}

						int selector = get_pseudo_class( selector_name.c_str(), -1, -1 );

						switch( selector ) {
							case pseudo_class_only_child:
								if( !el_parent->is_only_child( this, false ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_only_of_type:
								if( !el_parent->is_only_child( this, true ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_first_child:
								if( !el_parent->is_nth_child( this, 0, 1, false ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_first_of_type:
								if( !el_parent->is_nth_child( this, 0, 1, true ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_last_child:
								if( !el_parent->is_nth_last_child( this, 0, 1, false ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_last_of_type:
								if( !el_parent->is_nth_last_child( this, 0, 1, true ) ) {
									return select_no_match;
								}

								break;

							case pseudo_class_nth_child:
							case pseudo_class_nth_of_type:
							case pseudo_class_nth_last_child:
							case pseudo_class_nth_last_of_type: {
								if( selector_param.empty() ) {
									return select_no_match;
								}

								int num = 0;
								int off = 0;
								parse_nth_child_params( selector_param, num, off );

								if( !num && !off ) {
									return select_no_match;
								}

								switch( selector ) {
									case pseudo_class_nth_child:
										if( !el_parent->is_nth_child( this, num, off, false ) ) {
											return select_no_match;
										}

										break;

									case pseudo_class_nth_of_type:
										if( !el_parent->is_nth_child( this, num, off, true ) ) {
											return select_no_match;
										}

										break;

									case pseudo_class_nth_last_child:
										if( !el_parent->is_nth_last_child( this, num, off, false ) ) {
											return select_no_match;
										}

										break;

									case pseudo_class_nth_last_of_type:
										if( !el_parent->is_nth_last_child( this, num, off, true ) ) {
											return select_no_match;
										}

										break;
								}
							}
							break;

							case pseudo_class_not: {
								css_element_selector sel;
								sel.parse( selector_param );

								if( select( sel, apply_pseudo ) ) {
									return select_no_match;
								}
							}
							break;

							case pseudo_class_lang: {
								trim( selector_param );

								if( !get_document()->match_lang( selector_param ) ) {
									return select_no_match;
								}
							}
							break;

							default:
								if( std::find( m_pseudo_classes.begin(), m_pseudo_classes.end(), i->val ) == m_pseudo_classes.end() ) {
									return select_no_match;
								}

								break;
						}
					}
					else {
						res |= select_match_pseudo_class;
					}

					break;
			}
		}

		return res;
	}

	/**
	 *
	 */

	element* html_tag::find_ancestor( const css_selector& selector, bool apply_pseudo, bool* is_pseudo )
	{
		element* el_parent = parent();

		if( !el_parent ) {
			return NULL;
		}

		int res = el_parent->select( selector, apply_pseudo );

		if( res != select_no_match ) {
			if( is_pseudo ) {
				if( res & select_match_pseudo_class ) {
					*is_pseudo = true;
				}
				else {
					*is_pseudo = false;
				}
			}

			return el_parent;
		}

		return el_parent->find_ancestor( selector, apply_pseudo, is_pseudo );
	}

	/**
	 *
	 */

	int html_tag::get_floats_height( element_float el_float ) const
	{
		int h = 0;

		if( is_floats_holder() ) {
			bool process = false;

			//for(const auto& fb : m_floats_left)
			for( floated_box* fb = m_floats_left.first; fb; fb = fb->next ) {
				process = false;

				switch( el_float ) {
					case element_float_none:
						process = true;
						break;

					case element_float_left:
						if( fb->clear_floats == element_clear_left ||
								fb->clear_floats == element_clear_both ) {
							process = true;
						}

						break;

					case element_float_right:
						if( fb->clear_floats == element_clear_right ||
								fb->clear_floats == element_clear_both ) {
							process = true;
						}

						break;
				}

				if( process ) {
					if( el_float == element_float_none ) {
						h = std::max( h, fb->pos.bottom() );
					}
					else {
						h = std::max( h, fb->pos.top() );
					}
				}
			}

			//for(const auto fb : m_floats_right)
			for( floated_box* fb = m_floats_right.first; fb; fb = fb->next ) {
				process = false;

				switch( el_float ) {
					case element_float_none:
						process = true;
						break;

					case element_float_left:
						if( fb->clear_floats == element_clear_left ||
								fb->clear_floats == element_clear_both ) {
							process = true;
						}

						break;

					case element_float_right:
						if( fb->clear_floats == element_clear_right ||
								fb->clear_floats == element_clear_both ) {
							process = true;
						}

						break;
				}

				if( process ) {
					if( el_float == element_float_none ) {
						h = std::max( h, fb->pos.bottom() );
					}
					else {
						h = std::max( h, fb->pos.top() );
					}
				}
			}

			return h;
		}

		element* el_parent = parent();

		if( el_parent ) {
			h = el_parent->get_floats_height( el_float );
			return h - m_pos.y;
		}

		return 0;
	}

	/**
	 *
	 */

	int html_tag::get_left_floats_height() const
	{
		int h = 0;

		if( is_floats_holder() ) {
			for( floated_box* fb = m_floats_left.first; fb; fb = fb->next ) {
				h = std::max( h, fb->pos.bottom() );
			}

			return h;
		}

		element* el_parent = parent();

		if( el_parent ) {
			h = el_parent->get_left_floats_height();
			return h - m_pos.y;
		}

		return 0;
	}

	/**
	 *
	 */

	int html_tag::get_right_floats_height() const
	{
		int h = 0;

		if( is_floats_holder() ) {
			for( floated_box* fb = m_floats_right.first; fb; fb = fb->next ) {
				h = std::max( h, fb.pos.bottom() );
			}

			return h;
		}

		element* el_parent = parent();

		if( el_parent ) {
			int h = el_parent->get_right_floats_height();
			return h - m_pos.y;
		}

		return 0;
	}

	/**
	 *
	 */

	int html_tag::get_line_left( int y )
	{
		if( is_floats_holder() ) {
			if( m_cache_line_left.is_valid && m_cache_line_left.hash == y ) {
				return m_cache_line_left.val;
			}

			int w = 0;

			for( floated_box* fb = m_floats_left.first; fb; fb = fb->next ) {
				if( y >= fb->pos.top() && y < fb->pos.bottom() ) {
					w = std::max( w, fb->pos.right() );

					if( w < fb->pos.right() ) {
						break;
					}
				}
			}

			m_cache_line_left.set_value( y, w );
			return w;
		}

		element* el_parent = parent();

		if( el_parent ) {
			int w = el_parent->get_line_left( y + m_pos.y );

			if( w < 0 ) {
				w = 0;
			}

			return w - ( w ? m_pos.x : 0 );
		}

		return 0;
	}

	/**
	 *
	 */

	int html_tag::get_line_right( int y, int def_right )
	{
		if( is_floats_holder() ) {
			if( m_cache_line_right.is_valid && m_cache_line_right.hash == y ) {
				if( m_cache_line_right.is_default ) {
					return def_right;
				}

				return std::min( m_cache_line_right.val, def_right );
			}

			int w = def_right;
			m_cache_line_right.is_default = true;

			for( floated_box* fb = m_floats_right.first; fb; fb = fb->next ) {
				{
					if( y >= fb.pos.top() && y < fb->pos.bottom() ) {
						w = std::min( w, fb->pos.left() );
						m_cache_line_right.is_default = false;

						if( w > fb->pos.left() ) {
							break;
						}
					}
				}
				m_cache_line_right.set_value( y, w );
				return w;
			}

			element* el_parent = parent();

			if( el_parent ) {
				int w = el_parent->get_line_right( y + m_pos.y, def_right + m_pos.x );
				return w - m_pos.x;
			}

			return 0;
		}

		/**
		 *
		 */
		void html_tag::get_line_left_right( int y, int def_right, int& ln_left, int& ln_right ) {
			if( is_floats_holder() ) {
				ln_left		= get_line_left( y );
				ln_right	= get_line_right( y, def_right );
			}
			else {
				element* el_parent = parent();

				if( el_parent ) {
					el_parent->get_line_left_right( y + m_pos.y, def_right + m_pos.x, ln_left, ln_right );
				}

				ln_right -= m_pos.x;
				ln_left -= m_pos.x;

				if( ln_left < 0 ) {
					ln_left = 0;
				}
			}
		}
		/**
		 *
		 */
		int html_tag::fix_line_width( int max_width, element_float flt ) {
			int ret_width = 0;

			if( !m_boxes.empty() ) {
				elements_vector els;
				//m_boxes.back()->get_elements(els);
				m_boxes.m_last->get_elements( els );
				bool was_cleared = false;

				if( !els.empty() && els.front()->get_clear() != element_clear_none ) {
					if( els.front()->get_clear() == element_clear_both ) {
						was_cleared = true;
					}
					else {
						if(	( flt == element_float_left	&& els.front()->get_clear() == element_clear_left ) ||
								( flt == element_float_right	&& els.front()->get_clear() == element_clear_right ) ) {
							was_cleared = true;
						}
					}
				}

				if( !was_cleared ) {
					m_boxes.pop_back();

					for( elements_vector::iterator i = els.begin(); i != els.end(); i++ ) {
						int rw = place_element( ( *i ), max_width );

						if( rw > ret_width ) {
							ret_width = rw;
						}
					}
				}
				else {
					int 	line_top = 0;
					box*	last = m_boxes.m_last;

					//if(m_boxes.back()->get_type() == box_line) {
					if( last->get_type() == box_line ) {
						//line_top = m_boxes.back()->top();
						line_top = last->top();
					}
					else {
						//line_top = m_boxes.back()->bottom();
						line_top = last->bottom();
					}

					int line_left	= 0;
					int line_right	= max_width;
					get_line_left_right( line_top, max_width, line_left, line_right );

					if( m_boxes.back()->get_type() == box_line ) {
						if(	m_boxes.size() == 1 &&
								m_list_style_type != list_style_type_none &&
								m_list_style_position == list_style_position_inside ) {
							int sz_font = get_font_size();
							line_left += sz_font;
						}

						if( m_css_text_indent.val() != 0 ) {
							bool line_box_found = false;

							//for(box::vector::iterator iter = m_boxes.begin(); iter < m_boxes.end(); iter++)
							for( box* b = m_boxes.m_first; b; b = b->next ) {
								if( b->get_type() == box_line ) {
									line_box_found = true;
									break;
								}
							}

							if( !line_box_found ) {
								line_left += m_css_text_indent.calc_percent( max_width );
							}
						}
					}

					elements_vector els;
					m_boxes.back()->new_width( line_left, line_right, els );

					for( auto& el : els ) {
						int rw = place_element( el, max_width );

						if( rw > ret_width ) {
							ret_width = rw;
						}
					}
				}
			}

			return ret_width;
		}
		/**
		 *
		 */
		void html_tag::add_float( element * el, int x, int y ) {
			if( !is_floats_holder() ) {
				element* el_parent = parent();

				if( el_parent ) {
					el_parent->add_float( el, x + m_pos.x, y + m_pos.y );
				}

				return;
			}

			floated_box*	fb = new floated_box;
			fb->pos.x			= el->left() + x;
			fb->pos.y			= el->top()  + y;
			fb->pos.width		= el->width();
			fb->pos.height		= el->height();
			fb->float_side		= el->get_float();
			fb->clear_floats	= el->get_clear();
			fb->el				= el;

			if( fb->float_side == element_float_left ) {
				if( !m_floats_left.first ) {
					m_floats_left.first = m_floats_left.last = fb;
				}
				else {
					bool inserted = false;

					for( floated_box* i = m_floats_left.first; i; i = i->next ) {
						//todo: optimiser right()
						if( fb->pos.right() > i->pos.right() ) {
							//todo: check
							__asm int 3;
							//m_floats_left.insert(i, std::move(fb));
							inserted = true;
							break;
						}
					}

					if( !inserted ) {
						//m_floats_left.push_back(std::move(fb));
						m_floats_left.last->next = fb;
						m_floats_left.last = fb;
					}
				}

				m_cache_line_left.invalidate();
			}
			else if( fb->float_side == element_float_right ) {
				if( !m_floats_right.first ) {
					m_floats_right.first = m_floats_right.last = fb;
				}
				else {
					bool inserted = false;

					for( floated_box* i = m_floats_right.first; i; i = i->next ) {
						if( fb->pos.left() < i->pos.left() ) {
							//todo: check
							__asm int 3;
							//m_floats_right.insert(i, std::move(fb));
							inserted = true;
							break;
						}
					}

					if( !inserted ) {
						m_floats_right.first->next = fb;
						m_floats_right.first = fb;
						//m_floats_right.push_back(fb);
					}
				}

				m_cache_line_right.invalidate();
			}
		}
		/**
		 *
		 */
		int html_tag::find_next_line_top( int top, int width, int def_right ) {
			if( !is_floats_holder() ) {
				element* el_parent = parent();

				if( el_parent ) {
					int new_top = el_parent->find_next_line_top( top + m_pos.y, width, def_right + m_pos.x );
					return new_top - m_pos.y;
				}

				return 0;
			}

			//todo: do not need an array ????
			int new_top = top;
			int_vector points;

			for( floated_box* fb = m_floats_left.first; fb; fb = fb->next ) {
				if( fb->pos.top() >= top ) {
					if( find( points.begin(), points.end(), fb.pos.top() ) == points.end() ) {
						points.push_back( fb.pos.top() );
					}
				}

				if( fb->pos.bottom() >= top ) {
					if( find( points.begin(), points.end(), fb.pos.bottom() ) == points.end() ) {
						points.push_back( fb.pos.bottom() );
					}
				}
			}

			for( floated_box* fb = m_floats_right.first; fb; fb = fb->next ) {
				{
					if( fb->pos.top() >= top ) {
						if( find( points.begin(), points.end(), fb.pos.top() ) == points.end() ) {
							points.push_back( fb.pos.top() );
						}
					}

					if( fb->pos.bottom() >= top ) {
						if( find( points.begin(), points.end(), fb.pos.bottom() ) == points.end() ) {
							points.push_back( fb.pos.bottom() );
						}
					}
				}

				if( !points.empty() ) {
					sort( points.begin(), points.end(), std::less<int>( ) );
					new_top = points.back();

					for( auto pt : points ) {
						int pos_left	= 0;
						int pos_right	= def_right;
						get_line_left_right( pt, def_right, pos_left, pos_right );

						if( pos_right - pos_left >= width ) {
							new_top = pt;
							break;
						}
					}
				}

				return new_top;
			}

			/**
			 *
			 */
			void html_tag::parse_background() {
				// parse background-color
				m_bg.m_color = get_color( atom_background_color, false, web_color( 0, 0, 0, 0 ) );
				// parse background-position
				const tchar_t* str = get_style_property( atom_background_position, false, _t( "0% 0%" ) );

				if( str ) {
					string_vector res;
					split_string( str, res, _t( " \t" ) );
					const tchar_t*	left;
					const tchar_t*	right = NULL;
					left = skip_sp( str );

					if( *left ) {
						right = skip_nsp( left );
						right = skip_sp( right );
					}

					if( *left ) {
						atom a_left = atom_create( left, false );

						if( *right == 0 ) {
							if( atom_in_list( a_left, 3, atom_left, atom_right, atom_center, ( atom )0 ) ) {
								m_bg.m_position.x.fromString( res[0], _t( "left;right;center" ) );
								m_bg.m_position.y.set_value( 50, css_units_perc );
							}
							else if( atom_in_list( res[0].c_str(), atom_tom, atom_bottom, atom_center, ( atom )0 ) ) {
								m_bg.m_position.y.fromString( res[0], _t( "top;bottom;center" ) );
								m_bg.m_position.x.set_value( 50, css_units_perc );
							}
							else {
								m_bg.m_position.x.fromString( res[0], atom_left, atom_right, atom_center, ( atom )0 );
								m_bg.m_position.y.set_value( 50, css_units_perc );
							}
						}
						else {
							if( atom_in_list( res[0].c_str(), atom_left, atom_right, ( atom )0 ) ) {
								m_bg.m_position.x.fromString( res[0], _t( "left;right;center" ) );
								m_bg.m_position.y.fromString( res[1], _t( "top;bottom;center" ) );
							}
							else if( atom_in_list( res[0].c_str(), atom_top, atom_bottom, ( atom )0 ) ) {
								m_bg.m_position.x.fromString( res[1], _t( "left;right;center" ) );
								m_bg.m_position.y.fromString( res[0], _t( "top;bottom;center" ) );
							}
							else if( atom_in_list( res[1].c_str(), atom_left, atom_right, ( atom )0 ) ) {
								m_bg.m_position.x.fromString( res[1], _t( "left;right;center" ) );
								m_bg.m_position.y.fromString( res[0], _t( "top;bottom;center" ) );
							}
							else if( atom_in_list( res[1].c_str(), atom_top, atom_bottom, ( atom )0 ) ) {
								m_bg.m_position.x.fromString( res[0], _t( "left;right;center" ) );
								m_bg.m_position.y.fromString( res[1], _t( "top;bottom;center" ) );
							}
							else {
								m_bg.m_position.x.fromString( res[0], _t( "left;right;center" ) );
								m_bg.m_position.y.fromString( res[1], _t( "top;bottom;center" ) );
							}
						}

						if( m_bg.m_position.x.is_predefined() ) {
							switch( m_bg.m_position.x.predef() ) {
								case 0:
									m_bg.m_position.x.set_value( 0, css_units_percentage );
									break;

								case 1:
									m_bg.m_position.x.set_value( 100, css_units_percentage );
									break;

								case 2:
									m_bg.m_position.x.set_value( 50, css_units_percentage );
									break;
							}
						}

						if( m_bg.m_position.y.is_predefined() ) {
							switch( m_bg.m_position.y.predef() ) {
								case 0:
									m_bg.m_position.y.set_value( 0, css_units_percentage );
									break;

								case 1:
									m_bg.m_position.y.set_value( 100, css_units_percentage );
									break;

								case 2:
									m_bg.m_position.y.set_value( 50, css_units_percentage );
									break;
							}
						}
					}
					else {
						m_bg.m_position.x.set_value( 0, css_units_percentage );
						m_bg.m_position.y.set_value( 0, css_units_percentage );
					}
				}
				else {
					m_bg.m_position.y.set_value( 0, css_units_percentage );
					m_bg.m_position.x.set_value( 0, css_units_percentage );
				}

				str = get_style_property( atom_background_size, false, atom_auto );

				if( str ) {
					string_vector res;
					split_string( str, res, _t( " \t" ) );

					if( !res.empty() ) {
						m_bg.m_position.width.fromString( res[0], background_size_atoms );

						if( res.size() > 1 ) {
							m_bg.m_position.height.fromString( res[1], background_size_atoms );
						}
						else {
							m_bg.m_position.height.predef( background_size_auto );
						}
					}
					else {
						m_bg.m_position.width.predef( background_size_auto );
						m_bg.m_position.height.predef( background_size_auto );
					}
				}

				document::ptr doc = get_document();
				doc->cvt_units( m_bg.m_position.x,		m_font_size );
				doc->cvt_units( m_bg.m_position.y,		m_font_size );
				doc->cvt_units( m_bg.m_position.width,	m_font_size );
				doc->cvt_units( m_bg.m_position.height,	m_font_size );
				// parse background_attachment
				m_bg.m_attachment = ( background_attachment ) atom_index( get_style_property( atom_background_attachment, false, atom_scroll ), background_attachment_scroll, background_attachment_atoms );
				// parse background_attachment
				m_bg.m_repeat = ( background_repeat ) atom_index( get_style_property( atom_background_repeat, false, atom_repeat ), background_repeat_repeat, background_repeat_atoms );
				// parse background_clip
				m_bg.m_clip = ( background_box ) atom_index( get_style_property( atom_background_clip, false, atom_border_box ), background_box_border, background_box_atoms );
				// parse background_origin
				m_bg.m_origin = ( background_box ) atom_index( get_style_property( atom_background_origin, false, atom_padding_box ), background_box_content, background_box_atoms );
				// parse background-image
				css::parse_css_url( get_style_property( atom_background_image, false, _t( "" ) ), m_bg.m_image );
				m_bg.m_baseurl = get_style_property( atom_background_baseurl, false, _t( "" ) );

				if( !m_bg.m_image.empty() ) {
					doc->container()->load_image( m_bg.m_image.c_str(), m_bg.m_baseurl.empty() ? 0 : m_bg.m_baseurl.c_str(), true );
				}
			}
			/**
			 *
			 */
			void html_tag::add_positioned( const element*& el ) {
				if( m_el_position != element_position_static || !have_parent() ) {
					m_positioned.push_back( el );
				}
				else {
					element* el_parent = parent();

					if( el_parent ) {
						el_parent->add_positioned( el );
					}
				}
			}
			/**
			 *
			 */
			void html_tag::calc_outlines( int parent_width ) {
				m_padding.left		= m_css_padding.left.calc_percent( parent_width );
				m_padding.right		= m_css_padding.right.calc_percent( parent_width );
				m_borders.left		= m_css_borders.left.width.calc_percent( parent_width );
				m_borders.right		= m_css_borders.right.width.calc_percent( parent_width );
				m_margins.left		= m_css_margins.left.calc_percent( parent_width );
				m_margins.right		= m_css_margins.right.calc_percent( parent_width );
				m_margins.top		= m_css_margins.top.calc_percent( parent_width );
				m_margins.bottom	= m_css_margins.bottom.calc_percent( parent_width );
				m_padding.top		= m_css_padding.top.calc_percent( parent_width );
				m_padding.bottom	= m_css_padding.bottom.calc_percent( parent_width );
			}
			/**
			 *
			 */
			void html_tag::calc_auto_margins( int parent_width ) {
				if( get_element_position() != element_position_absolute &&
						( m_display == display_block || m_display == display_table ) ) {
					if( m_css_margins.left.is_predefined() && m_css_margins.right.is_predefined() ) {
						int el_width = m_pos.width + m_borders.left + m_borders.right + m_padding.left + m_padding.right;

						if( el_width <= parent_width ) {
							m_margins.left = ( parent_width - el_width ) / 2;
							m_margins.right = ( parent_width - el_width ) - m_margins.left;
						}
						else {
							m_margins.left = 0;
							m_margins.right = 0;
						}
					}
					else if( m_css_margins.left.is_predefined() && !m_css_margins.right.is_predefined() ) {
						int el_width = m_pos.width + m_borders.left + m_borders.right + m_padding.left + m_padding.right + m_margins.right;
						m_margins.left = parent_width - el_width;

						if( m_margins.left < 0 ) {
							m_margins.left = 0;
						}
					}
					else if( !m_css_margins.left.is_predefined() && m_css_margins.right.is_predefined() ) {
						int el_width = m_pos.width + m_borders.left + m_borders.right + m_padding.left + m_padding.right + m_margins.left;
						m_margins.right = parent_width - el_width;

						if( m_margins.right < 0 ) {
							m_margins.right = 0;
						}
					}
				}
			}
			/**
			 *
			 */
			void html_tag::parse_attributes() {
				//for(auto& el : m_children)
				for( element* el = m_children.first; el; el = el->m_next_element ) {
					el->parse_attributes();
				}
			}
			/**
			 *
			 */
			void html_tag::get_text( tstring & text ) {
				for( element* el = m_children.first; el; el = el->m_next_element ) {
					el->get_text( text );
				}
			}
			/**
			 *
			 */
			bool html_tag::is_body()  const {
				// todo: flags ?
				return false;
			}
			/**
			 *
			 */
			void html_tag::set_data( const tchar_t* data ) {
			}
			/**
			 *
			 */
			void html_tag::get_inline_boxes( position::vector & boxes ) {
				box* old_box = 0;
				position pos;

				for( element* el = m_children.first; el; el = el->m_next_element ) {
					if( !el->skip() ) {
						if( el->m_box ) {
							if( el->m_box != old_box ) {
								if( old_box ) {
									if( boxes.empty() ) {
										pos.x		-= m_padding.left + m_borders.left;
										pos.width	+= m_padding.left + m_borders.left;
									}

									boxes.push_back( pos );
								}

								old_box		= el->m_box;
								pos.x		= el->left() + el->margin_left();
								pos.y		= el->top() - m_padding.top - m_borders.top;
								pos.width	= 0;
								pos.height	= 0;
							}

							pos.width	= el->right() - pos.x - el->margin_right() - el->margin_left();
							pos.height	= std::max( pos.height, el->height() + m_padding.top + m_padding.bottom + m_borders.top + m_borders.bottom );
						}
						else if( el->get_display() == display_inline ) {
							position::vector sub_boxes;
							el->get_inline_boxes( sub_boxes );

							if( !sub_boxes.empty() ) {
								sub_boxes.rbegin()->width += el->margin_right();

								if( boxes.empty() ) {
									if( m_padding.left + m_borders.left > 0 ) {
										position padding_box = ( *sub_boxes.begin() );
										padding_box.x		-= m_padding.left + m_borders.left + el->margin_left();
										padding_box.width	= m_padding.left + m_borders.left + el->margin_left();
										boxes.push_back( padding_box );
									}
								}

								sub_boxes.rbegin()->width += el->margin_right();
								boxes.insert( boxes.end(), sub_boxes.begin(), sub_boxes.end() );
							}
						}
					}
				}

				if( pos.width || pos.height ) {
					if( boxes.empty() ) {
						pos.x		-= m_padding.left + m_borders.left;
						pos.width	+= m_padding.left + m_borders.left;
					}

					boxes.push_back( pos );
				}

				if( !boxes.empty() ) {
					if( m_padding.right + m_borders.right > 0 ) {
						boxes.back().width += m_padding.right + m_borders.right;
					}
				}
			}
			/**
			 *
			 */
			bool html_tag::on_mouse_over() {
				bool ret = false;
				element* el = this;

				while( el ) {
					if( el->set_pseudo_class( _t( "hover" ), true ) ) {
						ret = true;
					}

					el = el->parent();
				}

				return ret;
			}
			/**
			 *
			 */
			bool html_tag::find_styles_changes( xVector<position>& redraw_boxes, int x, int y ) {
				if( m_display == display_inline_text ) {
					return false;
				}

				bool ret = false;
				bool apply = false;

				for( used_selector::vector::iterator iter = m_used_styles.begin(); iter != m_used_styles.end() && !apply; iter++ ) {
					if( ( *iter )->m_selector->is_media_valid() ) {
						int res = select( *( ( *iter )->m_selector ), true );

						if( ( res == select_no_match && ( *iter )->m_used ) || ( res == select_match && !( *iter )->m_used ) ) {
							apply = true;
						}
					}
				}

				if( apply ) {
					if( m_display == display_inline ||  m_display == display_table_row ) {
						position::vector boxes;
						get_inline_boxes( boxes );

						for( position::vector::iterator pos = boxes.begin(); pos != boxes.end(); pos++ ) {
							pos->x	+= x;
							pos->y	+= y;
							redraw_boxes.push_back( *pos );
						}
					}
					else {
						position pos = m_pos;

						if( m_el_position != element_position_fixed ) {
							pos.x += x;
							pos.y += y;
						}

						pos += m_padding;
						pos += m_borders;
						redraw_boxes.push_back( pos );
					}

					ret = true;
					refresh_styles();
					parse_styles();
				}

				for( element* el = m_children.first; el && !ret ; el = el->m_next_element ) {
					if( !el->skip() ) {
						if( m_el_position != element_position_fixed ) {
							if( el->find_styles_changes( redraw_boxes, x + m_pos.x, y + m_pos.y ) ) {
								ret = true;
							}
						}
						else {
							if( el->find_styles_changes( redraw_boxes, m_pos.x, m_pos.y ) ) {
								ret = true;
							}
						}
					}
				}

				return ret;
			}
			/**
			 *
			 */
			bool html_tag::on_mouse_leave() {
				bool ret = false;
				element* el = this;

				while( el ) {
					if( el->set_pseudo_class( _t( "hover" ), false ) ) {
						ret = true;
					}

					if( el->set_pseudo_class( _t( "active" ), false ) ) {
						ret = true;
					}

					el = el->parent();
				}

				return ret;
			}
			/**
			 *
			 */
			bool html_tag::on_lbutton_down() {
				bool ret = false;
				element* el = this;

				while( el ) {
					if( el->set_pseudo_class( _t( "active" ), true ) ) {
						ret = true;
					}

					el = el->parent();
				}

				return ret;
			}
			/**
			 *
			 */
			bool html_tag::on_lbutton_up() {
				bool ret = false;
				element* el = this;

				while( el ) {
					if( el->set_pseudo_class( _t( "active" ), false ) ) {
						ret = true;
					}

					el = el->parent();
				}

				on_click();
				return ret;
			}
			/**
			 *
			 */
			void html_tag::on_click() {
				element* el_parent = parent();

				if( el_parent ) {
					el_parent->on_click();
				}
			}
			/**
			 *
			 */
			const tchar_t* html_tag::get_cursor() {
				return get_style_property( atom_cursor, true, 0 );
			}
			static const int font_size_table[8][7] = {
				{ 9,    9,     9,     9,    11,    14,    18},
				{ 9,    9,     9,    10,    12,    15,    20},
				{ 9,    9,     9,    11,    13,    17,    22},
				{ 9,    9,    10,    12,    14,    18,    24},
				{ 9,    9,    10,    13,    16,    20,    26},
				{ 9,    9,    11,    14,    17,    21,    28},
				{ 9,   10,    12,    15,    17,    23,    30},
				{ 9,   10,    13,    16,    18,    24,    32}
			};
			void html_tag::init_font() {
				// initialize font size
				const tchar_t* str = get_style_property( atom_font_size, false, 0 );
				int parent_sz = 0;
				int doc_font_size = get_document()->container()->get_default_font_size();
				element* el_parent = parent();

				if( el_parent ) {
					parent_sz = el_parent->get_font_size();
				}
				else {
					parent_sz = doc_font_size;
				}

				m_font_size = parent_sz;

				if( str ) {
					css_length sz;
					sz.fromString( str, font_size_strings );

					if( sz.is_predefined() ) {
						int idx_in_table = doc_font_size - 9;

						if( idx_in_table >= 0 && idx_in_table <= 7 ) {
							if( sz.predef() >= fontSize_xx_small && sz.predef() <= fontSize_xx_large ) {
								m_font_size = font_size_table[idx_in_table][sz.predef()];
							}
							else {
								m_font_size = doc_font_size;
							}
						}
						else {
							switch( sz.predef() ) {
								case fontSize_xx_small:
									m_font_size = doc_font_size * 3 / 5;
									break;

								case fontSize_x_small:
									m_font_size = doc_font_size * 3 / 4;
									break;

								case fontSize_small:
									m_font_size = doc_font_size * 8 / 9;
									break;

								case fontSize_large:
									m_font_size = doc_font_size * 6 / 5;
									break;

								case fontSize_x_large:
									m_font_size = doc_font_size * 3 / 2;
									break;

								case fontSize_xx_large:
									m_font_size = doc_font_size * 2;
									break;

								default:
									m_font_size = doc_font_size;
									break;
							}
						}
					}
					else {
						if( sz.units() == css_units_percentage ) {
							m_font_size = sz.calc_percent( parent_sz );
						}
						else if( sz.units() == css_units_none ) {
							m_font_size = parent_sz;
						}
						else {
							m_font_size = get_document()->cvt_units( sz, parent_sz );
						}
					}
				}

				// initialize font
				const tchar_t* name			= get_style_property( atom_font_family,		true,	atom_inherit );
				const tchar_t* weight		= get_style_property( atom_font_weight,		true,	atom_normal );
				const tchar_t* style		= get_style_property( atom_font_style,		true,	atom_normal );
				const tchar_t* decoration	= get_style_property( atom_text_decoration,	true,	atom_none );
				m_font = get_document()->get_font( name, m_font_size, weight, style, decoration, &m_font_metrics );
			}
			/**
			 *
			 */
			bool html_tag::is_break() const {
				//todo: flags ?
				return false;
			}
			/**
			 *
			 */
			void html_tag::set_tagName( atom tag ) {
				m_tag = tag;
			}
			/**
			 *
			 */
			void html_tag::draw_background( uint_ptr hdc, int x, int y, const position * clip ) {
				position pos = m_pos;
				pos.x	+= x;
				pos.y	+= y;
				position el_pos = pos;
				el_pos += m_padding;
				el_pos += m_borders;

				if( m_display != display_inline && m_display != display_table_row ) {
					if( el_pos.does_intersect( clip ) ) {
						const background* bg = get_background();

						if( bg ) {
							background_paint bg_paint;
							init_background_paint( pos, bg_paint, bg );
							get_document()->container()->draw_background( hdc, bg_paint );
						}

						position border_box = pos;
						border_box += m_padding;
						border_box += m_borders;
						borders bdr = m_css_borders;
						bdr.radius = m_css_borders.radius.calc_percents( border_box.width, border_box.height );
						get_document()->container()->draw_borders( hdc, bdr, border_box, have_parent() ? false : true );
					}
				}
				else {
					const background* bg = get_background();
					position::vector boxes;
					get_inline_boxes( boxes );
					background_paint bg_paint;
					position content_box;

					for( position::vector::iterator box = boxes.begin(); box != boxes.end(); box++ ) {
						box->x	+= x;
						box->y	+= y;

						if( box->does_intersect( clip ) ) {
							content_box = *box;
							content_box -= m_borders;
							content_box -= m_padding;

							if( bg ) {
								init_background_paint( content_box, bg_paint, bg );
							}

							css_borders bdr;

							// set left borders radius for the first box
							if( box == boxes.begin() ) {
								bdr.radius.bottom_left_x	= m_css_borders.radius.bottom_left_x;
								bdr.radius.bottom_left_y	= m_css_borders.radius.bottom_left_y;
								bdr.radius.top_left_x		= m_css_borders.radius.top_left_x;
								bdr.radius.top_left_y		= m_css_borders.radius.top_left_y;
							}

							// set right borders radius for the last box
							if( box == boxes.end() - 1 ) {
								bdr.radius.bottom_right_x	= m_css_borders.radius.bottom_right_x;
								bdr.radius.bottom_right_y	= m_css_borders.radius.bottom_right_y;
								bdr.radius.top_right_x		= m_css_borders.radius.top_right_x;
								bdr.radius.top_right_y		= m_css_borders.radius.top_right_y;
							}

							bdr.top		= m_css_borders.top;
							bdr.bottom	= m_css_borders.bottom;

							if( box == boxes.begin() ) {
								bdr.left	= m_css_borders.left;
							}

							if( box == boxes.end() - 1 ) {
								bdr.right	= m_css_borders.right;
							}

							if( bg ) {
								bg_paint.border_radius = bdr.radius.calc_percents( bg_paint.border_box.width, bg_paint.border_box.width );
								get_document()->container()->draw_background( hdc, bg_paint );
							}

							borders b = bdr;
							b.radius = bdr.radius.calc_percents( box->width, box->height );
							get_document()->container()->draw_borders( hdc, b, *box, false );
						}
					}
				}
			}
			/**
			 *
			 */
			int html_tag::render_inline( const element*& container, int max_width ) {
				int ret_width = 0;
				int rw = 0;
				white_space ws = get_white_space();
				bool skip_spaces = false;

				if( ws == white_space_normal ||
						ws == white_space_nowrap ||
						ws == white_space_pre_line ) {
					skip_spaces = true;
				}

				bool was_space = false;

				for( element* el = m_children.first; el; el = el->m_next_element ) {
					// skip spaces to make rendering a bit faster
					if( skip_spaces ) {
						if( el->is_white_space() ) {
							if( was_space ) {
								el->skip( true );
								continue;
							}
							else {
								was_space = true;
							}
						}
						else {
							was_space = false;
						}
					}

					rw = container->place_element( el, max_width );

					if( rw > ret_width ) {
						ret_width = rw;
					}
				}

				return ret_width;
			}
			/**
			 *
			 */
			int html_tag::place_element( const element*& el, int max_width ) {
				if( el->get_display() == style_display_none ) {
					return 0;
				}

				if( el->get_display() == style_display_inline ) {
					return el->render_inline( this, max_width );
				}

				element_position el_position = el->get_element_position();

				if(	el_position == element_position_absolute ||
						el_position == element_position_fixed ) {
					int line_top = 0;

					if( !m_boxes.empty() ) {
						box*	last = m_boxes.m_last;

						//if(m_boxes.back()->get_type() == box_line) {
						if( last->get_type() == box_line ) {
							line_top = last->top();

							if( !last->is_empty() ) {
								line_top += line_height();
							}
						}
						else {
							line_top = last->bottom();
						}
					}

					el->render( 0, line_top, max_width );
					el->m_pos.x	+= el->content_margins_left();
					el->m_pos.y	+= el->content_margins_top();
					return 0;
				}

				int ret_width = 0;

				switch( el->get_float() ) {
					case float_left: {
						int line_top = 0;

						if( !m_boxes.empty() ) {
							box*	last = m_boxes.m_last;

							if( last->get_type() == box_line ) {
								line_top = last->top();
							}
							else {
								line_top = last->bottom();
							}
						}

						line_top		= get_cleared_top( el, line_top );
						int line_left	= 0;
						int line_right	= max_width;
						get_line_left_right( line_top, max_width, line_left, line_right );
						el->render( line_left, line_top, line_right );

						if( el->right() > line_right ) {
							int new_top = find_next_line_top( el->top(), el->width(), max_width );
							el->m_pos.x = get_line_left( new_top ) + el->content_margins_left();
							el->m_pos.y = new_top + el->content_margins_top();
						}

						add_float( el, 0, 0 );
						ret_width = fix_line_width( max_width, float_left );

						if( !ret_width ) {
							ret_width = el->right();
						}
					}
					break;

					case float_right: {
						int line_top = 0;

						if( !m_boxes.empty() ) {
							box*	last = m_boxes.m_last;

							if( last->get_type() == box_line ) {
								line_top = last->top();
							}
							else {
								line_top = last->bottom();
							}
						}

						line_top		= get_cleared_top( el, line_top );
						int line_left	= 0;
						int line_right	= max_width;
						get_line_left_right( line_top, max_width, line_left, line_right );
						el->render( 0, line_top, line_right );

						if( line_left + el->width() > line_right ) {
							int new_top = find_next_line_top( el->top(), el->width(), max_width );
							el->m_pos.x = get_line_right( new_top, max_width ) - el->width() + el->content_margins_left();
							el->m_pos.y = new_top + el->content_margins_top();
						}
						else {
							el->m_pos.x = line_right - el->width() + el->content_margins_left();
						}

						add_float( el, 0, 0 );
						ret_width = fix_line_width( max_width, float_right );

						if( !ret_width ) {
							line_left	= 0;
							line_right	= max_width;
							get_line_left_right( line_top, max_width, line_left, line_right );
							ret_width = ret_width + ( max_width - line_right );
						}
					}
					break;

					default: {
						line_context line_ctx;
						line_ctx.top = 0;

						if( !m_boxes.empty() ) {
							line_ctx.top = m_boxes.m_last->top();
						}

						line_ctx.left = 0;
						line_ctx.right = max_width;
						line_ctx.fix_top();
						get_line_left_right( line_ctx.top, max_width, line_ctx.left, line_ctx.right );

						switch( el->get_display() ) {
							case display_inline_block:
								ret_width = el->render( line_ctx.left, line_ctx.top, line_ctx.right );
								break;

							case display_block:
								if( el->is_replaced() || el->is_floats_holder() ) {
									element* el_parent = el->parent();
									el->m_pos.width = el->get_css_width().calc_percent( line_ctx.right - line_ctx.left );
									el->m_pos.height = el->get_css_height().calc_percent( el_parent ? el_parent->m_pos.height : 0 );
								}

								el->calc_outlines( line_ctx.right - line_ctx.left );
								break;

							case display_inline_text: {
								size sz;
								el->get_content_size( sz, line_ctx.right );
								el->m_pos = sz;
							}
							break;

							default:
								ret_width = 0;
								break;
						}

						bool add_box = true;

						if( !m_boxes.empty() ) {
							if( m_boxes.m_last->can_hold( el, m_white_space ) ) {
								add_box = false;
							}
						}

						if( add_box ) {
							new_box( el, max_width, line_ctx );
						}
						else if( !m_boxes.empty() ) {
							line_ctx.top = m_boxes.m_last->top();
						}

						if( line_ctx.top != line_ctx.calculatedTop ) {
							line_ctx.left = 0;
							line_ctx.right = max_width;
							line_ctx.fix_top();
							get_line_left_right( line_ctx.top, max_width, line_ctx.left, line_ctx.right );
						}

						if( !el->is_inline_box() ) {
							if( m_boxes.size() == 1 ) {
								if( collapse_top_margin() ) {
									int shift = el->margin_top();

									if( shift >= 0 ) {
										line_ctx.top -= shift;
										m_boxes.back()->y_shift( -shift );
									}
								}
							}
							else {
								int shift = 0;
								int prev_margin = m_boxes[m_boxes.size() - 2]->bottom_margin();

								if( prev_margin > el->margin_top() ) {
									shift = el->margin_top();
								}
								else {
									shift = prev_margin;
								}

								if( shift >= 0 ) {
									line_ctx.top -= shift;
									m_boxes.back()->y_shift( -shift );
								}
							}
						}

						switch( el->get_display() ) {
							case display_table:
							case display_list_item:
								ret_width = el->render( line_ctx.left, line_ctx.top, line_ctx.width() );
								break;

							case display_block:
							case display_table_cell:
							case display_table_caption:
							case display_table_row:
								if( el->is_replaced() || el->is_floats_holder() ) {
									ret_width = el->render( line_ctx.left, line_ctx.top, line_ctx.width() ) + line_ctx.left + ( max_width - line_ctx.right );
								}
								else {
									ret_width = el->render( 0, line_ctx.top, max_width );
								}

								break;

							default:
								ret_width = 0;
								break;
						}

						m_boxes.back()->add_element( el );

						if( el->is_inline_box() && !el->skip() ) {
							ret_width = el->right() + ( max_width - line_ctx.right );
						}
					}
					break;
				}

				return ret_width;
			}
			bool html_tag::set_pseudo_class( const tchar_t* pclass, bool add ) {
				bool ret = false;

				if( add ) {
					if( std::find( m_pseudo_classes.begin(), m_pseudo_classes.end(), pclass ) == m_pseudo_classes.end() ) {
						m_pseudo_classes.push_back( pclass );
						ret = true;
					}
				}
				else {
					string_vector::iterator pi = std::find( m_pseudo_classes.begin(), m_pseudo_classes.end(), pclass );

					if( pi != m_pseudo_classes.end() ) {
						m_pseudo_classes.erase( pi );
						ret = true;
					}
				}

				return ret;
			}
			bool html_tag::set_class( const tchar_t* pclass, bool add ) {
				string_vector classes;
				bool changed = false;
				split_string( pclass, classes, _t( " " ) );

				if( add ) {
					for( auto& _class : classes ) {
						if( std::find( m_class_values.begin(), m_class_values.end(), _class ) == m_class_values.end() ) {
							m_class_values.push_back( std::move( _class ) );
							changed = true;
						}
					}
				}
				else {
					for( const auto& _class : classes ) {
						auto end = std::remove( m_class_values.begin(), m_class_values.end(), _class );

						if( end != m_class_values.end() ) {
							m_class_values.erase( end, m_class_values.end() );
							changed = true;
						}
					}
				}

				if( changed ) {
					tstring class_string;
					join_string( class_string, m_class_values, _t( " " ) );
					set_attr( atom_class, class_string.c_str() );
					return true;
				}

				return false;
			}
			/**
			 *
			 */
			int html_tag::line_height() const {
				return m_line_height;
			}
			/**
			 *
			 */
			bool html_tag::is_replaced() const {
				//todo: flags ???
				return false;
			}
			/**
			 *
			 */
			int html_tag::finish_last_box( bool end_of_render ) {
				int line_top = 0;

				if( !m_boxes.empty() ) {
					m_boxes.m_last->finish( end_of_render );

					if( m_boxes.m_last->is_empty() ) {
						line_top = m_boxes.m_last->top();
						m_boxes.pop_back();
					}

					if( !m_boxes.empty() ) {
						line_top = m_boxes.m_last->bottom();
					}
				}

				return line_top;
			}
			/**
			 *
			 */
			int html_tag::new_box( const element*& el, int max_width, line_context & line_ctx ) {
				line_ctx.top = get_cleared_top( el, finish_last_box() );
				line_ctx.left = 0;
				line_ctx.right = max_width;
				line_ctx.fix_top();
				get_line_left_right( line_ctx.top, max_width, line_ctx.left, line_ctx.right );

				if( el->is_inline_box() || el->is_floats_holder() ) {
					if( el->width() > line_ctx.right - line_ctx.left ) {
						line_ctx.top = find_next_line_top( line_ctx.top, el->width(), max_width );
						line_ctx.left = 0;
						line_ctx.right = max_width;
						line_ctx.fix_top();
						get_line_left_right( line_ctx.top, max_width, line_ctx.left, line_ctx.right );
					}
				}

				int first_line_margin = 0;

				if( m_boxes.empty() && m_list_style_type != list_style_type_none && m_list_style_position == list_style_position_inside ) {
					int sz_font = get_font_size();
					first_line_margin = sz_font;
				}

				if( el->is_inline_box() ) {
					int text_indent = 0;

					if( m_css_text_indent.val() != 0 ) {
						bool line_box_found = false;

						//for(box::vector::iterator iter = m_boxes.begin(); iter != m_boxes.end(); iter++) {
						for( box* b = m_boxes.m_first; b; b = b->next ) {
							if( b->get_type() == box_line ) {
								line_box_found = true;
								break;
							}
						}

						if( !line_box_found ) {
							text_indent = m_css_text_indent.calc_percent( max_width );
						}
					}

					font_metrics fm;
					get_font( &fm );
					m_boxes.emplace_back( new line_box( line_ctx.top, line_ctx.left + first_line_margin + text_indent, line_ctx.right, line_height(), fm, m_text_align ) );
				}
				else {
					m_boxes.emplace_back( new block_box( line_ctx.top, line_ctx.left, line_ctx.right ) );
				}

				return line_ctx.top;
			}
			int html_tag::get_cleared_top( const element*& el, int line_top ) const {
				switch( el->get_clear() ) {
					case clear_left: {
						int fh = get_left_floats_height();

						if( fh && fh > line_top ) {
							line_top = fh;
						}
					}
					break;

					case clear_right: {
						int fh = get_right_floats_height();

						if( fh && fh > line_top ) {
							line_top = fh;
						}
					}
					break;

					case clear_both: {
						int fh = get_floats_height();

						if( fh && fh > line_top ) {
							line_top = fh;
						}
					}
					break;

					default:
						if( el->get_float() != float_none ) {
							int fh = get_floats_height( el->get_float() );

							if( fh && fh > line_top ) {
								line_top = fh;
							}
						}

						break;
				}

				return line_top;
			}
			/**
			 *
			 */
			style_display html_tag::get_display() const {
				return m_display;
			}
			/**
			 *
			 */
			element_float html_tag::get_float() const {
				return m_float;
			}
			/**
			 *
			 */
			bool html_tag::is_floats_holder() const {
				if(	m_display == display_inline_block ||
						m_display == display_table_cell ||
						!have_parent() ||
						is_body() ||
						m_float != float_none ||
						m_el_position == element_position_absolute ||
						m_el_position == element_position_fixed ||
						m_overflow > overflow_visible ) {
					return true;
				}

				return false;
			}
			/**
			 *
			 */
			bool html_tag::is_first_child_inline( const element*& el ) const {
				if( m_children.empty() ) {
					return false;
				}

				for( element* this_el = m_children.first; this_el; this_el = this_el->m_next_element ) {
					if( !this_el->is_white_space() ) {
						if( el == this_el ) {
							return true;
						}

						if( this_el->get_display() == display_inline ) {
							if( this_el->have_inline_child() ) {
								return false;
							}
						}
						else {
							//todo: break;
							return false;
						}
					}
				}

				return false;
			}
			/**
			 *
			 */
			bool html_tag::is_last_child_inline( const element*& el ) {
				if( m_children.empty() ) {
					return false;
				}

				//for (auto this_el = m_children.rbegin(); this_el < m_children.rend(); ++this_el) {
				for( element* this_el = m_children.last; this_el; this_el = this_el->m_prev_element ) {
					if( !this_el->is_white_space() ) {
						if( el == this_el ) {
							return true;
						}

						if( this_el->get_display() == display_inline ) {
							if( this_el->have_inline_child() ) {
								return false;
							}
						}
						else {
							//todo: break;
							return false;
						}
					}
				}

				return false;
			}
			/**
			 *
			 */
			white_space html_tag::get_white_space() const {
				return m_white_space;
			}
			/**
			 *
			 */
			vertical_align html_tag::get_vertical_align() const {
				return m_vertical_align;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_left() const {
				return m_css_offsets.left;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_right() const {
				return m_css_offsets.right;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_top() const {
				return m_css_offsets.top;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_bottom() const {
				return m_css_offsets.bottom;
			}
			/**
			 *
			 */
			css_offsets html_tag::get_css_offsets() const {
				return m_css_offsets;
			}
			/**
			 *
			 */
			element_clear html_tag::get_clear() const {
				return m_clear;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_width() const {
				return m_css_width;
			}
			/**
			 *
			 */
			css_length html_tag::get_css_height() const {
				return m_css_height;
			}
			/**
			 * see if needed
			 */
			size_t html_tag::get_children_count() const {
				return m_children.size();
			}
			/**
			 * see if needed
			 */
			element* html_tag::get_child( int idx ) const {
				return m_children[idx];
			}
			/**
			 *
			 */
			void html_tag::set_css_width( css_length & w ) {
				m_css_width = w;
			}
			/**
			 *
			 */
			void html_tag::apply_vertical_align() {
				if( !m_boxes.empty() ) {
					int add = 0;
					int content_height	= m_boxes.back()->bottom();

					if( m_pos.height > content_height ) {
						switch( m_vertical_align ) {
							case va_middle:
								add = ( m_pos.height - content_height ) / 2;
								break;

							case va_bottom:
								add = m_pos.height - content_height;
								break;

							default:
								add = 0;
								break;
						}
					}

					if( add ) {
						for( size_t i = 0; i < m_boxes.size(); i++ ) {
							m_boxes[i]->y_shift( add );
						}
					}
				}
			}
			/**
			 *
			 */
			element_position html_tag::get_element_position( css_offsets * offsets ) const {
				if( offsets && m_el_position != element_position_static ) {
					*offsets = m_css_offsets;
				}

				return m_el_position;
			}
			/**
			 *
			 */
			void html_tag::init_background_paint( position pos, background_paint & bg_paint, const background * bg ) {
				if( !bg ) {
					return;
				}

				bg_paint = *bg;
				position content_box	= pos;
				position padding_box	= pos;
				padding_box += m_padding;
				position border_box		= padding_box;
				border_box += m_borders;

				switch( bg->m_clip ) {
					case background_box_padding:
						bg_paint.clip_box = padding_box;
						break;

					case background_box_content:
						bg_paint.clip_box = content_box;
						break;

					default:
						bg_paint.clip_box = border_box;
						break;
				}

				switch( bg->m_origin ) {
					case background_box_border:
						bg_paint.origin_box = border_box;
						break;

					case background_box_content:
						bg_paint.origin_box = content_box;
						break;

					default:
						bg_paint.origin_box = padding_box;
						break;
				}

				if( !bg_paint.image.empty() ) {
					get_document()->container()->get_image_size( bg_paint.image.c_str(), bg_paint.baseurl.c_str(), bg_paint.image_size );

					if( bg_paint.image_size.width && bg_paint.image_size.height ) {
						size img_new_sz = bg_paint.image_size;
						double img_ar_width		= ( double ) bg_paint.image_size.width / ( double ) bg_paint.image_size.height;
						double img_ar_height	= ( double ) bg_paint.image_size.height / ( double ) bg_paint.image_size.width;

						if( bg->m_position.width.is_predefined() ) {
							switch( bg->m_position.width.predef() ) {
								case background_size_contain:
									if( ( int )( ( double ) bg_paint.origin_box.width * img_ar_height ) <= bg_paint.origin_box.height ) {
										img_new_sz.width = bg_paint.origin_box.width;
										img_new_sz.height	= ( int )( ( double ) bg_paint.origin_box.width * img_ar_height );
									}
									else {
										img_new_sz.height = bg_paint.origin_box.height;
										img_new_sz.width	= ( int )( ( double ) bg_paint.origin_box.height * img_ar_width );
									}

									break;

								case background_size_cover:
									if( ( int )( ( double ) bg_paint.origin_box.width * img_ar_height ) >= bg_paint.origin_box.height ) {
										img_new_sz.width = bg_paint.origin_box.width;
										img_new_sz.height	= ( int )( ( double ) bg_paint.origin_box.width * img_ar_height );
									}
									else {
										img_new_sz.height = bg_paint.origin_box.height;
										img_new_sz.width	= ( int )( ( double ) bg_paint.origin_box.height * img_ar_width );
									}

									break;
									break;

								case background_size_auto:
									if( !bg->m_position.height.is_predefined() ) {
										img_new_sz.height	= bg->m_position.height.calc_percent( bg_paint.origin_box.height );
										img_new_sz.width	= ( int )( ( double ) img_new_sz.height * img_ar_width );
									}

									break;
							}
						}
						else {
							img_new_sz.width = bg->m_position.width.calc_percent( bg_paint.origin_box.width );

							if( bg->m_position.height.is_predefined() ) {
								img_new_sz.height = ( int )( ( double ) img_new_sz.width * img_ar_height );
							}
							else {
								img_new_sz.height = bg->m_position.height.calc_percent( bg_paint.origin_box.height );
							}
						}

						bg_paint.image_size = img_new_sz;
						bg_paint.position_x = bg_paint.origin_box.x + ( int ) bg->m_position.x.calc_percent( bg_paint.origin_box.width - bg_paint.image_size.width );
						bg_paint.position_y = bg_paint.origin_box.y + ( int ) bg->m_position.y.calc_percent( bg_paint.origin_box.height - bg_paint.image_size.height );
					}
				}

				bg_paint.border_radius	= m_css_borders.radius.calc_percents( border_box.width, border_box.height );;
				bg_paint.border_box		= border_box;
				bg_paint.is_root		= have_parent() ? false : true;
			}
			/**
			 *
			 */
			visibility html_tag::get_visibility() const {
				return m_visibility;
			}
			void html_tag::draw_list_marker( uint_ptr hdc, const position & pos ) {
				list_marker lm;
				const tchar_t* list_image = get_style_property( atom_list_style_image, true, ( atom )0 );
				size img_size;

				if( list_image ) {
					css::parse_css_url( list_image, lm.image );
					lm.baseurl = get_style_property( atom_list_style_baseurl, true, ( atom )0 );
					get_document()->container()->get_image_size( lm.image.c_str(), lm.baseurl, img_size );
				}
				else {
					lm.baseurl = 0;
				}

				int ln_height	= line_height();
				int sz_font		= get_font_size();
				lm.pos.x		= pos.x;
				lm.pos.width	= sz_font	- sz_font * 2 / 3;
				lm.pos.height	= sz_font	- sz_font * 2 / 3;
				lm.pos.y		= pos.y		+ ln_height / 2 - lm.pos.height / 2;

				if( img_size.width && img_size.height ) {
					if( lm.pos.y + img_size.height > pos.y + pos.height ) {
						lm.pos.y = pos.y + pos.height - img_size.height;
					}

					if( img_size.width > lm.pos.width ) {
						lm.pos.x -= img_size.width - lm.pos.width;
					}

					lm.pos.width	= img_size.width;
					lm.pos.height	= img_size.height;
				}

				if( m_list_style_position == list_style_position_outside ) {
					lm.pos.x -= sz_font;
				}

				lm.color = get_color( atom_color, true, web_color( 0, 0, 0 ) );
				lm.marker_type = m_list_style_type;
				get_document()->container()->draw_list_marker( hdc, lm );
			}
			void html_tag::draw_children( uint_ptr hdc, int x, int y, const position * clip, draw_flag flag, int zindex ) {
				if( m_display == display_table || m_display == display_inline_table ) {
					draw_children_table( hdc, x, y, clip, flag, zindex );
				}
				else {
					draw_children_box( hdc, x, y, clip, flag, zindex );
				}
			}
			bool html_tag::fetch_positioned() {
				bool ret = false;
				m_positioned.clear();
				element_position el_pos;

				//for(auto& el : m_children) {
				for( element* el = m_children.first; el; el = el->m_next_element ) {
					el_pos = el->get_element_position();

					if( el_pos != element_position_static ) {
						add_positioned( el );
					}

					if( !ret && ( el_pos == element_position_absolute || el_pos == element_position_fixed ) ) {
						ret = true;
					}

					if( el->fetch_positioned() ) {
						ret = true;
					}
				}

				return ret;
			}
			int html_tag::get_zindex() const {
				return m_z_index;
			}
			void html_tag::render_positioned( render_type rt ) {
				position wnd_position;
				get_document()->container()->get_client_rect( wnd_position );
				element_position el_position;
				bool process;

				for( auto& el : m_positioned ) {
					el_position = el->get_element_position();
					process = false;

					if( el->get_display() != display_none ) {
						if( el_position == element_position_absolute ) {
							if( rt != render_fixed_only ) {
								process = true;
							}
						}
						else if( el_position == element_position_fixed ) {
							if( rt != render_no_fixed ) {
								process = true;
							}
						}
					}

					if( process ) {
						int parent_height	= 0;
						int parent_width	= 0;
						int client_x		= 0;
						int client_y		= 0;

						if( el_position == element_position_fixed ) {
							parent_height	= wnd_position.height;
							parent_width	= wnd_position.width;
							client_x		= wnd_position.left();
							client_y		= wnd_position.top();
						}
						else {
							element* el_parent = el->parent();

							if( el_parent ) {
								parent_height	= el_parent->height();
								parent_width	= el_parent->width();
							}
						}

						css_length	css_left	= el->get_css_left();
						css_length	css_right	= el->get_css_right();
						css_length	css_top		= el->get_css_top();
						css_length	css_bottom	= el->get_css_bottom();
						bool need_render = false;
						css_length el_w = el->get_css_width();
						css_length el_h = el->get_css_height();
						int new_width = -1;
						int new_height = -1;

						if( el_w.units() == css_units_percentage && parent_width ) {
							new_width = el_w.calc_percent( parent_width );

							if( el->m_pos.width != new_width ) {
								need_render = true;
								el->m_pos.width = new_width;
							}
						}

						if( el_h.units() == css_units_percentage && parent_height ) {
							new_height = el_h.calc_percent( parent_height );

							if( el->m_pos.height != new_height ) {
								need_render = true;
								el->m_pos.height = new_height;
							}
						}

						bool cvt_x = false;
						bool cvt_y = false;

						if( el_position == element_position_fixed ) {
							if( !css_left.is_predefined() || !css_right.is_predefined() ) {
								if( !css_left.is_predefined() && css_right.is_predefined() ) {
									el->m_pos.x = css_left.calc_percent( parent_width ) + el->content_margins_left();
								}
								else if( css_left.is_predefined() && !css_right.is_predefined() ) {
									el->m_pos.x = parent_width - css_right.calc_percent( parent_width ) - el->m_pos.width - el->content_margins_right();
								}
								else {
									el->m_pos.x		= css_left.calc_percent( parent_width ) + el->content_margins_left();
									el->m_pos.width	= parent_width - css_left.calc_percent( parent_width ) - css_right.calc_percent( parent_width ) - ( el->content_margins_left() + el->content_margins_right() );
									need_render = true;
								}
							}

							if( !css_top.is_predefined() || !css_bottom.is_predefined() ) {
								if( !css_top.is_predefined() && css_bottom.is_predefined() ) {
									el->m_pos.y = css_top.calc_percent( parent_height ) + el->content_margins_top();
								}
								else if( css_top.is_predefined() && !css_bottom.is_predefined() ) {
									el->m_pos.y = parent_height - css_bottom.calc_percent( parent_height ) - el->m_pos.height - el->content_margins_bottom();
								}
								else {
									el->m_pos.y			= css_top.calc_percent( parent_height ) + el->content_margins_top();
									el->m_pos.height	= parent_height - css_top.calc_percent( parent_height ) - css_bottom.calc_percent( parent_height ) - ( el->content_margins_top() + el->content_margins_bottom() );
									need_render = true;
								}
							}
						}
						else {
							if( !css_left.is_predefined() || !css_right.is_predefined() ) {
								if( !css_left.is_predefined() && css_right.is_predefined() ) {
									el->m_pos.x = css_left.calc_percent( parent_width ) + el->content_margins_left() - m_padding.left;
								}
								else if( css_left.is_predefined() && !css_right.is_predefined() ) {
									el->m_pos.x = m_pos.width + m_padding.right - css_right.calc_percent( parent_width ) - el->m_pos.width - el->content_margins_right();
								}
								else {
									el->m_pos.x		= css_left.calc_percent( parent_width ) + el->content_margins_left() - m_padding.left;
									el->m_pos.width	= m_pos.width + m_padding.left + m_padding.right - css_left.calc_percent( parent_width ) - css_right.calc_percent( parent_width ) -
													  ( el->content_margins_left() + el->content_margins_right() );

									if( new_width != -1 ) {
										el->m_pos.x += ( el->m_pos.width - new_width ) / 2;
										el->m_pos.width = new_width;
									}

									need_render = true;
								}

								cvt_x = true;
							}

							if( !css_top.is_predefined() || !css_bottom.is_predefined() ) {
								if( !css_top.is_predefined() && css_bottom.is_predefined() ) {
									el->m_pos.y = css_top.calc_percent( parent_height ) + el->content_margins_top() - m_padding.top;
								}
								else if( css_top.is_predefined() && !css_bottom.is_predefined() ) {
									el->m_pos.y = m_pos.height + m_padding.bottom - css_bottom.calc_percent( parent_height ) - el->m_pos.height - el->content_margins_bottom();
								}
								else {
									el->m_pos.y			= css_top.calc_percent( parent_height ) + el->content_margins_top() - m_padding.top;
									el->m_pos.height	= m_pos.height + m_padding.top + m_padding.bottom - css_top.calc_percent( parent_height ) - css_bottom.calc_percent( parent_height ) -
														  ( el->content_margins_top() + el->content_margins_bottom() );

									if( new_height != -1 ) {
										el->m_pos.y += ( el->m_pos.height - new_height ) / 2;
										el->m_pos.height = new_height;
									}

									need_render = true;
								}

								cvt_y = true;
							}
						}

						if( cvt_x || cvt_y ) {
							int offset_x = 0;
							int offset_y = 0;
							element* cur_el = el->parent();
							element* this_el = this;

							while( cur_el && cur_el != this_el ) {
								offset_x += cur_el->m_pos.x;
								offset_y += cur_el->m_pos.y;
								cur_el = cur_el->parent();
							}

							if( cvt_x )	{
								el->m_pos.x -= offset_x;
							}

							if( cvt_y )	{
								el->m_pos.y -= offset_y;
							}
						}

						if( need_render ) {
							position pos = el->m_pos;
							el->render( el->left(), el->top(), el->width(), true );
							el->m_pos = pos;
						}

						if( el_position == element_position_fixed ) {
							position fixed_pos;
							el->get_redraw_box( fixed_pos );
							get_document()->add_fixed_box( fixed_pos );
						}
					}

					el->render_positioned();
				}

				if( !m_positioned.empty() ) {
					std::stable_sort( m_positioned.begin(), m_positioned.end(), []( const element*& _Left, const element*& _Right ) {
						return ( _Left->get_zindex() < _Right->get_zindex() );
					} );
				}
			}
			void html_tag::draw_stacking_context( uint_ptr hdc, int x, int y, const position * clip, bool with_positioned ) {
				if( !is_visible() ) {
					return;
				}

				std::map<int, bool> zindexes;

				if( with_positioned ) {
					for( elements_vector::iterator i = m_positioned.begin(); i != m_positioned.end(); i++ ) {
						zindexes[( *i )->get_zindex()];
					}

					for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end(); idx++ ) {
						if( idx->first < 0 ) {
							draw_children( hdc, x, y, clip, draw_positioned, idx->first );
						}
					}
				}

				draw_children( hdc, x, y, clip, draw_block, 0 );
				draw_children( hdc, x, y, clip, draw_floats, 0 );
				draw_children( hdc, x, y, clip, draw_inlines, 0 );

				if( with_positioned ) {
					for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end(); idx++ ) {
						if( idx->first == 0 ) {
							draw_children( hdc, x, y, clip, draw_positioned, idx->first );
						}
					}

					for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end(); idx++ ) {
						if( idx->first > 0 ) {
							draw_children( hdc, x, y, clip, draw_positioned, idx->first );
						}
					}
				}
			}
			/**
			 *
			 */
			overflow html_tag::get_overflow() const {
				return m_overflow;
			}
			/**
			 *
			 */
			bool html_tag::is_nth_child( const element*& el, int num, int off, bool of_type ) const {
				int idx = 1;

				//for(const auto& child : m_children) {
				for( element* child = m_children.first; child; child = child->m_next_element ) {
					if( child->get_display() != display_inline_text ) {
						//if( (!of_type) || (of_type && !t_strcmp(el->get_tagName(), child->get_tagName())) )
						if( ( !of_type ) || ( of_type && el->get_tagName() == child->get_tagName() ) ) {
							if( el == child ) {
								if( num != 0 ) {
									if( ( idx - off ) >= 0 && ( idx - off ) % num == 0 ) {
										return true;
									}
								}
								else if( idx == off ) {
									return true;
								}

								return false;
							}

							idx++;
						}

						if( el == child ) {
							break;
						}
					}
				}

				return false;
			}
			/**
			 *
			 */
			bool html_tag::is_nth_last_child( const element*& el, int num, int off, bool of_type ) const {
				int idx = 1;

				//for(elements_vector::const_reverse_iterator child = m_children.rbegin(); child != m_children.rend(); child++) {
				for( element* child = m_children.last; child; child = child->m_prev_element ) {
					if( child->get_display() != display_inline_text ) {
						//if( !of_type || (of_type && !t_strcmp(el->get_tagName(), (*child)->get_tagName())) )
						if( !of_type || ( of_type && el->get_tagName() == child->get_tagName() ) ) {
							if( el == child ) {
								if( num != 0 ) {
									if( ( idx - off ) >= 0 && ( idx - off ) % num == 0 ) {
										return true;
									}
								}
								else if( idx == off ) {
									return true;
								}

								return false;
							}

							idx++;
						}

						if( el == ( *child ) ) {
							break;
						}
					}
				}

				return false;
			}
			/**
			 *
			 */
			void html_tag::parse_nth_child_params( tstring param, int& num, int& off ) {
				if( param == _t( "odd" ) ) {
					num = 2;
					off = 1;
				}
				else if( param == _t( "even" ) ) {
					num = 2;
					off = 0;
				}
				else {
					string_vector tokens;
					split_string( param, tokens, _t( " n" ), _t( "n" ) );
					tstring s_num;
					tstring s_off;
					tstring s_int;

					for( string_vector::iterator tok = tokens.begin(); tok != tokens.end(); tok++ ) {
						if( ( *tok ) == _t( "n" ) ) {
							s_num = s_int;
							s_int.clear();
						}
						else {
							s_int += ( *tok );
						}
					}

					s_off = s_int;
					num = t_atoi( s_num.c_str() );
					off = t_atoi( s_off.c_str() );
				}
			}
			void html_tag::calc_document_size( size & sz, int x /*= 0*/, int y /*= 0*/ ) {
				if( is_visible() && m_el_position != element_position_fixed ) {
					element::calc_document_size( sz, x, y );

					if( m_overflow == overflow_visible ) {
						for( element* el = m_children.first; el; el = el->m_next_element ) {
							el->calc_document_size( sz, x + m_pos.x, y + m_pos.y );
						}
					}

					// root element (<html>) must to cover entire window
					if( !have_parent() ) {
						position client_pos;
						get_document()->container()->get_client_rect( client_pos );
						m_pos.height = std::max( sz.height, client_pos.height ) - content_margins_top() - content_margins_bottom();
						m_pos.width	 = std::max( sz.width, client_pos.width ) - content_margins_left() - content_margins_right();
					}
				}
			}
			void html_tag::get_redraw_box( position & pos, int x /*= 0*/, int y /*= 0*/ ) {
				if( is_visible() ) {
					element::get_redraw_box( pos, x, y );

					if( m_overflow == overflow_visible ) {
						for( element* el = m_children.first; el; el = el->m_next_element ) {
							if( el->get_element_position() != element_position_fixed ) {
								el->get_redraw_box( pos, x + m_pos.x, y + m_pos.y );
							}
						}
					}
				}
			}
			element* html_tag::find_adjacent_sibling( const element*& el, const css_selector & selector, bool apply_pseudo /*= true*/, bool * is_pseudo /*= 0*/ ) {
				element* ret;

				for( element* e = m_children.first; e; e = e->m_next_element ) {
					if( e->get_display() != display_inline_text ) {
						if( e == el ) {
							if( ret ) {
								int res = ret->select( selector, apply_pseudo );

								if( res != select_no_match ) {
									if( is_pseudo ) {
										if( res & select_match_pseudo_class ) {
											*is_pseudo = true;
										}
										else {
											*is_pseudo = false;
										}
									}

									return ret;
								}
							}

							return 0;
						}
						else {
							ret = e;
						}
					}
				}

				return 0;
			}
			element* html_tag::find_sibling( const element*& el, const css_selector & selector, bool apply_pseudo /*= true*/, bool * is_pseudo /*= 0*/ ) {
				element* ret = 0;

				for( element* e = m_children.first; e; e = e->m_next_element ) {
					if( e->get_display() != display_inline_text ) {
						if( e == el ) {
							return ret;
						}
						else if( !ret ) {
							int res = e->select( selector, apply_pseudo );

							if( res != select_no_match ) {
								if( is_pseudo ) {
									if( res & select_match_pseudo_class ) {
										*is_pseudo = true;
									}
									else {
										*is_pseudo = false;
									}
								}

								ret = e;
							}
						}
					}
				}

				return 0;
			}
			bool html_tag::is_only_child( const element*& el, bool of_type ) const {
				int child_count = 0;

				for( element* child = m_children.first; child; child = child->m_next_element ) {
					if( child->get_display() != display_inline_text ) {
						//if( !of_type || (of_type && !t_strcmp(el->get_tagName(), child->get_tagName())) )
						if( !of_type || ( of_type && el->get_tagName() == child->get_tagName() ) ) {
							child_count++;
						}

						if( child_count > 1 ) {
							break;
						}
					}
				}

				if( child_count > 1 ) {
					return false;
				}

				return true;
			}
			void html_tag::update_floats( int dy, const element*& parent ) {
				if( is_floats_holder() ) {
					bool reset_cache = false;

					for( floated_box::vector::reverse_iterator fb = m_floats_left.rbegin(); fb != m_floats_left.rend(); fb++ ) {
						if( fb->el->is_ancestor( parent ) ) {
							reset_cache	= true;
							fb->pos.y	+= dy;
						}
					}

					if( reset_cache ) {
						m_cache_line_left.invalidate();
					}

					reset_cache = false;

					for( floated_box::vector::reverse_iterator fb = m_floats_right.rbegin(); fb != m_floats_right.rend(); fb++ ) {
						if( fb->el->is_ancestor( parent ) ) {
							reset_cache	= true;
							fb->pos.y	+= dy;
						}
					}

					if( reset_cache ) {
						m_cache_line_right.invalidate();
					}
				}
				else {
					element* el_parent = this->parent();

					if( el_parent ) {
						el_parent->update_floats( dy, parent );
					}
				}
			}
			void html_tag::remove_before_after() {
				if( !m_children.empty() ) {
					//if( !t_strcmp(m_children.front()->get_tagName(), _t("::before")) )
					if( m_children.front()->get_tagName() == atom___before ) {
						m_children.erase( m_children.begin() );
					}
				}

				if( !m_children.empty() ) {
					//if( !t_strcmp(m_children.back()->get_tagName(), _t("::after")) )
					if( m_children.back()->get_tagName() == atom___after ) {
						m_children.erase( m_children.end() - 1 );
					}
				}
			}
			element* html_tag::get_element_before() {
				if( !m_children.empty() ) {
					//if( !t_strcmp(m_children.front()->get_tagName(), _t("::before")) )
					if( m_children.front()->get_tagName() == atom___before ) {
						return m_children.front();
					}
				}

				element* el = new el_before( get_document() );
				el->parent( this );
				m_children.insert( m_children.begin(), el );
				return el;
			}
			element* html_tag::get_element_after() {
				if( !m_children.empty() ) {
					//if( !t_strcmp(m_children.back()->get_tagName(), _t("::after")) )
					if( m_children.back()->get_tagName() == atom___after ) {
						return m_children.back();
					}
				}

				element* el = std::make_shared<el_after>( get_document() );
				appendChild( el );
				return el;
			}
			void html_tag::add_style( const style & st ) {
				m_style.combine( st );
			}
			bool html_tag::have_inline_child() const {
				if( !m_children.empty() ) {
					for( element* el = m_children.first; el; el = el->m_next_element ) {
						if( !el->is_white_space() ) {
							return true;
						}
					}
				}

				return false;
			}
			void html_tag::refresh_styles() {
				remove_before_after();

				for( element* el = m_children.first; el; el = el->m_next_element ) {
					if( el->get_display() != display_inline_text ) {
						el->refresh_styles();
					}
				}

				m_style.clear();

				for( auto& usel : m_used_styles ) {
					usel->m_used = false;

					if( usel->m_selector->is_media_valid() ) {
						int apply = select( *usel->m_selector, false );

						if( apply != select_no_match ) {
							if( apply & select_match_pseudo_class ) {
								if( select( *usel->m_selector, true ) ) {
									if( apply & select_match_with_after ) {
										element* el = get_element_after();

										if( el ) {
											el->add_style( *usel->m_selector->m_style );
										}
									}
									else if( apply & select_match_with_before ) {
										element* el = get_element_before();

										if( el ) {
											el->add_style( *usel->m_selector->m_style );
										}
									}
									else {
										add_style( *usel->m_selector->m_style );
										usel->m_used = true;
									}
								}
							}
							else if( apply & select_match_with_after ) {
								element* el = get_element_after();

								if( el ) {
									el->add_style( *usel->m_selector->m_style );
								}
							}
							else if( apply & select_match_with_before ) {
								element* el = get_element_before();

								if( el ) {
									el->add_style( *usel->m_selector->m_style );
								}
							}
							else {
								add_style( *usel->m_selector->m_style );
								usel->m_used = true;
							}
						}
					}
				}
			}
			element* html_tag::get_child_by_point( int x, int y, int client_x, int client_y, draw_flag flag, int zindex ) {
				element* ret = 0;

				if( m_overflow > overflow_visible ) {
					if( !m_pos.is_point_inside( x, y ) ) {
						return ret;
					}
				}

				position pos = m_pos;
				pos.x	= x - pos.x;
				pos.y	= y - pos.y;

				for( elements_vector::reverse_iterator i = m_children.rbegin(); i != m_children.rend() && !ret; i++ ) {
					element* el = ( *i );

					if( el->is_visible() && el->get_display() != display_inline_text ) {
						switch( flag ) {
							case draw_positioned:
								if( el->is_positioned() && el->get_zindex() == zindex ) {
									if( el->get_element_position() == element_position_fixed ) {
										ret = el->get_element_by_point( client_x, client_y, client_x, client_y );

										if( !ret && ( *i )->is_point_inside( client_x, client_y ) ) {
											ret = ( *i );
										}
									}
									else {
										ret = el->get_element_by_point( pos.x, pos.y, client_x, client_y );

										if( !ret && ( *i )->is_point_inside( pos.x, pos.y ) ) {
											ret = ( *i );
										}
									}

									el = 0;
								}

								break;

							case draw_block:
								if( !el->is_inline_box() && el->get_float() == float_none && !el->is_positioned() ) {
									if( el->is_point_inside( pos.x, pos.y ) ) {
										ret = el;
									}
								}

								break;

							case draw_floats:
								if( el->get_float() != float_none && !el->is_positioned() ) {
									ret = el->get_element_by_point( pos.x, pos.y, client_x, client_y );

									if( !ret && ( *i )->is_point_inside( pos.x, pos.y ) ) {
										ret = ( *i );
									}

									el = 0;
								}

								break;

							case draw_inlines:
								if( el->is_inline_box() && el->get_float() == float_none && !el->is_positioned() ) {
									if( el->get_display() == display_inline_block ) {
										ret = el->get_element_by_point( pos.x, pos.y, client_x, client_y );
										el = 0;
									}

									if( !ret && ( *i )->is_point_inside( pos.x, pos.y ) ) {
										ret = ( *i );
									}
								}

								break;

							default:
								break;
						}

						if( el && !el->is_positioned() ) {
							if( flag == draw_positioned ) {
								element* child = el->get_child_by_point( pos.x, pos.y, client_x, client_y, flag, zindex );

								if( child ) {
									ret = child;
								}
							}
							else {
								if(	el->get_float() == float_none &&
										el->get_display() != display_inline_block ) {
									element* child = el->get_child_by_point( pos.x, pos.y, client_x, client_y, flag, zindex );

									if( child ) {
										ret = child;
									}
								}
							}
						}
					}
				}

				return ret;
			}
			element* html_tag::get_element_by_point( int x, int y, int client_x, int client_y ) {
				if( !is_visible() ) {
					return 0;
				}

				element* ret;
				std::map<int, bool> zindexes;

				for( elements_vector::iterator i = m_positioned.begin(); i != m_positioned.end(); i++ ) {
					zindexes[( *i )->get_zindex()];
				}

				for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end() && !ret; idx++ ) {
					if( idx->first > 0 ) {
						ret = get_child_by_point( x, y, client_x, client_y, draw_positioned, idx->first );
					}
				}

				if( ret ) {
					return ret;
				}

				for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end() && !ret; idx++ ) {
					if( idx->first == 0 ) {
						ret = get_child_by_point( x, y, client_x, client_y, draw_positioned, idx->first );
					}
				}

				if( ret ) {
					return ret;
				}

				ret = get_child_by_point( x, y, client_x, client_y, draw_inlines, 0 );

				if( ret ) {
					return ret;
				}

				ret = get_child_by_point( x, y, client_x, client_y, draw_floats, 0 );

				if( ret ) {
					return ret;
				}

				ret = get_child_by_point( x, y, client_x, client_y, draw_block, 0 );

				if( ret ) {
					return ret;
				}

				for( std::map<int, bool>::iterator idx = zindexes.begin(); idx != zindexes.end() && !ret; idx++ ) {
					if( idx->first < 0 ) {
						ret = get_child_by_point( x, y, client_x, client_y, draw_positioned, idx->first );
					}
				}

				if( ret ) {
					return ret;
				}

				if( m_el_position == element_position_fixed ) {
					if( is_point_inside( client_x, client_y ) ) {
						ret = this;
					}
				}
				else {
					if( is_point_inside( x, y ) ) {
						ret = this;
					}
				}

				return ret;
			}
			const background* html_tag::get_background( bool own_only ) {
				if( own_only ) {
					// return own background with check for empty one
					if( m_bg.m_image.empty() && !m_bg.m_color.alpha() ) {
						return 0;
					}

					return &m_bg;
				}

				if( m_bg.m_image.empty() && !m_bg.m_color.alpha() ) {
					// if this is root element (<html>) try to get background from body
					if( !have_parent() ) {
						for( const auto& el : m_children ) {
							if( el->is_body() ) {
								// return own body background
								return el->get_background( true );
							}
						}
					}

					return 0;
				}

				if( is_body() ) {
					element* el_parent = parent();

					if( el_parent ) {
						if( !el_parent->get_background( true ) ) {
							// parent of body will draw background for body
							return 0;
						}
					}
				}

				return &m_bg;
			}
			int html_tag::render_box( int x, int y, int max_width, bool second_pass /*= false*/ ) {
				int parent_width = max_width;
				calc_outlines( parent_width );
				m_pos.clear();
				m_pos.move_to( x, y );
				m_pos.x += content_margins_left();
				m_pos.y += content_margins_top();
				int ret_width = 0;
				def_value<int>	block_width( 0 );

				if( m_display != display_table_cell && !m_css_width.is_predefined() ) {
					int w = calc_width( parent_width );

					if( m_box_sizing == box_sizing_border_box ) {
						w -= m_padding.width() + m_borders.width();
					}

					ret_width = max_width = block_width = w;
				}
				else {
					if( max_width ) {
						max_width -= content_margins_left() + content_margins_right();
					}
				}

				// check for max-width (on the first pass only)
				if( !m_css_max_width.is_predefined() && !second_pass ) {
					int mw = get_document()->cvt_units( m_css_max_width, m_font_size, parent_width );

					if( m_box_sizing == box_sizing_border_box ) {
						mw -= m_padding.left + m_borders.left + m_padding.right + m_borders.right;
					}

					if( max_width > mw ) {
						max_width = mw;
					}
				}

				m_floats_left.clear();
				m_floats_right.clear();
				m_boxes.clear();
				m_cache_line_left.invalidate();
				m_cache_line_right.invalidate();
				element_position el_position;
				int block_height = 0;
				m_pos.height = 0;

				if( get_predefined_height( block_height ) ) {
					m_pos.height = block_height;
				}

				white_space ws = get_white_space();
				bool skip_spaces = false;

				if( ws == white_space_normal ||
						ws == white_space_nowrap ||
						ws == white_space_pre_line ) {
					skip_spaces = true;
				}

				bool was_space = false;

				for( auto el : m_children ) {
					// we don't need process absolute and fixed positioned element on the second pass
					if( second_pass ) {
						el_position = el->get_element_position();

						if( ( el_position == element_position_absolute || el_position == element_position_fixed ) ) {
							continue;
						}
					}

					// skip spaces to make rendering a bit faster
					if( skip_spaces ) {
						if( el->is_white_space() ) {
							if( was_space ) {
								el->skip( true );
								continue;
							}
							else {
								was_space = true;
							}
						}
						else {
							was_space = false;
						}
					}

					// place element into rendering flow
					int rw = place_element( el, max_width );

					if( rw > ret_width ) {
						ret_width = rw;
					}
				}

				finish_last_box( true );

				if( block_width.is_default() && is_inline_box() ) {
					m_pos.width = ret_width;
				}
				else {
					m_pos.width = max_width;
				}

				calc_auto_margins( parent_width );

				if( !m_boxes.empty() ) {
					if( collapse_top_margin() ) {
						int old_top = m_margins.top;
						m_margins.top = std::max( m_boxes.front()->top_margin(), m_margins.top );

						if( m_margins.top != old_top ) {
							update_floats( m_margins.top - old_top, this );
						}
					}

					if( collapse_bottom_margin() ) {
						m_margins.bottom = std::max( m_boxes.back()->bottom_margin(), m_margins.bottom );
						m_pos.height = m_boxes.back()->bottom() - m_boxes.back()->bottom_margin();
					}
					else {
						m_pos.height = m_boxes.back()->bottom();
					}
				}

				// add the floats height to the block height
				if( is_floats_holder() ) {
					int floats_height = get_floats_height();

					if( floats_height > m_pos.height ) {
						m_pos.height = floats_height;
					}
				}

				// calculate the final position
				m_pos.move_to( x, y );
				m_pos.x += content_margins_left();
				m_pos.y += content_margins_top();

				if( get_predefined_height( block_height ) ) {
					m_pos.height = block_height;
				}

				int min_height = 0;

				if( !m_css_min_height.is_predefined() && m_css_min_height.units() == css_units_percentage ) {
					element* el_parent = parent();

					if( el_parent ) {
						if( el_parent->get_predefined_height( block_height ) ) {
							min_height = m_css_min_height.calc_percent( block_height );
						}
					}
				}
				else {
					min_height = ( int )m_css_min_height.val();
				}

				if( min_height != 0 && m_box_sizing == box_sizing_border_box ) {
					min_height -= m_padding.top + m_borders.top + m_padding.bottom + m_borders.bottom;

					if( min_height < 0 ) {
						min_height = 0;
					}
				}

				if( m_display == display_list_item ) {
					const tchar_t* list_image = get_style_property( atom_list_style_image, true, 0 );

					if( list_image ) {
						xstring url;
						css::parse_css_url( list_image, url );
						size sz;
						const tchar_t* list_image_baseurl = get_style_property( atom_list_style_baseurl, true, 0 );
						get_document()->container()->get_image_size( url.c_str(), list_image_baseurl, sz );

						if( min_height < sz.height ) {
							min_height = sz.height;
						}
					}
				}

				if( min_height > m_pos.height ) {
					m_pos.height = min_height;
				}

				int min_width = m_css_min_width.calc_percent( parent_width );

				if( min_width != 0 && m_box_sizing == box_sizing_border_box ) {
					min_width -= m_padding.left + m_borders.left + m_padding.right + m_borders.right;

					if( min_width < 0 ) {
						min_width = 0;
					}
				}

				if( min_width != 0 ) {
					if( min_width > m_pos.width ) {
						m_pos.width = min_width;
					}

					if( min_width > ret_width ) {
						ret_width = min_width;
					}
				}

				ret_width += content_margins_left() + content_margins_right();

				// re-render with new width
				if( ret_width < max_width && !second_pass && have_parent() ) {
					if( m_display == display_inline_block ||
							m_css_width.is_predefined() &&
							( m_float != float_none ||
							  m_display == display_table ||
							  m_el_position == element_position_absolute ||
							  m_el_position == element_position_fixed
							)
					  ) {
						render( x, y, ret_width, true );
						m_pos.width = ret_width - ( content_margins_left() + content_margins_right() );
					}
				}

				if( is_floats_holder() && !second_pass ) {
					for( const auto& fb : m_floats_left ) {
						fb.el->apply_relative_shift( fb.el->parent()->calc_width( m_pos.width ) );
					}
				}

				return ret_width;
			}
			int html_tag::render_table( int x, int y, int max_width, bool second_pass /*= false*/ ) {
				if( !m_table_infos || !m_table_infos->m_grid ) {
					return 0;
				}

				int parent_width = max_width;
				calc_outlines( parent_width );
				m_pos.clear();
				m_pos.move_to( x, y );
				m_pos.x += content_margins_left();
				m_pos.y += content_margins_top();
				def_value<int>	block_width( 0 );

				if( !m_css_width.is_predefined() ) {
					max_width = block_width = calc_width( parent_width ) - m_padding.width() - m_borders.width();
				}
				else {
					if( max_width ) {
						max_width -= content_margins_left() + content_margins_right();
					}
				}

				// Calculate table spacing
				int table_width_spacing = 0;

				if( m_table_infos->m_border_collapse == border_collapse_separate ) {
					table_width_spacing = m_table_infos->m_border_spacing_x * ( m_table_infos->m_grid->cols_count() + 1 );
				}
				else {
					table_width_spacing = 0;

					if( m_table_infos->m_grid->cols_count() ) {
						table_width_spacing -= std::min( border_left(), m_table_infos->m_grid->column( 0 ).border_left );
						table_width_spacing -= std::min( border_right(), m_table_infos->m_grid->column( m_table_infos->m_grid->cols_count() - 1 ).border_right );
					}

					for( int col = 1; col < m_table_infos->m_grid->cols_count(); col++ ) {
						table_width_spacing -= std::min( m_table_infos->m_grid->column( col ).border_left, m_table_infos->m_grid->column( col - 1 ).border_right );
					}
				}

				// Calculate the minimum content width (MCW) of each cell: the formatted content may span any number of lines but may not overflow the cell box.
				// If the specified 'width' (W) of the cell is greater than MCW, W is the minimum cell width. A value of 'auto' means that MCW is the minimum
				// cell width.
				//
				// Also, calculate the "maximum" cell width of each cell: formatting the content without breaking lines other than where explicit line breaks occur.

				if( m_table_infos->m_grid->cols_count() == 1 && !block_width.is_default() ) {
					for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
						table_cell* cell = m_table_infos->m_grid->cell( 0, row );

						if( cell && cell->el ) {
							cell->min_width = cell->max_width = cell->el->render( 0, 0, max_width - table_width_spacing );
							cell->el->m_pos.width = cell->min_width - cell->el->content_margins_left() - cell->el->content_margins_right();
						}
					}
				}
				else {
					for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
						for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
							table_cell* cell = m_table_infos->m_grid->cell( col, row );

							if( cell && cell->el ) {
								if( !m_table_infos->m_grid->column( col ).css_width.is_predefined() && m_table_infos->m_grid->column( col ).css_width.units() != css_units_percentage ) {
									int css_w = m_table_infos->m_grid->column( col ).css_width.calc_percent( block_width );
									int el_w = cell->el->render( 0, 0, css_w );
									cell->min_width = cell->max_width = std::max( css_w, el_w );
									cell->el->m_pos.width = cell->min_width - cell->el->content_margins_left() - cell->el->content_margins_right();
								}
								else {
									// calculate minimum content width
									cell->min_width = cell->el->render( 0, 0, 1 );
									// calculate maximum content width
									cell->max_width = cell->el->render( 0, 0, max_width - table_width_spacing );
								}
							}
						}
					}
				}

				// For each column, determine a maximum and minimum column width from the cells that span only that column.
				// The minimum is that required by the cell with the largest minimum cell width (or the column 'width', whichever is larger).
				// The maximum is that required by the cell with the largest maximum cell width (or the column 'width', whichever is larger).

				for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
					m_table_infos->m_grid->column( col ).max_width = 0;
					m_table_infos->m_grid->column( col ).min_width = 0;

					for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
						if( m_table_infos->m_grid->cell( col, row )->colspan <= 1 ) {
							m_table_infos->m_grid->column( col ).max_width = std::max( m_table_infos->m_grid->column( col ).max_width, m_table_infos->m_grid->cell( col, row )->max_width );
							m_table_infos->m_grid->column( col ).min_width = std::max( m_table_infos->m_grid->column( col ).min_width, m_table_infos->m_grid->cell( col, row )->min_width );
						}
					}
				}

				// For each cell that spans more than one column, increase the minimum widths of the columns it spans so that together,
				// they are at least as wide as the cell. Do the same for the maximum widths.
				// If possible, widen all spanned columns by approximately the same amount.

				for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
					for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
						if( m_table_infos->m_grid->cell( col, row )->colspan > 1 ) {
							int max_total_width = m_table_infos->m_grid->column( col ).max_width;
							int min_total_width = m_table_infos->m_grid->column( col ).min_width;

							for( int col2 = col + 1; col2 < col + m_table_infos->m_grid->cell( col, row )->colspan; col2++ ) {
								max_total_width += m_table_infos->m_grid->column( col2 ).max_width;
								min_total_width += m_table_infos->m_grid->column( col2 ).min_width;
							}

							if( min_total_width < m_table_infos->m_grid->cell( col, row )->min_width ) {
								m_table_infos->m_grid->distribute_min_width( m_table_infos->m_grid->cell( col, row )->min_width - min_total_width, col, col + m_table_infos->m_grid->cell( col, row )->colspan - 1 );
							}

							if( max_total_width < m_table_infos->m_grid->cell( col, row )->max_width ) {
								m_table_infos->m_grid->distribute_max_width( m_table_infos->m_grid->cell( col, row )->max_width - max_total_width, col, col + m_table_infos->m_grid->cell( col, row )->colspan - 1 );
							}
						}
					}
				}

				// If the 'table' or 'inline-table' element's 'width' property has a computed value (W) other than 'auto', the used width is the
				// greater of W, CAPMIN, and the minimum width required by all the columns plus cell spacing or borders (MIN).
				// If the used width is greater than MIN, the extra width should be distributed over the columns.
				//
				// If the 'table' or 'inline-table' element has 'width: auto', the used width is the greater of the table's containing block width,
				// CAPMIN, and MIN. However, if either CAPMIN or the maximum width required by the columns plus cell spacing or borders (MAX) is
				// less than that of the containing block, use max(MAX, CAPMIN).
				int table_width = 0;
				int min_table_width = 0;
				int max_table_width = 0;

				if( !block_width.is_default() ) {
					table_width = m_table_infos->m_grid->calc_table_width( block_width - table_width_spacing, false, min_table_width, max_table_width );
				}
				else {
					table_width = m_table_infos->m_grid->calc_table_width( max_width - table_width_spacing, true, min_table_width, max_table_width );
				}

				min_table_width += table_width_spacing;
				max_table_width += table_width_spacing;
				table_width += table_width_spacing;
				m_table_infos->m_grid->calc_horizontal_positions( m_borders, m_table_infos->m_border_collapse, m_table_infos->m_border_spacing_x );
				bool row_span_found = false;

				// render cells with computed width
				for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
					m_table_infos->m_grid->row( row ).height = 0;

					for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
						table_cell* cell = m_table_infos->m_grid->cell( col, row );

						if( cell->el ) {
							int span_col = col + cell->colspan - 1;

							if( span_col >= m_table_infos->m_grid->cols_count() ) {
								span_col = m_table_infos->m_grid->cols_count() - 1;
							}

							int cell_width = m_table_infos->m_grid->column( span_col ).right - m_table_infos->m_grid->column( col ).left;

							if( cell->el->m_pos.width != cell_width - cell->el->content_margins_left() - cell->el->content_margins_right() ) {
								cell->el->render( m_table_infos->m_grid->column( col ).left, 0, cell_width );
								cell->el->m_pos.width = cell_width - cell->el->content_margins_left() - cell->el->content_margins_right();
							}
							else {
								cell->el->m_pos.x = m_table_infos->m_grid->column( col ).left + cell->el->content_margins_left();
							}

							if( cell->rowspan <= 1 ) {
								m_table_infos->m_grid->row( row ).height = std::max( m_table_infos->m_grid->row( row ).height, cell->el->height() );
							}
							else {
								row_span_found = true;
							}
						}
					}
				}

				if( row_span_found ) {
					for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
						for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
							table_cell* cell = m_table_infos->m_grid->cell( col, row );

							if( cell->el ) {
								int span_row = row + cell->rowspan - 1;

								if( span_row >= m_table_infos->m_grid->rows_count() ) {
									span_row = m_table_infos->m_grid->rows_count() - 1;
								}

								if( span_row != row ) {
									int h = 0;

									for( int i = row; i <= span_row; i++ ) {
										h += m_table_infos->m_grid->row( i ).height;
									}

									if( h < cell->el->height() ) {
										m_table_infos->m_grid->row( span_row ).height += cell->el->height() - h;
									}
								}
							}
						}
					}
				}

				// Calculate vertical table spacing
				int table_height_spacing = 0;

				if( m_table_infos->m_border_collapse == border_collapse_separate ) {
					table_height_spacing = m_table_infos->m_border_spacing_y * ( m_table_infos->m_grid->rows_count() + 1 );
				}
				else {
					table_height_spacing = 0;

					if( m_table_infos->m_grid->rows_count() ) {
						table_height_spacing -= std::min( border_top(), m_table_infos->m_grid->row( 0 ).border_top );
						table_height_spacing -= std::min( border_bottom(), m_table_infos->m_grid->row( m_table_infos->m_grid->rows_count() - 1 ).border_bottom );
					}

					for( int row = 1; row < m_table_infos->m_grid->rows_count(); row++ ) {
						table_height_spacing -= std::min( m_table_infos->m_grid->row( row ).border_top, m_table_infos->m_grid->row( row - 1 ).border_bottom );
					}
				}

				// calculate block height
				int block_height = 0;

				if( get_predefined_height( block_height ) ) {
					block_height -= m_padding.height() + m_borders.height();
				}

				// calculate minimum height from m_css_min_height
				int min_height = 0;

				if( !m_css_min_height.is_predefined() && m_css_min_height.units() == css_units_percentage ) {
					element* el_parent = parent();

					if( el_parent ) {
						int parent_height = 0;

						if( el_parent->get_predefined_height( parent_height ) ) {
							min_height = m_css_min_height.calc_percent( parent_height );
						}
					}
				}
				else {
					min_height = ( int )m_css_min_height.val();
				}

				//<eco: not used int extra_row_height = 0;
				int minimum_table_height = std::max( block_height, min_height );
				m_table_infos->m_grid->calc_rows_height( minimum_table_height - table_height_spacing, m_table_infos->m_border_spacing_y );
				m_table_infos->m_grid->calc_vertical_positions( m_borders, m_table_infos->m_border_collapse, m_table_infos->m_border_spacing_y );
				int table_height = 0;

				// place cells vertically
				for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
					for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
						table_cell* cell = m_table_infos->m_grid->cell( col, row );

						if( cell->el ) {
							int span_row = row + cell->rowspan - 1;

							if( span_row >= m_table_infos->m_grid->rows_count() ) {
								span_row = m_table_infos->m_grid->rows_count() - 1;
							}

							cell->el->m_pos.y = m_table_infos->m_grid->row( row ).top + cell->el->content_margins_top();
							cell->el->m_pos.height = m_table_infos->m_grid->row( span_row ).bottom - m_table_infos->m_grid->row( row ).top - cell->el->content_margins_top() - cell->el->content_margins_bottom();
							table_height = std::max( table_height, m_table_infos->m_grid->row( span_row ).bottom );
							cell->el->apply_vertical_align();
						}
					}
				}

				if( m_table_infos->m_border_collapse == border_collapse_collapse ) {
					if( m_table_infos->m_grid->rows_count() ) {
						table_height -= std::min( border_bottom(), m_table_infos->m_grid->row( m_table_infos->m_grid->rows_count() - 1 ).border_bottom );
					}
				}
				else {
					table_height += m_table_infos->m_border_spacing_y;
				}

				m_pos.width = table_width;
				calc_auto_margins( parent_width );
				m_pos.move_to( x, y );
				m_pos.x += content_margins_left();
				m_pos.y += content_margins_top();
				m_pos.width = table_width;
				m_pos.height = table_height;
				return max_table_width;
			}
			void html_tag::draw_children_box( uint_ptr hdc, int x, int y, const position * clip, draw_flag flag, int zindex ) {
				position pos = m_pos;
				pos.x += x;
				pos.y += y;
				document::ptr doc = get_document();

				if( m_overflow > overflow_visible ) {
					position border_box = pos;
					border_box += m_padding;
					border_box += m_borders;
					border_radiuses bdr_radius = m_css_borders.radius.calc_percents( border_box.width, border_box.height );
					bdr_radius -= m_borders;
					bdr_radius -= m_padding;
					doc->container()->set_clip( pos, bdr_radius, true, true );
				}

				position browser_wnd;
				doc->container()->get_client_rect( browser_wnd );
				element* el;

				for( auto& item : m_children ) {
					el = item;

					if( el->is_visible() ) {
						switch( flag ) {
							case draw_positioned:
								if( el->is_positioned() && el->get_zindex() == zindex ) {
									if( el->get_element_position() == element_position_fixed ) {
										el->draw( hdc, browser_wnd.x, browser_wnd.y, clip );
										el->draw_stacking_context( hdc, browser_wnd.x, browser_wnd.y, clip, true );
									}
									else {
										el->draw( hdc, pos.x, pos.y, clip );
										el->draw_stacking_context( hdc, pos.x, pos.y, clip, true );
									}

									el = 0;
								}

								break;

							case draw_block:
								if( !el->is_inline_box() && el->get_float() == float_none && !el->is_positioned() ) {
									el->draw( hdc, pos.x, pos.y, clip );
								}

								break;

							case draw_floats:
								if( el->get_float() != float_none && !el->is_positioned() ) {
									el->draw( hdc, pos.x, pos.y, clip );
									el->draw_stacking_context( hdc, pos.x, pos.y, clip, false );
									el = 0;
								}

								break;

							case draw_inlines:
								if( el->is_inline_box() && el->get_float() == float_none && !el->is_positioned() ) {
									el->draw( hdc, pos.x, pos.y, clip );

									if( el->get_display() == display_inline_block ) {
										el->draw_stacking_context( hdc, pos.x, pos.y, clip, false );
										el = 0;
									}
								}

								break;

							default:
								break;
						}

						if( el ) {
							if( flag == draw_positioned ) {
								if( !el->is_positioned() ) {
									el->draw_children( hdc, pos.x, pos.y, clip, flag, zindex );
								}
							}
							else {
								if( el->get_float() == float_none &&
										el->get_display() != display_inline_block &&
										!el->is_positioned() ) {
									el->draw_children( hdc, pos.x, pos.y, clip, flag, zindex );
								}
							}
						}
					}
				}

				if( m_overflow > overflow_visible ) {
					doc->container()->del_clip();
				}
			}
			void html_tag::draw_children_table( uint_ptr hdc, int x, int y, const position * clip, draw_flag flag, int zindex ) {
				if( !m_table_infos || !m_table_infos->m_grid ) {
					return;
				}

				position pos = m_pos;
				pos.x += x;
				pos.y += y;

				for( int row = 0; row < m_table_infos->m_grid->rows_count(); row++ ) {
					if( flag == draw_block ) {
						m_table_infos->m_grid->row( row ).el_row->draw_background( hdc, pos.x, pos.y, clip );
					}

					for( int col = 0; col < m_table_infos->m_grid->cols_count(); col++ ) {
						table_cell* cell = m_table_infos->m_grid->cell( col, row );

						if( cell->el ) {
							if( flag == draw_block ) {
								cell->el->draw( hdc, pos.x, pos.y, clip );
							}

							cell->el->draw_children( hdc, pos.x, pos.y, clip, flag, zindex );
						}
					}
				}
			}
			elements_vector html_tag::select_all( const tstring & selector ) {
				css_selector sel( media_query_list::ptr( 0 ) );
				sel.parse( selector );
				return select_all( sel );
			}
			elements_vector html_tag::select_all( const css_selector & selector ) {
				elements_vector res;
				select_all( selector, res );
				return res;
			}
			void html_tag::select_all( const css_selector & selector, elements_vector & res ) {
				if( select( selector ) ) {
					res.push_back( this );
				}

				for( auto& el : m_children ) {
					el->select_all( selector, res );
				}
			}
			element* html_tag::select_one( const tstring & selector ) {
				css_selector sel( media_query_list::ptr( 0 ) );
				sel.parse( selector );
				return select_one( sel );
			}
			element* html_tag::select_one( const css_selector & selector ) {
				if( select( selector ) ) {
					return this;
				}

				for( element* el = m_children.first; el; el = el->m_next_element ) {
					element* res = el->select_one( selector );

					if( res ) {
						return res;
					}
				}

				return 0;
			}
			void html_tag::apply_stylesheet( const css & stylesheet ) {
				remove_before_after();

				for( const auto& sel : stylesheet.selectors() ) {
					int apply = select( *sel, false );

					if( apply != select_no_match ) {
						used_selector::ptr us = std::unique_ptr<used_selector>( new used_selector( sel, false ) );

						if( sel->is_media_valid() ) {
							if( apply & select_match_pseudo_class ) {
								if( select( *sel, true ) ) {
									if( apply & select_match_with_after ) {
										element* el = get_element_after();

										if( el ) {
											el->add_style( *sel->m_style );
										}
									}
									else if( apply & select_match_with_before ) {
										element* el = get_element_before();

										if( el ) {
											el->add_style( *sel->m_style );
										}
									}
									else {
										add_style( *sel->m_style );
										us->m_used = true;
									}
								}
							}
							else if( apply & select_match_with_after ) {
								element* el = get_element_after();

								if( el ) {
									el->add_style( *sel->m_style );
								}
							}
							else if( apply & select_match_with_before ) {
								element* el = get_element_before();

								if( el ) {
									el->add_style( *sel->m_style );
								}
							}
							else {
								add_style( *sel->m_style );
								us->m_used = true;
							}
						}

						m_used_styles.push_back( std::move( us ) );
					}
				}

				for( auto& el : m_children ) {
					if( el->get_display() != style_display_inline_text ) {
						el->apply_stylesheet( stylesheet );
					}
				}
			}
			void html_tag::draw( uint_ptr hdc, int x, int y, const position * clip ) {
				position pos = m_pos;
				pos.x	+= x;
				pos.y	+= y;
				draw_background( hdc, x, y, clip );

				if( m_display == style_display_list_item && m_list_style_type != list_style_type_none ) {
					if( m_overflow > overflow_visible ) {
						position border_box = pos;
						border_box += m_padding;
						border_box += m_borders;
						border_radiuses bdr_radius = m_css_borders.radius.calc_percents( border_box.width, border_box.height );
						bdr_radius -= m_borders;
						bdr_radius -= m_padding;
						get_document()->container()->set_clip( pos, bdr_radius, true, true );
					}

					draw_list_marker( hdc, pos );

					if( m_overflow > overflow_visible ) {
						get_document()->container()->del_clip();
					}
				}
			}
		}