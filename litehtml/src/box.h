#pragma once

namespace litehtml
{
	class html_tag;

	enum box_type
	{
		box_block,
		box_line
	};

	class box
	{
//	public:
//		typedef std::unique_ptr<box>	ptr;
//		typedef std::vector< box::ptr >			vector;

	protected:
		int		m_top;
		int		m_left;
		int		m_right;

	public:
		box(int top, int left, int right)
		{
			m_top	= top;
			m_left	= left;
			m_right	= right;
		}

		virtual ~box() 
		{
		}

		int		bottom()	{ return m_top + height();	}
		int		top()		{ return m_top;				}
		int		right()		{ return m_left + width();	}
		int		left()		{ return m_left;			}

		virtual box_type			get_type() = 0;
		virtual int					height() = 0;
		virtual int					width() = 0;
		virtual void				add_element( element* el) = 0;
		virtual bool				can_hold( element* el, white_space ws) = 0;
		virtual void				finish(bool last_box = false) = 0;
		virtual bool				is_empty() = 0;
		virtual int					baseline() = 0;
		virtual void				get_elements( xVector<element>& els) = 0;
		virtual int					top_margin() = 0;
		virtual int					bottom_margin() = 0;
		virtual void				y_shift(int shift) = 0;
		virtual void				new_width(int left, int right, xVector<element>& els) = 0;
	};

	//////////////////////////////////////////////////////////////////////////

	class block_box
		: public box
	{
		element*	 m_element;

	public:
		block_box(int top, int left, int right) 
			: box(top, left, right)
		{
			m_element = NULL;
		}

		virtual box_type	get_type()
		{
			return box_block;
		}

		virtual int		height()
		{
			return m_element->height();
		}

		virtual int		width()
		{
			return m_element->width();
		}

		virtual void	add_element( element* el)
		{
			m_element = el;
			el->m_box = this;
		}

		virtual bool	can_hold( element* el, white_space ws)
		{
			if(m_element || el->is_inline_box()) {
				return false;
			}

			return true;
		}
		
		virtual void	finish(bool last_box = false)
		{
			if(!m_element) return;
			m_element->apply_relative_shift(m_right - m_left);
		}

		virtual bool	is_empty()
		{
			if(m_element) {
				return false;
			}

			return true;
		}

		virtual int		baseline()
		{
			if(m_element) {
				return m_element->get_base_line();
			}
			return 0;
		}

		virtual void	get_elements( xVector<element>& els)
		{
			els.add( m_element );
		}

		virtual int					top_margin()
		{
			if(m_element && m_element->collapse_top_margin()) {
				return m_element->m_margins.top;
			}
			return 0;
		}

		virtual int		bottom_margin()
		{
			if(m_element && m_element->collapse_bottom_margin()) {
				return m_element->m_margins.bottom;
			}
			return 0;
		}

		virtual void	y_shift(int shift)
		{
			m_top += shift;
			if(m_element) {
				m_element->m_pos.y += shift;
			}
		}

		virtual void	new_width(int left, int right, xVector<element>& els)
		{
			
		}
	};

	//////////////////////////////////////////////////////////////////////////

	class line_box
		: public box
	{
		xVector<element>		m_items;
		int						m_height;
		int						m_width;
		int						m_line_height;
		font_metrics			m_font_metrics;
		int						m_baseline;
		text_align				m_text_align;

	public:
		line_box(int top, int left, int right, int line_height, const font_metrics& fm, text_align align)
			: box(top, left, right)
		{
			m_height		= 0;
			m_width			= 0;
			m_font_metrics	= fm;
			m_line_height	= line_height;
			m_baseline		= 0;
			m_text_align	= align;
		}

		virtual box_type			get_type();
		virtual int					height();
		virtual int					width();
		virtual void				add_element( element* el);
		virtual bool				can_hold( element* el, white_space ws);
		virtual void				finish(bool last_box = false);
		virtual bool				is_empty();
		virtual int					baseline();
		virtual void				get_elements( xVector<element>& els);
		virtual int					top_margin();
		virtual int					bottom_margin();
		virtual void				y_shift(int shift);
		virtual void				new_width(int left, int right, xVector<element>& els);

	private:
		bool						have_last_space();
		bool						is_break_only();
	};
}
