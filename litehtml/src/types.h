#pragma once

#include <stdlib.h>
#include <memory>
#include <map>
#include <vector>

#include "xvector.h"
#include "xstring.h"
#include "_keywords.h"

namespace litehtml
{
	#define		countof( x )		((sizeof(x))	/ (sizeof((x)[0])))

	/**
	 * @brief The refcnt class
	 */

	class refcnt {
		int32_t	__cnt;

	public:
		refcnt( )
			: __cnt( 0 )
		{
		}

		virtual ~refcnt( ) {
		}

		void	addref( ) {
			__cnt++;
		}

		void	release( ) {
			if( __cnt==0 )
				__asm int 3;

			if( --__cnt==0 ) {
				delete this;
			}
		}
	};

	

	class document;
	class element;

	enum atom;

	//typedef std::map<litehtml::tstring, litehtml::tstring>			string_map;
	//typedef std::map<litehtml::atom, litehtml::tstring>				attr_map;
	//typedef std::vector< std::shared_ptr<litehtml::element> >		elements_vector;
	//typedef std::vector<int>										int_vector;
	//typedef std::vector<litehtml::tstring>							string_vector;

	const unsigned int font_decoration_none			= 0x00;
	const unsigned int font_decoration_underline	= 0x01;
	const unsigned int font_decoration_linethrough	= 0x02;
	const unsigned int font_decoration_overline		= 0x04;

	typedef unsigned char	byte;
	typedef unsigned int	ucode_t;

	class 	attribute {
	public:
		atom		m_name;
		xstring		m_value;

		attribute*	m_m_next;
	};

	struct margins
	{
		int	left;
		int	right;
		int top;
		int bottom;

		margins()
		{
			left = right = top = bottom = 0;
		}

		int width()		const	{ return left + right; } 
		int height()	const	{ return top + bottom; } 
	};

	struct size
	{
		int		width;
		int		height;

		size()
		{
			width	= 0;
			height	= 0;
		}
	};

	struct position
	{
		int		x;
		int		y;
		int		width;
		int		height;

		position()
		{
			x = y = width = height = 0;
		}

		position(int x, int y, int width, int height)
		{
			this->x			= x;
			this->y			= y;
			this->width		= width;
			this->height	= height;
		}

		int right()		const		{ return x + width;		}
		int bottom()	const		{ return y + height;	}
		int left()		const		{ return x;				}
		int top()		const		{ return y;				}

		void operator+=(const margins& mg)
		{
			x		-= mg.left;
			y		-= mg.top;
			width	+= mg.left + mg.right;
			height	+= mg.top + mg.bottom;
		}

		void operator-=(const margins& mg)
		{
			x		+= mg.left;
			y		+= mg.top;
			width	-= mg.left + mg.right;
			height	-= mg.top + mg.bottom;
		}

		void clear()
		{
			x = y = width = height = 0;
		}

		void operator=(const size& sz)
		{
			width	= sz.width;
			height	= sz.height;
		}

		void move_to(int x, int y)
		{
			this->x = x;
			this->y = y;
		}

		bool does_intersect(const position* val) const
		{
			if(!val) return true;

			return (
				left()			<= val->right()		&& 
				right()			>= val->left()		&& 
				bottom()		>= val->top()		&& 
				top()			<= val->bottom()	)
				|| (
				val->left()		<= right()			&& 
				val->right()	>= left()			&& 
				val->bottom()	>= top()			&& 
				val->top()		<= bottom()			);
		}

		bool empty() const
		{
			if(!width && !height)
			{
				return true;
			}
			return false;
		}

		bool is_point_inside(int x, int y) const
		{
			if(x >= left() && x <= right() && y >= top() && y <= bottom())
			{
				return true;
			}
			return false;
		}
	};

	struct font_metrics
	{
		int		height;
		int		ascent;
		int		descent;
		int		x_height;
		bool	draw_spaces;

		font_metrics()
		{
			height			= 0;
			ascent			= 0;
			descent			= 0;
			x_height		= 0;
			draw_spaces		= true;
		}
		int base_line()	{ return descent; }
	};

	struct font_item
	{
		uint_ptr		font;
		font_metrics	metrics;
		font_item*		next;
	};

	template <class type>
	struct 	xList
	{
		type*	first;
		type*	last;

		xList( ) {
			first = last = NULL;
		}

		bool empty( ) const {
			return m_first==NULL;
		}
	};

	//typedef std::map<tstring, font_item>	fonts_map;

	enum draw_flag
	{
		draw_root,
		draw_block,
		draw_floats,
		draw_inlines,
		draw_positioned,
	};

	struct floated_box
	{
		//typedef std::vector<floated_box>	vector;

		position		pos;
		element_float	float_side;
		element_clear	clear_floats;
		element*		el;

		floated_box*	next;

		floated_box();
		
		floated_box( const floated_box& val )
		{
			pos = val.pos;
			float_side = val.float_side;
			clear_floats = val.clear_floats;
			el = val.el;
		}

		floated_box& operator=(const floated_box& val)
		{
			pos = val.pos;
			float_side = val.float_side;
			clear_floats = val.clear_floats;
			el = val.el;
			return *this;
		}
		
		floated_box(floated_box&& val)
		{
			pos = val.pos;
			float_side = val.float_side;
			clear_floats = val.clear_floats;
			el = val.el;
		}
		
		void operator=(floated_box&& val)
		{
			pos = val.pos;
			float_side = val.float_side;
			clear_floats = val.clear_floats;
			el = val.el;
		}
	};

	struct int_int_cache
	{
		int		hash;
		int		val;
		bool	is_valid;
		bool	is_default;

