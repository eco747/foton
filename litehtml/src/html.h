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
#include "web_color.h"
#include "media_query.h"

namespace litehtml
{
	struct list_marker
	{
		xstring			image;
		const tchar_t*	baseurl;
		list_style_type	marker_type;
		web_color		color;
		position		pos;
	};

	// call back interface to draw text, images and other elements
	class document_container
	{
	public:
		virtual litehtml::uint_ptr	create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) = 0;
		virtual void				delete_font(litehtml::uint_ptr hFont) = 0;
		virtual int					text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont) = 0;
		virtual void				draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos) = 0;
		virtual int					pt_to_px(int pt) = 0;
		virtual int					get_default_font_size() const = 0;
		virtual const litehtml::tchar_t*	get_default_font_name() const = 0;
		virtual void				draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker) = 0;
		virtual void				load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready) = 0;
		virtual void				get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz) = 0;
		virtual void				draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg) = 0;
		virtual void				draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root) = 0;

		virtual	void				set_caption(const litehtml::tchar_t* caption) = 0;
		virtual	void				set_base_url(const litehtml::tchar_t* base_url) = 0;
		virtual void				link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el) = 0;
		virtual void				on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el) = 0;
		virtual	void				set_cursor(const litehtml::tchar_t* cursor) = 0;
		virtual	void				transform_text(litehtml::tstring& text, litehtml::text_transform tt) = 0;
		virtual void				import_css(xstring& text, const xstring& url, xstring& baseurl) = 0;
		virtual void				set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y) = 0;
		virtual void				del_clip() = 0;
		virtual void				get_client_rect(litehtml::position& client) const = 0;
		virtual litehtml::element::ptr	create_element(const litehtml::tchar_t *tag_name, const litehtml::attr_map &attributes, const std::shared_ptr<litehtml::document> &doc) = 0;
		virtual void				get_media_features(litehtml::media_features& media) const = 0;
		virtual void				get_language(litehtml::tstring& language, litehtml::tstring & culture) const = 0;
		virtual litehtml::tstring	resolve_color(const litehtml::tstring& /*color*/) const  { return litehtml::tstring(); }
	};

	void trim(tstring &s);
	void lcase(tstring &s);

	int	 value_index(const tstring& val, const tstring& strings, int defValue = -1, tchar_t delim = _t(';'));
	bool value_in_list(const tstring& val, const tstring& strings, tchar_t delim = _t(';'));

	int	 atom_index(const tchar_t* val, int defValue, ... );
	int	 atom_index( const tchar_t* val, int defValue, va_list lst );
	bool atom_in_list(atom val, ...);

	tstring::size_type find_close_bracket(const tstring &s, tstring::size_type off, tchar_t open_b = _t('('), tchar_t close_b = _t(')'));
	void split_string(const tstring& str, string_vector& tokens, const tstring& delims, const tstring& delims_preserve = _t(""), const tstring& quote = _t("\""));
	void join_string(tstring& str, const string_vector& tokens, const tstring& delims);

	inline int round_f(float val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}

	inline int round_d(double val)
	{
		int int_val = (int) val;
		if(val - int_val >= 0.5)
		{
			int_val++;
		}
		return int_val;
	}

	inline const tchar_t*	skip_sp( const tchar_t* p ) {
		tchar_t		ch;
		while( (ch=*p)==' ' || ch=='\t' ) {
			p++;
		}

		return p;
	}

	inline const tchar_t*	skip_nsp( const tchar_t* p ) {
		tchar_t		ch;
		while( (ch=*p)!=0 && ch!=' ' && ch!='\t' ) {
			p++;
		}

		return p;
	}

	void		atom_init( );
	atom		atom_create(const tchar_t* text , bool create_it=true);
	tstring		atom_get( atom v );

	enum atom {
		atom_null = 0,
		atom_perc = -2,
		atom_star = -3,
		atom__xx_border_spacing_x = -4,
		atom__xx_border_spacing_y = -5,
		atom___after = -6,
		atom___before = -7,
		atom_a = -8,
		atom_abbr = -9,
		atom_absolute = -10,
		atom_address = -11,
		atom_after = -12,
		atom_align = -13,
		atom_align_content = -14,
		atom_align_items = -15,
		atom_align_self = -16,
		atom_alt = -17,
		atom_area = -18,
		atom_armenian = -19,
		atom_article = -20,
		atom_aside = -21,
		atom_aspect_ratio = -22,
		atom_audio = -23,
		atom_auto = -24,
		atom_b = -25,
		atom_background = -26,
		atom_background_attachment = -27,
		atom_background_baseurl = -28,
		atom_background_clip = -29,
		atom_background_color = -30,
		atom_background_image = -31,
		atom_background_origin = -32,
		atom_background_position = -33,
		atom_background_position_x = -34,
		atom_background_position_y = -35,
		atom_background_repeat = -36,
		atom_background_size = -37,
		atom_base = -38,
		atom_baseline = -39,
		atom_bdi = -40,
		atom_bdo = -41,
		atom_before = -42,
		atom_bgcolor = -43,
		atom_big = -44,
		atom_block = -45,
		atom_blockquote = -46,
		atom_body = -47,
		atom_border = -48,
		atom_border_bottom = -49,
		atom_border_bottom_color = -50,
		atom_border_bottom_left_radius = -51,
		atom_border_bottom_left_radius_x = -52,
		atom_border_bottom_left_radius_y = -53,
		atom_border_bottom_right_radius = -54,
		atom_border_bottom_right_radius_x = -55,
		atom_border_bottom_right_radius_y = -56,
		atom_border_bottom_style = -57,
		atom_border_bottom_width = -58,
		atom_border_box = -59,
		atom_border_collapse = -60,
		atom_border_color = -61,
		atom_border_left = -62,
		atom_border_left_color = -63,
		atom_border_left_style = -64,
		atom_border_left_width = -65,
		atom_border_radius = -66,
		atom_border_radius_x = -67,
		atom_border_radius_y = -68,
		atom_border_right = -69,
		atom_border_right_color = -70,
		atom_border_right_style = -71,
		atom_border_right_width = -72,
		atom_border_spacing = -73,
		atom_border_style = -74,
		atom_border_top = -75,
		atom_border_top_color = -76,
		atom_border_top_left_radius = -77,
		atom_border_top_left_radius_x = -78,
		atom_border_top_left_radius_y = -79,
		atom_border_top_right_radius = -80,
		atom_border_top_right_radius_x = -81,
		atom_border_top_right_radius_y = -82,
		atom_border_top_style = -83,
		atom_border_top_width = -84,
		atom_border_width = -85,
		atom_both = -86,
		atom_bottom = -87,
		atom_box_shadow = -88,
		atom_box_sizing = -89,
		atom_br = -90,
		atom_braille = -91,
		atom_button = -92,
		atom_canvas = -93,
		atom_capitalize = -94,
		atom_caption = -95,
		atom_cellpadding = -96,
		atom_cellspacing = -97,
		atom_center = -98,
		atom_charset = -99,
		atom_checked = -100,
		atom_circle = -101,
		atom_cite = -102,
		atom_cjk_ideographic = -103,
		atom_class = -104,
		atom_clear = -105,
		atom_close_quote = -106,
		atom_cm = -107,
		atom_code = -108,
		atom_col = -109,
		atom_colgroup = -110,
		atom_collapse = -111,
		atom_color = -112,
		atom_color_index = -113,
		atom_cols = -114,
		atom_colspan = -115,
		atom_column_count = -116,
		atom_column_gap = -117,
		atom_column_width = -118,
		atom_contain = -119,
		atom_content = -120,
		atom_content_box = -121,
		atom_controls = -122,
		atom_cover = -123,
		atom_cursor = -124,
		atom_cx = -125,
		atom_cy = -126,
		atom_dashed = -127,
		atom_datalist = -128,
		atom_datetime = -129,
		atom_dd = -130,
		atom_decimal = -131,
		atom_decimal_leading_zero = -132,
		atom_del = -133,
		atom_details = -134,
		atom_device_aspect_ratio = -135,
		atom_device_height = -136,
		atom_device_width = -137,
		atom_dfn = -138,
		atom_dialog = -139,
		atom_dir = -140,
		atom_direction = -141,
		atom_disabled = -142,
		atom_disc = -143,
		atom_display = -144,
		atom_div = -145,
		atom_dl = -146,
		atom_dotted = -147,
		atom_double = -148,
		atom_dpcm = -149,
		atom_dpi = -150,
		atom_dt = -151,
		atom_em = -152,
		atom_embed = -153,
		atom_embossed = -154,
		atom_ex = -155,
		atom_face = -156,
		atom_fieldset = -157,
		atom_figcaption = -158,
		atom_figure = -159,
		atom_fill = -160,
		atom_first_child = -161,
		atom_first_of_type = -162,
		atom_fixed = -163,
		atom_flex = -164,
		atom_flex_basis = -165,
		atom_flex_direction = -166,
		atom_flex_flow = -167,
		atom_flex_grow = -168,
		atom_flex_wrap = -169,
		atom_float = -170,
		atom_font = -171,
		atom_font_face = -172,
		atom_font_family = -173,
		atom_font_size = -174,
		atom_font_style = -175,
		atom_font_variant = -176,
		atom_font_weight = -177,
		atom_footer = -178,
		atom_for = -179,
		atom_form = -180,
		atom_frame = -181,
		atom_frameset = -182,
		atom_georgian = -183,
		atom_groove = -184,
		atom_h1 = -185,
		atom_h2 = -186,
		atom_h3 = -187,
		atom_h4 = -188,
		atom_h5 = -189,
		atom_h6 = -190,
		atom_halign = -191,
		atom_handheld = -192,
		atom_head = -193,
		atom_header = -194,
		atom_hebrew = -195,
		atom_height = -196,
		atom_hgroup = -197,
		atom_hidden = -198,
		atom_hiragana = -199,
		atom_hiragana_iroha = -200,
		atom_hr = -201,
		atom_href = -202,
		atom_html = -203,
		atom_i = -204,
		atom_id = -205,
		atom_iframe = -206,
		atom_img = -207,
		atom_in = -208,
		atom_inherit = -209,
		atom_inline = -210,
		atom_inline_block = -211,
		atom_inline_table = -212,
		atom_input = -213,
		atom_ins = -214,
		atom_inset = -215,
		atom_inside = -216,
		atom_isindex = -217,
		atom_italic = -218,
		atom_justify = -219,
		atom_justify_content = -220,
		atom_katakana = -221,
		atom_katakana_iroha = -222,
		atom_kbd = -223,
		atom_keygen = -224,
		atom_label = -225,
		atom_landscape = -226,
		atom_lang = -227,
		atom_large = -228,
		atom_larger = -229,
		atom_last_child = -230,
		atom_last_of_type = -231,
		atom_layer = -232,
		atom_left = -233,
		atom_legend = -234,
		atom_letter_spacing = -235,
		atom_li = -236,
		atom_line_height = -237,
		atom_line_through = -238,
		atom_link = -239,
		atom_list = -240,
		atom_list_item = -241,
		atom_list_style = -242,
		atom_list_style_baseurl = -243,
		atom_list_style_image = -244,
		atom_list_style_position = -245,
		atom_list_style_type = -246,
		atom_listing = -247,
		atom_lower_alpha = -248,
		atom_lower_greek = -249,
		atom_lower_latin = -250,
		atom_lower_roman = -251,
		atom_lowercase = -252,
		atom_main = -253,
		atom_map = -254,
		atom_margin = -255,
		atom_margin_bottom = -256,
		atom_margin_left = -257,
		atom_margin_right = -258,
		atom_margin_top = -259,
		atom_mark = -260,
		atom_marquee = -261,
		atom_max = -262,
		atom_max_aspect_ratio = -263,
		atom_max_color = -264,
		atom_max_color_index = -265,
		atom_max_device_aspect_ratio = -266,
		atom_max_device_height = -267,
		atom_max_device_width = -268,
		atom_max_height = -269,
		atom_max_monochrome = -270,
		atom_max_resolution = -271,
		atom_max_width = -272,
		atom_media = -273,
		atom_medium = -274,
		atom_menu = -275,
		atom_meta = -276,
		atom_meter = -277,
		atom_middle = -278,
		atom_min = -279,
		atom_min_aspect_ratio = -280,
		atom_min_color = -281,
		atom_min_color_index = -282,
		atom_min_device_aspect_ratio = -283,
		atom_min_device_height = -284,
		atom_min_device_width = -285,
		atom_min_height = -286,
		atom_min_monochrome = -287,
		atom_min_resolution = -288,
		atom_min_width = -289,
		atom_mm = -290,
		atom_monochrome = -291,
		atom_name = -292,
		atom_nav = -293,
		atom_no_close_quote = -294,
		atom_no_content = -295,
		atom_no_display = -296,
		atom_no_open_quote = -297,
		atom_no_repeat = -298,
		atom_nobr = -299,
		atom_noframes = -300,
		atom_none = -301,
		atom_normal = -302,
		atom_not = -303,
		atom_nowrap = -304,
		atom_nth_child = -305,
		atom_nth_last_child = -306,
		atom_nth_last_of_type = -307,
		atom_nth_of_type = -308,
		atom_object = -309,
		atom_object_fit = -310,
		atom_ol = -311,
		atom_only_child = -312,
		atom_only_of_type = -313,
		atom_opacity = -314,
		atom_open_quote = -315,
		atom_optgroup = -316,
		atom_option = -317,
		atom_orientation = -318,
		atom_outline = -319,
		atom_outline_offset = -320,
		atom_output = -321,
		atom_outset = -322,
		atom_outside = -323,
		atom_overflow = -324,
		atom_overflow_x = -325,
		atom_overflow_y = -326,
		atom_overline = -327,
		atom_p = -328,
		atom_padding = -329,
		atom_padding_bottom = -330,
		atom_padding_box = -331,
		atom_padding_left = -332,
		atom_padding_right = -333,
		atom_padding_top = -334,
		atom_param = -335,
		atom_pc = -336,
		atom_placeholder = -337,
		atom_plaintext = -338,
		atom_pointer_events = -339,
		atom_portrait = -340,
		atom_position = -341,
		atom_pre = -342,
		atom_pre_line = -343,
		atom_pre_wrap = -344,
		atom_print = -345,
		atom_progress = -346,
		atom_projection = -347,
		atom_pseudo = -348,
		atom_pseudo_el = -349,
		atom_pt = -350,
		atom_px = -351,
		atom_q = -352,
		atom_r = -353,
		atom_readonly = -354,
		atom_rel = -355,
		atom_relative = -356,
		atom_repeat = -357,
		atom_repeat_x = -358,
		atom_repeat_y = -359,
		atom_resize = -360,
		atom_resolution = -361,
		atom_ridge = -362,
		atom_right = -363,
		atom_role = -364,
		atom_rows = -365,
		atom_rowspan = -366,
		atom_rp = -367,
		atom_rt = -368,
		atom_ruby = -369,
		atom_s = -370,
		atom_samp = -371,
		atom_screen = -372,
		atom_script = -373,
		atom_scroll = -374,
		atom_section = -375,
		atom_select = -376,
		atom_separate = -377,
		atom_size = -378,
		atom_small = -379,
		atom_small_caps = -380,
		atom_smaller = -381,
		atom_solid = -382,
		atom_span = -383,
		atom_speech = -384,
		atom_square = -385,
		atom_src = -386,
		atom_static = -387,
		atom_strike = -388,
		atom_strong = -389,
		atom_style = -390,
		atom_sub = -391,
		atom_summary = -392,
		atom_sup = -393,
		atom_super = -394,
		atom_svg = -395,
		atom_tabindex = -396,
		atom_table = -397,
		atom_table_caption = -398,
		atom_table_cell = -399,
		atom_table_column = -400,
		atom_table_column_group = -401,
		atom_table_footer_group = -402,
		atom_table_header_group = -403,
		atom_table_layout = -404,
		atom_table_row = -405,
		atom_table_row_group = -406,
		atom_tbody = -407,
		atom_td = -408,
		atom_template = -409,
		atom_text_align = -410,
		atom_text_bottom = -411,
		atom_text_decoration = -412,
		atom_text_indent = -413,
		atom_text_overflow = -414,
		atom_text_rendering = -415,
		atom_text_shadow = -416,
		atom_text_top = -417,
		atom_text_transform = -418,
		atom_textarea = -419,
		atom_tfoot = -420,
		atom_th = -421,
		atom_thead = -422,
		atom_thick = -423,
		atom_thin = -424,
		atom_time = -425,
		atom_title = -426,
		atom_top = -427,
		atom_tr = -428,
		atom_transparent = -429,
		atom_tt = -430,
		atom_tty = -431,
		atom_tv = -432,
		atom_type = -433,
		atom_u = -434,
		atom_ul = -435,
		atom_underline = -436,
		atom_unicode_bidi = -437,
		atom_upper_alpha = -438,
		atom_upper_latin = -439,
		atom_upper_roman = -440,
		atom_uppercase = -441,
		atom_user_drag = -442,
		atom_valign = -443,
		atom_value = -444,
		atom_var = -445,
		atom_vertical_align = -446,
		atom_vh = -447,
		atom_video = -448,
		atom_visibility = -449,
		atom_visible = -450,
		atom_vmax = -451,
		atom_vmin = -452,
		atom_vw = -453,
		atom_white_space = -454,
		atom_width = -455,
		atom_word_break = -456,
		atom_word_spacing = -457,
		atom_word_wrap = -458,
		atom_x_large = -459,
		atom_x_small = -460,
		atom_xmp = -461,
		atom_xx_large = -462,
		atom_xx_small = -463,
		atom_z_index = -464,
		atom_zoom = -465,
	};
}
