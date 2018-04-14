#pragma once

#include <stdlib.h>
#include <memory>
#include <map>
#include <vector>

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

	/**
	 * @brief The xstring class
	 */

	class xstring {
		static const int	BSIZE = 16;

		tchar_t*	__ptr;
		tchar_t		__buf[BSIZE];
		uint32_t	__len;

	public:
		xstring( )
			: __ptr( __buf )
		{
			*__buf = 0;
			__len = 0;
		}

		xstring( const xstring& other ) {
			*__buf = 0;
			__ptr = __buf;
			__len = 0;

			operator = (other);
		}

		xstring( const tchar_t* txt, int32_t len=-1 ) {

			if( len<0 ) {
				len = strlen( txt );
			}

			__len = len;
			__alloc( __len );
			memcpy( __ptr, txt, __len*sizeof(tchar_t) );
			__ptr[__len] = 0;
		}

		~xstring( )
		{
			__release( );
		}

		const tchar_t* c_str() const {
			return __ptr;
		}

		bool	empty( ) const {
			return __len==0;
		}

		xstring& operator = (const xstring& other ) {

			if( &other==this ) {
				__asm int 3;
			}

			__realloc( other.__len );
			__len = other.__len;
			memcpy( __ptr, other.__ptr, (__len+1)*sizeof(tchar_t) );
			return *this;
		}

		xstring& operator = (const tchar_t* txt ) {
			return set( txt );
		}

		xstring& set( const tchar_t* txt, int32_t len=-1, bool lowercase=false )
		{
			if( txt>=__ptr && txt<=__ptr+__len ) {
				__asm int 3;
			}

			if( len<0 ) {
				len = strlen( txt );
			}

			__realloc( len );
			__len	= len;

			if( lewercase==false ) {
				memcpy( __ptr, txt, __len*sizeof(tchar_t) );
			}
			else {

			}

			__ptr[__len] = 0;

			return *this;
		}

		void	clear( ) {
			__release( );
			*__buf	= 0;
			__ptr	= __buf;
			__len	= 0;
		}

		bool match( const xstring& ) {

		}

	private:
		void	__release( ) {
			if( __ptr!=__buf ) {
				free( __ptr );
			}
		}

		void	__alloc( uint32_t len ) {
			len++;
			if( len<BSIZE ) {
				__ptr = __buf;
			}
			else {
				__ptr = (tchar_t*)malloc( adj_size(len) * sizeof(tchar_t) );
			}
		}

		void	__realloc( uint32_t len )
		{
			uint32_t	ol = adj_size( __len+1 );
			uint32_t	nl = adj_size( len+1 );

			if( ol!=nl ) {
				if( __ptr==__buf ) {
					__ptr = (tchar_t*)malloc( nl );
				}
				else {
					if( nl<=BSIZE ) {
						free( __ptr );
						__ptr = __buf;
					}
					else {
						__ptr = (tchar_t*)realloc( __ptr, nl*sizeof(tchar_t) );
					}
				}
			}
		}

		uint32_t adj_size( uint32_t size ) {
			return	((size + BSIZE - 1) / BSIZE) * BSIZE;
		}
	};


	class document;
	class element;

	enum atom;

	typedef std::map<litehtml::tstring, litehtml::tstring>			string_map;
	typedef std::map<litehtml::atom, litehtml::tstring>				attr_map;
	typedef std::vector< std::shared_ptr<litehtml::element> >		elements_vector;
	typedef std::vector<int>										int_vector;
	typedef std::vector<litehtml::tstring>							string_vector;

	const unsigned int font_decoration_none			= 0x00;
	const unsigned int font_decoration_underline	= 0x01;
	const unsigned int font_decoration_linethrough	= 0x02;
	const unsigned int font_decoration_overline		= 0x04;

	typedef unsigned char	byte;
	typedef unsigned int	ucode_t;

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
		typedef std::vector<position>	vector;

		int	x;
		int	y;
		int	width;
		int	height;

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
	};

	typedef std::map<tstring, font_item>	fonts_map;

	enum draw_flag
	{
		draw_root,
		draw_block,
		draw_floats,
		draw_inlines,
		draw_positioned,
	};

