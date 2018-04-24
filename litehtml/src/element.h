#pragma once
#include <memory>
#include "stylesheet.h"
#include "css_offsets.h"

namespace litehtml
{
	class box;


	class element
	{
		friend class block_box;
		friend class line_box;
		friend class html_tag;
		friend class el_table;
		friend class document;

	protected:
		const uint32_t	flag_skip = 0x0001;

		document*			m_doc;
		box*				m_box;
		position			m_pos;
		margins				m_margins;
		margins				m_padding;
		margins				m_borders;
		//bool				m_skip;
		uint32_t 			m_flags;	//
				
		//xVector<element>	m_children;
		
		element*			m_parent;
		element*			m_next_element;
		//element*			m_prev_element;
		element*			m_next_positionned;

		xList<element>		m_children;
		//element*			m_first_child;
		//element*			m_last_child;

		virtual void select_all( const css_selector& selector, xVector<element*>& res );

	public:
		element( document* doc);
		virtual ~element();

		// returns refer to m_pos member;
		position&					get_position();

		int							left()						const;
		int							right()						const;
		int							top()						const;
		int							bottom()					const;
		int							height()					const;
		int							width()						const;

		int							content_margins_top()		const;
		int							content_margins_bottom()	const;
		int							content_margins_left()		const;
		int							content_margins_right()		const;
		int							content_margins_width()		const;
		int							content_margins_height()	const;

		int							margin_top()				const;
		int							margin_bottom()				const;
		int							margin_left()				const;
		int							margin_right()				const;
		margins						get_margins()				const;

		int							padding_top()				const;
		int							padding_bottom()			const;
		int							padding_left()				const;
		int							padding_right()				const;
		margins						get_paddings()				const;

		int							border_top()				const;
		int							border_bottom()				const;
		int							border_left()				const;
		int							border_right()				const;
		margins						get_borders()				const;

		bool						in_normal_flow()			const;
		web_color					get_color(atom prop_name, bool inherited, const web_color& def_color = web_color());
		bool						is_inline_box()				const;
		position					get_placement()				const;
		bool						collapse_top_margin()		const;
		bool						collapse_bottom_margin()	const;
		bool						is_positioned()				const;

		bool						skip() const;
		void						skip(bool val);
		bool						have_parent() const;
		element*					parent() const;
		void						set_parent(element* par);
		bool						is_visible() const;
		int							calc_width(int defVal) const;
		int							get_inline_shift_left();
		int							get_inline_shift_right();
		void						apply_relative_shift(int parent_width);

		document*					get_document() const;

		virtual elements_vector		select_all(const tstring& selector);
		virtual elements_vector		select_all(const css_selector& selector);

		virtual element*			select_one(const tstring& selector);
		virtual element*			select_one(const css_selector& selector);

		virtual int					render(int x, int y, int max_width, bool second_pass = false);
		virtual int					render_inline(const element* container, int max_width);
		virtual int					place_element(const element* el, int max_width);
		virtual void				calc_outlines( int parent_width );
		virtual void				calc_auto_margins(int parent_width);
		virtual void				apply_vertical_align();
		virtual bool				fetch_positioned();
		virtual void				render_positioned(render_type rt = render_all);

		virtual bool				appendChild( element* el);
		virtual bool				removeChild( element* el);
		virtual void				clearRecursive() ;

		virtual atom				get_tagName() const;
		virtual void				set_tagName( atom /*const tchar_t* */ tag);
		virtual void				set_data(const tchar_t* data);
		virtual element_float		get_float() const;
		virtual vertical_align		get_vertical_align() const;
		virtual element_clear		get_clear() const;
		virtual size_t				get_children_count() const;
		virtual element*			get_child(int idx) const;
		virtual overflow			get_overflow() const;

		virtual css_length			get_css_left() const;
		virtual css_length			get_css_right() const;
		virtual css_length			get_css_top() const;
		virtual css_length			get_css_bottom() const;
		virtual css_offsets			get_css_offsets() const;
		virtual css_length			get_css_width() const;
		virtual void				set_css_width(css_length& w);
		virtual css_length			get_css_height() const;