		int_int_cache()
		{
			hash		= 0;
			val			= 0;
			is_valid	= false;
			is_default	= false;
		}
		void invalidate()
		{
			is_valid	= false;
			is_default	= false;
		}
		void set_value(int vHash, int vVal)
		{
			hash		= vHash;
			val			= vVal;
			is_valid	= true;
		}
	};

	enum select_result
	{
		select_no_match				= 0x00,
		select_match				= 0x01,
		select_match_pseudo_class	= 0x02,
		select_match_with_before	= 0x10,
		select_match_with_after		= 0x20,
	};

	template<class T>
	class def_value
	{
		T		m_val;
		bool	m_is_default;
	public:
		def_value(T def_val)
		{
			m_is_default	= true;
			m_val			= def_val;
		}
		void reset(T def_val)
		{
			m_is_default	= true;
			m_val			= def_val;
		}
		bool is_default()
		{
			return m_is_default;
		}
		T operator=(T new_val)
		{
			m_val			= new_val;
			m_is_default	= false;
			return m_val;
		}
		operator T()
		{
			return m_val;
		}
	};

/*
//#define media_orientation_strings		_t("portrait;landscape")
	#define media_orientation_atoms		atom_portrait,atom_landscape,0

	enum media_orientation
	{
		media_orientation_portrait,
		media_orientation_landscape,
	};

//#define media_feature_strings		_t("none;width;min-width;max-width;height;min-height;max-height;device-width;min-device-width;max-device-width;device-height;min-device-height;max-device-height;orientation;aspect-ratio;min-aspect-ratio;max-aspect-ratio;device-aspect-ratio;min-device-aspect-ratio;max-device-aspect-ratio;color;min-color;max-color;color-index;min-color-index;max-color-index;monochrome;min-monochrome;max-monochrome;resolution;min-resolution;max-resolution")
	#define media_feature_strings		atom_none,atom_width,atom_min_width,atom_max_width,atom_height,atom_min_height,atom_max_height,atom_device_width,atom_min_device_width,atom_max_device_width,\
										atom_device_height,atom_min_device_height,atom_max_device_height,atom_orientation,atom_aspect_ratio,atom_min_aspect_ratio,atom_max_aspect_ratio,atom_device_aspect_ratio,\
										atom_min_device_aspect_ratio,atom_max_device_aspect_ratio,atom_color,atom_min_color,atom_max_color,atom_color_index,atom_min_color_index,atom_max_color_index,atom_monochrome,\
										atom_min_monochrome,atom_max_monochrome,atom_resolution,atom_min_resolution,atom_max_resolution,0

	enum media_feature
	{
		media_feature_none,

		media_feature_width,
		media_feature_min_width,
		media_feature_max_width,

		media_feature_height,
		media_feature_min_height,
		media_feature_max_height,

		media_feature_device_width,
		media_feature_min_device_width,
		media_feature_max_device_width,

		media_feature_device_height,
		media_feature_min_device_height,
		media_feature_max_device_height,

		media_feature_orientation,

		media_feature_aspect_ratio,
		media_feature_min_aspect_ratio,
		media_feature_max_aspect_ratio,

		media_feature_device_aspect_ratio,
		media_feature_min_device_aspect_ratio,
		media_feature_max_device_aspect_ratio,

		media_feature_color,
		media_feature_min_color,
		media_feature_max_color,

		media_feature_color_index,
		media_feature_min_color_index,
		media_feature_max_color_index,

		media_feature_monochrome,
		media_feature_min_monochrome,
		media_feature_max_monochrome,

		media_feature_resolution,
		media_feature_min_resolution,
		media_feature_max_resolution,
	};

//#define box_sizing_strings		_t("content-box;border-box")
	#define box_sizing_atoms		atom_content_box,atom_border_box,0

	enum box_sizing
	{
		box_sizing_content_box,
		box_sizing_border_box,
	};


//#define media_type_strings		_t("none;all;screen;print;braille;embossed;handheld;projection;speech;tty;tv")
	#define media_type_atoms		atom_none,atom_all,atom_screen,atom_print,atom_braille,atom_embossed,atom_handheld,atom_projection,atom_speech,atom_tty,atom_tv,0

	enum media_type
	{
		media_type_none,
		media_type_all,
		media_type_screen,
		media_type_print,
		media_type_braille,
		media_type_embossed,
		media_type_handheld,
		media_type_projection,
		media_type_speech,
		media_type_tty,
		media_type_tv,
	};
*/

	struct media_features
	{
		media_type	type;
		int			width;			// (pixels) For continuous media, this is the width of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the width of the page box.
		int			height;			// (pixels) The height of the targeted display area of the output device. For continuous media, this is the height of the viewport including the size of a rendered scroll bar (if any). For paged media, this is the height of the page box.
		int			device_width;	// (pixels) The width of the rendering surface of the output device. For continuous media, this is the width of the screen. For paged media, this is the width of the page sheet size.
		int			device_height;	// (pixels) The height of the rendering surface of the output device. For continuous media, this is the height of the screen. For paged media, this is the height of the page sheet size.
		int			color;			// The number of bits per color component of the output device. If the device is not a color device, the value is zero.
		int			color_index;	// The number of entries in the color lookup table of the output device. If the device does not use a color lookup table, the value is zero.
		int			monochrome;		// The number of bits per pixel in a monochrome frame buffer. If the device is not a monochrome device, the output device value will be 0.
		int			resolution;		// The resolution of the output device (in DPI)
	};

	enum render_type
	{
		render_all,
		render_no_fixed,
		render_fixed_only,
	};

	// List of the Void Elements (can't have any contents)
	const litehtml::tchar_t* const void_elements = _t("area;base;br;col;command;embed;hr;img;input;keygen;link;meta;param;source;track;wbr");

}