//#define  style_display_strings		_t("none;block;inline;inline-block;inline-table;list-item;table;table-caption;table-cell;table-column;table-column-group;table-footer-group;table-header-group;table-row;table-row-group;flex")
#define		style_display_atoms			atom_none,atom_block,atom_inline,atom_inline_block,atom_inline_table,atom_list_item,atom_table,atom_table_caption,atom_table_cell,atom_table_column,atom_table_column_group,atom_table_footer_group,\
										atom_table_header_group,atom_table_row,atom_table_row_group,atom_flex,0

	enum style_display
	{
		display_none = 0,
		display_block,
		display_inline,
		display_inline_block,
		display_inline_table,
		display_list_item,
		display_table,
		display_table_caption,
		display_table_cell,
		display_table_column,
		display_table_column_group,
		display_table_footer_group,
		display_table_header_group,
		display_table_row,
		display_table_row_group,
		display_inline_text,
		display_flex
	};

	enum style_border
	{
		borderNope = 0,
		borderNone,
		borderHidden,
		borderDotted,
		borderDashed,
		borderSolid,
		borderDouble
	};

//#define  font_size_strings		_t("xx-small;x-small;small;medium;large;x-large;xx-large;smaller;larger")
	#define  font_size_atoms		atom_xx_small,atom_x_small,atom_small,atom_medium,atom_large,atom_x_large,atom_xx_large,atom_smaller,atom_larger,0

	enum font_size
	{
		fontSize_xx_small,
		fontSize_x_small,
		fontSize_small,
		fontSize_medium,
		fontSize_large,
		fontSize_x_large,
		fontSize_xx_large,
		fontSize_smaller,
		fontSize_larger,
	};

//#define  font_style_strings		_t("normal;italic")
	#define  font_style_atoms		atom_normal,atom_italic,0

	enum font_style
	{
		fontStyleNormal,
		fontStyleItalic
	};

//#define  font_variant_strings		_t("normal;small-caps")
	#define  font_variant_atoms		atom_normal,atom_small_caps,0

	enum font_variant
	{
		font_variant_normal,
		font_variant_italic
	};

#define  font_weight_strings	_t("normal;bold;bolder;lighter;100;200;300;400;500;600;700")

	enum font_weight
	{
		fontWeightNormal,
		fontWeightBold,
		fontWeightBolder,
		fontWeightLighter,
		fontWeight100,
		fontWeight200,
		fontWeight300,
		fontWeight400,
		fontWeight500,
		fontWeight600,
		fontWeight700
	};

//#define  list_style_type_strings	_t("none;circle;disc;square;armenian;cjk-ideographic;decimal;decimal-leading-zero;georgian;hebrew;hiragana;hiragana-iroha;katakana;katakana-iroha;lower-alpha;lower-greek;lower-latin;lower-roman;upper-alpha;upper-latin;upper-roman")
	#define  list_style_type_atoms	atom_none,atom_circle,atom_disc,atom_square,atom_armenian,atom_cjk_ideographic,atom_decimal,atom_decimal_leading_zero,atom_georgian,atom_hebrew,atom_hiragana\
									atom_hiragana_iroha,atom_katakana,atom_katakana_iroha,atom_lower_alpha,atom_lower_greek,atom_lower_latin,atom_lower_roman,atom_upper_alpha,atom_upper_latin,atom_upper_roman,0

	enum list_style_type
	{
		list_style_type_none,
		list_style_type_circle,
		list_style_type_disc,
		list_style_type_square,
		list_style_type_armenian,
		list_style_type_cjk_ideographic,
		list_style_type_decimal,
		list_style_type_decimal_leading_zero,
		list_style_type_georgian,
		list_style_type_hebrew,
		list_style_type_hiragana,
		list_style_type_hiragana_iroha,
		list_style_type_katakana,
		list_style_type_katakana_iroha,
		list_style_type_lower_alpha,
		list_style_type_lower_greek,
		list_style_type_lower_latin,
		list_style_type_lower_roman,
		list_style_type_upper_alpha,
		list_style_type_upper_latin,
		list_style_type_upper_roman,
	};