		virtual void				set_attr( atom name, const tchar_t* val);
		virtual const tchar_t*		get_attr( atom name, const tchar_t* def = 0 );

		virtual void				apply_stylesheet(const css& stylesheet);
		virtual void				refresh_styles();
		virtual bool				is_white_space() const;
		virtual bool				is_body() const;
		virtual bool				is_break() const;
		virtual int					get_base_line();
		virtual bool				on_mouse_over();
		virtual bool				on_mouse_leave();
		virtual bool				on_lbutton_down();
		virtual bool				on_lbutton_up();
		virtual void				on_click();
		virtual bool				find_styles_changes(xVector<position>& redraw_boxes, int x, int y);
		virtual const tchar_t*		get_cursor();
		virtual void				init_font();
		virtual bool				is_point_inside(int x, int y);
		virtual bool				set_pseudo_class(const tchar_t* pclass, bool add);
		virtual bool				set_class(const tchar_t* pclass, bool add);
		virtual bool				is_replaced() const;
		virtual int					line_height() const;
		virtual white_space			get_white_space() const;
		virtual style_display		get_display() const;
		virtual visibility			get_visibility() const;
		virtual element_position	get_element_position(css_offsets* offsets = 0) const;
		virtual void				get_inline_boxes(xVector<position>& boxes);
		virtual void				parse_styles(bool is_reparse = false);
		virtual void				draw(uint_ptr hdc, int x, int y, const position* clip);
		virtual void				draw_background( uint_ptr hdc, int x, int y, const position* clip );
		virtual const css_value*	get_style_property(atom name, bool inherited);
		virtual uint_ptr			get_font(font_metrics* fm = 0);
		virtual int					get_font_size() const;
		virtual void				get_text(tstring& text);
		virtual void				parse_attributes();
		virtual int					select(const css_selector& selector, bool apply_pseudo = true);
		virtual int					select(const css_element_selector& selector, bool apply_pseudo = true);
		virtual element*			find_ancestor(const css_selector& selector, bool apply_pseudo = true, bool* is_pseudo = 0);
		virtual bool				is_ancestor(const element* el) const;
		virtual element*			find_adjacent_sibling(const element* el, const css_selector& selector, bool apply_pseudo = true, bool* is_pseudo = 0);
		virtual element*			find_sibling(const element*  el, const css_selector& selector, bool apply_pseudo = true, bool* is_pseudo = 0);
		virtual bool				is_first_child_inline(const element* el) const;
		virtual bool				is_last_child_inline(const element* el);
		virtual bool				have_inline_child() const;
		virtual void				get_content_size(size& sz, int max_width);
		virtual void				init();
		virtual bool				is_floats_holder() const;
		virtual int					get_floats_height(element_float el_float = element_float_none) const;
		virtual int					get_left_floats_height() const;
		virtual int					get_right_floats_height() const;
		virtual int					get_line_left(int y);
		virtual int					get_line_right(int y, int def_right);
		virtual void				get_line_left_right(int y, int def_right, int& ln_left, int& ln_right);
		virtual void				add_float(const element* el, int x, int y);
		virtual void				update_floats(int dy, const element* parent);
		virtual void				add_positioned(const element* el);
		virtual int					find_next_line_top(int top, int width, int def_right);
		virtual int					get_zindex() const;
		virtual void				draw_stacking_context(uint_ptr hdc, int x, int y, const position* clip, bool with_positioned);
		virtual void				draw_children( uint_ptr hdc, int x, int y, const position* clip, draw_flag flag, int zindex );
		virtual bool				is_nth_child(const element* el, int num, int off, bool of_type) const;
		virtual bool				is_nth_last_child(const element* el, int num, int off, bool of_type) const;
		virtual bool				is_only_child(const element* el, bool of_type) const;
		virtual bool				get_predefined_height(int& p_height) const;
		virtual void				calc_document_size(size& sz, int x = 0, int y = 0);
		virtual void				get_redraw_box(position& pos, int x = 0, int y = 0);
		virtual void				add_style(const style& st);
		virtual element*			get_element_by_point(int x, int y, int client_x, int client_y);
		virtual element*			get_child_by_point(int x, int y, int client_x, int client_y, draw_flag flag, int zindex);
		virtual const background*	get_background(bool own_only = false);
	};

