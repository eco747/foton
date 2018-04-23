#include "html.h"
#include "box.h"
#include "html_tag.h"

namespace litehtml
{
	//////////////////////////////////////////////////////////////////////////

	box_type line_box::get_type()
	{
		return box_line;
	}

	int line_box::height()
	{
		return m_height;
	}

	int line_box::width()
	{
		return m_width;
	}

	void line_box::add_element( element* el)
	{
		el->skip( false );
		el->m_box	= 0;
		
		bool add	= true;
		if( (m_items.empty() && el->is_white_space()) || el->is_break() ) {
			el->skip( true );
		} 
		else if(el->is_white_space()) {
			if (have_last_space()) {
				add = false;
				el->skip( true );
			}
		}

		if(add) {
			el->m_box = this;
			m_items.add( el );

			if(!el->skip() ) {
				int el_shift_left	= el->get_inline_shift_left();
				int el_shift_right	= el->get_inline_shift_right();

				el->m_pos.x	= m_left + m_width + el_shift_left + el->content_margins_left();
				el->m_pos.y	= m_top + el->content_margins_top();

				m_width	+= el->width() + el_shift_left + el_shift_right;
			}
		}
	}

	void line_box::finish(bool last_box)
	{
		if( is_empty() || (!is_empty() && last_box && is_break_only()) )
		{
			m_height = 0;
			return;
		}

		for( int i=0; i<m_items.length(); i++ ) {
			
			element* el = m_items.at( i );
			if( el->is_white_space() || el->is_break() ) {
				if(!el->skip() ) {
					el->skip( true );
					m_width -= el->width();
				}
			} 
			else {
				break;
			}
		}

		int base_line	= m_font_metrics.base_line();
		int line_height = m_line_height;

		int add_x = 0;
		switch(m_text_align)
		{
		case text_align_right:
			if(m_width < (m_right - m_left)) {
				add_x = (m_right - m_left) - m_width;
			}
			break;

		case text_align_center:
			if(m_width < (m_right - m_left)) {
				add_x = ((m_right - m_left) - m_width) / 2;
			}
			break;

		default:
			add_x = 0;
			break;
		}

		m_height = 0;
		
		// find line box baseline and line-height
		//for(const auto& el : m_items)
		for( int i=0; i<m_items.length(); i++ ) {
			
			element* el = m_items.at(i);
			
			if(el->get_display() == style_display_inline_text) {
				font_metrics fm;
				el->get_font(&fm);
				base_line	= std::max(base_line,	fm.base_line());
				line_height = std::max(line_height, el->line_height());
				m_height = std::max(m_height, fm.height);
			}
			
			el->m_pos.x += add_x;
		}

		if(m_height) {
			base_line += (line_height - m_height) / 2;
		}

		m_height = line_height;

		int y1	= 0;
		int y2	= m_height;

		//for (const auto& el : m_items) {
		for( int i=0; i<m_items.length(); i++ ) {
			
			element* el = m_items.at(i);
			
			if(el->get_display() == style_display_inline_text) {
				font_metrics fm;
				el->get_font(&fm);
				el->m_pos.y = m_height - base_line - fm.ascent;
			} 
			else {
				switch(el->get_vertical_align())
				{
				case vertical_align_super:
				case vertical_align_sub:
				case vertical_align_baseline:
					el->m_pos.y = m_height - base_line - el->height() + el->get_base_line() + el->content_margins_top();
					break;

				case vertical_align_top:
					el->m_pos.y = y1 + el->content_margins_top();
					break;

				case vertical_align_text_top:
					el->m_pos.y = m_height - base_line - m_font_metrics.ascent + el->content_margins_top();
					break;

				case vertical_align_middle:
					el->m_pos.y = m_height - base_line - m_font_metrics.x_height / 2 - el->height() / 2 + el->content_margins_top();
					break;

				case vertical_align_bottom:
					el->m_pos.y = y2 - el->height() + el->content_margins_top();
					break;

				case vertical_align_text_bottom:
					el->m_pos.y = m_height - base_line + m_font_metrics.descent - el->height() + el->content_margins_top();
					break;
				}

				y1 = std::min(y1, el->top());
				y2 = std::max(y2, el->bottom());
			}
		}

		css_offsets offsets;

		//for (const auto& el : m_items) {
		for( int i=0; i<m_items.length(); i++ ) {
			element* el = m_items.at(i);

			el->m_pos.y -= y1;
			el->m_pos.y += m_top;
			if(el->get_display() != style_display_inline_text) {
				switch(el->get_vertical_align())
				{
				case vertical_align_top:
					el->m_pos.y = m_top + el->content_margins_top();
					break;

				case vertical_align_bottom:
					el->m_pos.y = m_top + (y2 - y1) - el->height() + el->content_margins_top();
					break;

				case vertical_align_baseline:
					//TODO: process vertical align "baseline"
					break;

				case vertical_align_middle:
					//TODO: process vertical align "middle"
					break;

				case vertical_align_sub:
					//TODO: process vertical align "sub"
					break;

				case vertical_align_super:
					//TODO: process vertical align "super"
					break;

				case vertical_align_text_bottom:
					//TODO: process vertical align "text-bottom"
					break;

				case vertical_align_text_top:
					//TODO: process vertical align "text-top"
					break;

				}
			}

			el->apply_relative_shift(m_right - m_left);
		}
		
		m_height = y2 - y1;
		m_baseline = (base_line - y1) - (m_height - line_height);
	}