//#define  list_style_position_strings	_t("inside;outside")
	#define  list_style_position_atoms	atom_inside,atom_outside,0

	enum list_style_position
	{
		list_style_position_inside,
		list_style_position_outside
	};

//#define  vertical_align_strings	_t("baseline;sub;super;top;text-top;middle;bottom;text-bottom")
	#define  vertical_align_atoms	atom_baseline,atom_sub,atom_super,atom_top,atom_text_top,atom_middle,atom_bottom,atom_text_bottom,0

	enum vertical_align
	{
		va_baseline,
		va_sub,
		va_super,
		va_top,
		va_text_top,
		va_middle,
		va_bottom,
		va_text_bottom
	};

//#define  border_width_strings	_t("thin;medium;thick")
	#define  border_width_atoms	atom_thin,atom_medium,atom_thick,0

	enum border_width
	{
		border_width_thin,
		border_width_medium,
		border_width_thick
	};

//#define  border_style_strings	_t("none;hidden;dotted;dashed;solid;double;groove;ridge;inset;outset")
	#define  border_style_atoms	atom_none,atom_hidden,atom_dotted,atom_dashed,atom_solid,atom_double,atom_groove,atom_ridge,atom_inset,atom_outset,0

	enum border_style
	{
		border_style_none,
		border_style_hidden,
		border_style_dotted,
		border_style_dashed,
		border_style_solid,
		border_style_double,
		border_style_groove,
		border_style_ridge,
		border_style_inset,
		border_style_outset
	};

	//#define  element_float_strings	_t("none;left;right")
	#define  element_float_atoms	atom_none,atom_left,atom_right,0

	enum element_float
	{
		float_none,
		float_left,
		float_right
	};

//#define  element_clear_strings	_t("none;left;right;both")
	#define  element_clear_atoms	atom_none,atom_left,atom_right,atom_both,0

	enum element_clear
	{
		clear_none,
		clear_left,
		clear_right,
		clear_both
	};

//#define  css_units_strings	_t("none;%;in;cm;mm;em;ex;pt;pc;px;dpi;dpcm;vw;vh;vmin;vmax")
	#define  css_units_atoms	atom_none,atom_perc,atom_in,atom_cm,atom_mm,atom_em,atom_ex,atom_pt,atom_pc,atom_px,atom_dpi,atom_dpcm,atom_vw,atom_vh,atom_vmin,atom_vmax,0

	enum css_units
	{
		css_units_none,
		css_units_percentage,
		css_units_in,
		css_units_cm,
		css_units_mm,
		css_units_em,
		css_units_ex,
		css_units_pt,
		css_units_pc,
		css_units_px,
		css_units_dpi,
		css_units_dpcm,
		css_units_vw,
		css_units_vh,
		css_units_vmin,
		css_units_vmax,
	};

//#define  background_attachment_strings	_t("scroll;fixed")
	#define  background_attachment_atoms	atom_scroll,atom_fixed,0

	enum background_attachment
	{
		background_attachment_scroll,
		background_attachment_fixed
	};

//#define  background_repeat_strings	_t("repeat;repeat-x;repeat-y;no-repeat")
	#define  background_repeat_atoms	atom_repeat,atom_repeat_x,atom_repeat_y,atom_no_repeat,0

	enum background_repeat
	{
		background_repeat_repeat,
		background_repeat_repeat_x,
		background_repeat_repeat_y,
		background_repeat_no_repeat
	};

//#define  background_box_strings	_t("border-box;padding-box;content-box")
	#define  background_box_atoms	atom_border_box,atom_padding_box,atom_content_box,0

	enum background_box
	{
		background_box_border,
		background_box_padding,
		background_box_content
	};

//#define element_position_strings	_t("static;relative;absolute;fixed")
	#define element_position_atoms	atom_static,atom_relative,atom_absolute,atom_fixed,0

	enum element_position
	{
		element_position_static,
		element_position_relative,
		element_position_absolute,
		element_position_fixed,
	};