	//////////////////////////////////////////////////////////////////////////
	//							INLINE FUNCTIONS							//
	//////////////////////////////////////////////////////////////////////////

	inline int element::right() const
	{
		return left() + width();
	}

	inline int element::left() const
	{
		return m_pos.left() - margin_left() - m_padding.left - m_borders.left;
	}

	inline int element::top() const
	{
		return m_pos.top() - margin_top() - m_padding.top - m_borders.top;
	}

	inline int element::bottom() const
	{
		return top() + height();
	}

	inline int element::height() const
	{
		return m_pos.height + margin_top() + margin_bottom() + m_padding.height() + m_borders.height();
	}

	inline int element::width() const
	{
		return m_pos.width + margin_left() + margin_right() + m_padding.width() + m_borders.width();
	}

	inline int element::content_margins_top() const
	{
		return margin_top() + m_padding.top + m_borders.top;
	}

	inline int element::content_margins_bottom() const
	{
		return margin_bottom() + m_padding.bottom + m_borders.bottom;
	}

	inline int element::content_margins_left() const
	{
		return margin_left() + m_padding.left + m_borders.left;
	}

	inline int element::content_margins_right() const
	{
		return margin_right() + m_padding.right + m_borders.right;
	}

	inline int element::content_margins_width() const
	{
		return content_margins_left() + content_margins_right();
	}

	inline int element::content_margins_height() const
	{
		return content_margins_top() + content_margins_bottom();
	}

	inline margins element::get_paddings()	const
	{
		return m_padding;
	}

	inline margins element::get_borders()	const
	{
		return m_borders;
	}

	inline int element::padding_top() const
	{
		return m_padding.top;
	}

	inline int element::padding_bottom() const
	{
		return m_padding.bottom;
	}

	inline int element::padding_left() const
	{
		return m_padding.left;
	}

	inline int element::padding_right() const
	{
		return m_padding.right;
	}

	inline bool element::in_normal_flow() const
	{
		if(get_element_position() != element_position_absolute && get_display() != style_display_none)
		{
			return true;
		}
		return false;
	}

	inline int element::border_top() const
	{
		return m_borders.top;
	}

	inline int element::border_bottom() const
	{
		return m_borders.bottom;
	}

	inline int element::border_left() const
	{
		return m_borders.left;
	}

	inline int element::border_right() const
	{
		return m_borders.right;
	}

	inline bool element::skip() const
	{
		return m_flags&flag_skip ? true : false;
	}

	inline void element::skip(bool val)
	{
		//m_skip = val;
		if( val ) m_flags |= flag_skip;
		else	  m_flags &= ~flag_skip;
	}

	inline bool element::have_parent() const
	{
		return m_parent!=NULL;
	}

	inline element* element::parent() const
	{
		return m_parent;
	}

	inline void element::set_parent(element* par)
	{
		m_parent = par;
	}

	inline int element::margin_top() const
	{
		return m_margins.top;
	}

	inline int element::margin_bottom() const
	{
		return m_margins.bottom;
	}

	inline int element::margin_left() const
	{
		return m_margins.left;
	}

	inline int element::margin_right() const
	{
		return m_margins.right;
	}

	inline margins element::get_margins() const
	{
		margins ret;
		ret.left	= margin_left();
		ret.right	= margin_right();
		ret.top		= margin_top();
		ret.bottom	= margin_bottom();

		return ret;
	}

	inline bool element::is_positioned()	const
	{
		return (get_element_position() > element_position_static);
	}

	inline bool element::is_visible() const
	{
		return !(skip() || get_display() == style_display_none || get_visibility() != visibility_visible);
	}

	inline position& element::get_position()
	{
		return m_pos;
	}

	inline document* element::get_document() const
	{
		return m_doc;
	}
}