	bool line_box::can_hold( element* el, white_space ws)
	{
		if(!el->is_inline_box()) {
			return false;
		}

		if(el->is_break()) {
			return false;
		}

		if(ws == white_space_nowrap || ws == white_space_pre) {
			return true;
		}

		if(m_left + m_width + el->width() + el->get_inline_shift_left() + el->get_inline_shift_right() > m_right) {
			return false;
		}

		return true;
	}

	bool line_box::have_last_space()
	{
		element* el = m_items.last();
		return el && (el->is_white_space() || el->is_break());
		
		/*
		bool ret = false;
		//for (auto i = m_items.rbegin(); i != m_items.rend() && !ret; i++)
		for( int i=m_items.length() && !ret; i++ ) {
			m_items.at(i);
			if(el->is_white_space() || el->is_break()) {
				ret = true;
			} 
			else {
				break;
			}
		}
		
		return ret;
		*/
	}

	bool line_box::is_empty()
	{
		if(m_items.empty()) {
			return true;
		}
		
		for( int i=m_items.length()-1; i>=0; i++ ) {
			element* el = m_items.at(i);
			if(!el->skip() || el->is_break()) {
				return false;
			}
		}
		
		return true;
	}

	int line_box::baseline()
	{
		return m_baseline;
	}

	void line_box::get_elements( xVector<element>& els )
	{
		els.copy( m_items );
	}

	int line_box::top_margin()
	{
		return 0;
	}

	int line_box::bottom_margin()
	{
		return 0;
	}

	void line_box::y_shift( int shift )
	{
		m_top += shift;
		
		for( int i=0; i<m_items.length(); i++ ) {
			element* el = m_items.at(i);
			el->m_pos.y += shift;
		}
	}

	bool line_box::is_break_only()
	{
		if(m_items.empty()) {
			return true;
		}

		if( m_items.first()->is_break() ) {
			
			//for (auto& el : m_items) {
			for( int i=0; i<m_items.length(); i++ ) {
				element* el = m_items.at(i);
				if(!el->skip() ) {
					return false;
				}
			}
			
			return true;
		}

		return false;
	}

	void line_box::new_width( int left, int right, xVector<element>& els )
	{
		int add = left - m_left;
		if(add) {
			m_left	= left;
			m_right	= right;
			m_width = 0;

			auto remove_begin = m_items.end();
			for (auto i = m_items.begin() + 1; i != m_items.end(); i++)
			{
				element* el = (*i);

				if(!el->skip())
				{
					if(m_left + m_width + el->width() + el->get_inline_shift_right() + el->get_inline_shift_left() > m_right) {
						remove_begin = i;
						break;
					} 
					else {
						el->m_pos.x += add;
						m_width += el->width() + el->get_inline_shift_right() + el->get_inline_shift_left();
					}
				}
			}
			if(remove_begin != m_items.end())
			{
				els.insert(els.begin(), remove_begin, m_items.end());
				m_items.erase(remove_begin, m_items.end());

				//for(const auto& el : els) {
				for( int i=0; i<m_items.length(); i++ ) {
					element* el = m_items.at(i);
					el->m_box = 0;
				}
			}
		}
	}

}