//#define text_align_strings		_t("left;right;center;justify")
	#define text_align_atoms		atom_left,atom_right,atom_center,atom_justify,0

	enum text_align
	{
		text_align_left,
		text_align_right,
		text_align_center,
		text_align_justify
	};

//#define text_transform_strings		_t("none;capitalize;uppercase;lowercase")
#define text_transform_atoms		atom_none,atom_capitalize,atom_uppercase,atom_lowercase,0

	enum text_transform
	{
		text_transform_none,
		text_transform_capitalize,
		text_transform_uppercase,
		text_transform_lowercase
	};

//#define white_space_strings		_t("normal;nowrap;pre;pre-line;pre-wrap")
	#define white_space_atoms		atom_normal,atom_nowrap,atom_pre,atom_pre_line,atom_pre_wrap,0

	enum white_space
	{
		white_space_normal,
		white_space_nowrap,
		white_space_pre,
		white_space_pre_line,
		white_space_pre_wrap
	};

//#define overflow_strings		_t("visible;hidden;scroll;auto;no-display;no-content")
	#define overflow_atoms		atom_visible,atom_hidden,atom_scroll,atom_auto,atom_no_display,atom_no_content,0

	enum overflow
	{
		overflow_visible,
		overflow_hidden,
		overflow_scroll,
		overflow_auto,
		overflow_no_display,
		overflow_no_content
	};

//#define background_size_strings		_t("auto;cover;contain")
	#define background_size_atoms	atom_auto,atom_cover,atom_contain,0

	enum background_size
	{
		background_size_auto,
		background_size_cover,
		background_size_contain,
	};

//#define visibility_strings			_t("visible;hidden;collapse")
	#define visibility_atoms		atom_visible,atom_hidden,atom_collapse,0

	enum visibility
	{
		visibility_visible,
		visibility_hidden,
		visibility_collapse,
	};

//#define border_collapse_strings		_t("collapse;separate")
	#define border_collapse_atoms	atom_collapse,atom_separate,0

	enum border_collapse
	{
		border_collapse_collapse,
		border_collapse_separate,
	};


//#define pseudo_class_strings		_t("only-child;only-of-type;first-child;first-of-type;last-child;last-of-type;nth-child;nth-of-type;nth-last-child;nth-last-of-type;not;lang")
	#define pseudo_class_atoms		atom_only_child,atom_only_of_type,atom_first_child,atom_first_of_type,atom_last_child,atom_last_of_type,atom_nth_child,atom_nth_of_type,atom_nth_last_child,\
									atom_nth_last_of_type,atom_not,atom_lang,0

	enum pseudo_class
	{
		pseudo_class_only_child,
		pseudo_class_only_of_type,
		pseudo_class_first_child,
		pseudo_class_first_of_type,
		pseudo_class_last_child,
		pseudo_class_last_of_type,
		pseudo_class_nth_child,
		pseudo_class_nth_of_type,
		pseudo_class_nth_last_child,
		pseudo_class_nth_last_of_type,
		pseudo_class_not,
		pseudo_class_lang,
	};

//#define content_property_string		_t("none;normal;open-quote;close-quote;no-open-quote;no-close-quote")
	#define content_property_atoms		atom_none,atom_normal,atom_open_quote,atom_close_quote,atom_no_open_quote,atom_no_close_quote,0

	enum content_property
	{
		content_property_none,
		content_property_normal,
		content_property_open_quote,
		content_property_close_quote,
		content_property_no_open_quote,
		content_property_no_close_quote,
	};


	struct floated_box
	{
		typedef std::vector<floated_box>	vector;

		position		pos;
		element_float	float_side;
		element_clear	clear_floats;
		std::shared_ptr<element>	el;

		floated_box() = default;
		floated_box(const floated_box& val)
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
			el = std::move(val.el);
		}
		void operator=(floated_box&& val)
		{
			pos = val.pos;
			float_side = val.float_side;
			clear_floats = val.clear_floats;
			el = std::move(val.el);
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
