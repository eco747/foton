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
	int	 value_index_atom(const tchar_t* val, const atom* predef_values, int defValue = -1);

	bool value_in_list(const tstring& val, const tstring& strings, tchar_t delim = _t(';'));
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


	void		atom_init( );
	atom		atom_create(const tchar_t* text , bool create_it=true);
	tstring		atom_get( atom v );

	enum atom {
		atom_null = 0,
		atom_star = -1,
		atom__xx_border_spacing_x = -2,
		atom__xx_border_spacing_y = -3,
		atom___after = -4,
		atom___before = -5,
		atom_a = -6,
		atom_abbr = -7,
		atom_absolute = -8,
		atom_address = -9,
		atom_after = -10,
		atom_align = -11,
		atom_align_content = -12,
		atom_align_items = -13,
		atom_align_self = -14,
		atom_alt = -15,
		atom_area = -16,
		atom_article = -17,
		atom_aside = -18,
		atom_audio = -19,
		atom_auto = -20,
		atom_b = -21,
		atom_background = -22,
		atom_background_attachment = -23,
		atom_background_baseurl = -24,
		atom_background_clip = -25,
		atom_background_color = -26,
		atom_background_image = -27,
		atom_background_origin = -28,
		atom_background_position = -29,
		atom_background_position_x = -30,
		atom_background_position_y = -31,
		atom_background_repeat = -32,
		atom_background_size = -33,
		atom_base = -34,
		atom_bdi = -35,
		atom_bdo = -36,
		atom_before = -37,
		atom_bgcolor = -38,
		atom_big = -39,
		atom_block = -40,
		atom_blockquote = -41,
		atom_body = -42,
		atom_border = -43,
		atom_border_bottom = -44,
		atom_border_bottom_color = -45,
		atom_border_bottom_left_radius = -46,
		atom_border_bottom_left_radius_x = -47,
		atom_border_bottom_left_radius_y = -48,
		atom_border_bottom_right_radius = -49,
		atom_border_bottom_right_radius_x = -50,
		atom_border_bottom_right_radius_y = -51,
		atom_border_bottom_style = -52,
		atom_border_bottom_width = -53,
		atom_border_box = -54,
		atom_border_collapse = -55,
		atom_border_color = -56,
		atom_border_left = -57,
		atom_border_left_color = -58,
		atom_border_left_style = -59,
		atom_border_left_width = -60,
		atom_border_radius = -61,
		atom_border_radius_x = -62,
		atom_border_radius_y = -63,
		atom_border_right = -64,
		atom_border_right_color = -65,
		atom_border_right_style = -66,
		atom_border_right_width = -67,
		atom_border_spacing = -68,
		atom_border_style = -69,
		atom_border_top = -70,
		atom_border_top_color = -71,
		atom_border_top_left_radius = -72,
		atom_border_top_left_radius_x = -73,
		atom_border_top_left_radius_y = -74,
		atom_border_top_right_radius = -75,
		atom_border_top_right_radius_x = -76,
		atom_border_top_right_radius_y = -77,
		atom_border_top_style = -78,
		atom_border_top_width = -79,
		atom_border_width = -80,
		atom_bottom = -81,
		atom_box_shadow = -82,
		atom_box_sizing = -83,
		atom_br = -84,
		atom_button = -85,
		atom_canvas = -86,
		atom_capitalize = -87,
		atom_caption = -88,
		atom_cellpadding = -89,
		atom_cellspacing = -90,
		atom_center = -91,
		atom_charset = -92,
		atom_checked = -93,
		atom_circle = -94,
		atom_cite = -95,
		atom_class = -96,
		atom_clear = -97,
		atom_code = -98,
		atom_col = -99,
		atom_colgroup = -100,
		atom_color = -101,
		atom_cols = -102,
		atom_colspan = -103,
		atom_column_count = -104,
		atom_column_gap = -105,
		atom_column_width = -106,
		atom_content = -107,
		atom_content_boxrepeat = -108,
		atom_controls = -109,
		atom_cursor = -110,
		atom_cx = -111,
		atom_cy = -112,
		atom_datalist = -113,
		atom_datetime = -114,
		atom_dd = -115,
		atom_del = -116,
		atom_details = -117,
		atom_dfn = -118,
		atom_dialog = -119,
		atom_dir = -120,
		atom_direction = -121,
		atom_disabled = -122,
		atom_display = -123,
		atom_div = -124,
		atom_dl = -125,
		atom_dt = -126,
		atom_em = -127,
		atom_embed = -128,
		atom_face = -129,
		atom_fieldset = -130,
		atom_figcaption = -131,
		atom_figure = -132,
		atom_fill = -133,
		atom_fixed = -134,
		atom_flex = -135,
		atom_flex_basis = -136,
		atom_flex_direction = -137,
		atom_flex_flow = -138,
		atom_flex_grow = -139,
		atom_flex_wrap = -140,
		atom_float = -141,
		atom_font = -142,
		atom_font_face = -143,
		atom_font_family = -144,
		atom_font_size = -145,
		atom_font_style = -146,
		atom_font_variant = -147,
		atom_font_weight = -148,
		atom_footer = -149,
		atom_for = -150,
		atom_form = -151,
		atom_frame = -152,
		atom_frameset = -153,
		atom_h1 = -154,
		atom_h2 = -155,
		atom_h3 = -156,
		atom_h4 = -157,
		atom_h5 = -158,
		atom_h6 = -159,
		atom_halign = -160,
		atom_head = -161,
		atom_header = -162,
		atom_height = -163,
		atom_hgroup = -164,
		atom_hr = -165,
		atom_href = -166,
		atom_html = -167,
		atom_i = -168,
		atom_id = -169,
		atom_iframe = -170,
		atom_img = -171,
		atom_inherit = -172,
		atom_inline = -173,
		atom_inline_block = -174,
		atom_inline_table = -175,
		atom_input = -176,
		atom_ins = -177,
		atom_isindex = -178,
		atom_justify_content = -179,
		atom_kbd = -180,
		atom_keygen = -181,
		atom_label = -182,
		atom_lang = -183,
		atom_layer = -184,
		atom_left = -185,
		atom_legend = -186,
		atom_letter_spacing = -187,
		atom_li = -188,
		atom_line_height = -189,
		atom_line_through = -190,
		atom_link = -191,
		atom_list = -192,
		atom_list_item = -193,
		atom_list_style = -194,
		atom_list_style_baseurl = -195,
		atom_list_style_image = -196,
		atom_list_style_position = -197,
		atom_list_style_type = -198,
		atom_listing = -199,
		atom_lowercase = -200,
		atom_main = -201,
		atom_map = -202,
		atom_margin = -203,
		atom_margin_bottom = -204,
		atom_margin_left = -205,
		atom_margin_right = -206,
		atom_margin_top = -207,
		atom_mark = -208,
		atom_marquee = -209,
		atom_max = -210,
		atom_max_height = -211,
		atom_max_width = -212,
		atom_media = -213,
		atom_menu = -214,
		atom_meta = -215,
		atom_meter = -216,
		atom_min = -217,
		atom_min_height = -218,
		atom_min_width = -219,
		atom_name = -220,
		atom_nav = -221,
		atom_no_repeat = -222,
		atom_nobr = -223,
		atom_noframes = -224,
		atom_none = -225,
		atom_nowrap = -226,
		atom_object = -227,
		atom_object_fit = -228,
		atom_ol = -229,
		atom_opacity = -230,
		atom_optgroup = -231,
		atom_option = -232,
		atom_outline = -233,
		atom_outline_offset = -234,
		atom_output = -235,
		atom_overflow = -236,
		atom_overflow_x = -237,
		atom_overflow_y = -238,
		atom_overline = -239,
		atom_p = -240,
		atom_padding = -241,
		atom_padding_bottom = -242,
		atom_padding_box = -243,
		atom_padding_left = -244,
		atom_padding_right = -245,
		atom_padding_top = -246,
		atom_param = -247,
		atom_placeholder = -248,
		atom_plaintext = -249,
		atom_pointer_events = -250,
		atom_position = -251,
		atom_pre = -252,
		atom_progress = -253,
		atom_pseudo = -254,
		atom_pseudo_el = -255,
		atom_q = -256,
		atom_r = -257,
		atom_readonly = -258,
		atom_rel = -259,
		atom_relative = -260,
		atom_repeat_x = -261,
		atom_repeat_y = -262,
		atom_resize = -263,
		atom_right = -264,
		atom_role = -265,
		atom_rows = -266,
		atom_rowspan = -267,
		atom_rp = -268,
		atom_rt = -269,
		atom_ruby = -270,
		atom_s = -271,
		atom_samp = -272,
		atom_script = -273,
		atom_section = -274,
		atom_select = -275,
		atom_size = -276,
		atom_small = -277,
		atom_span = -278,
		atom_src = -279,
		atom_static = -280,
		atom_strike = -281,
		atom_strong = -282,
		atom_style = -283,
		atom_sub = -284,
		atom_summary = -285,
		atom_sup = -286,
		atom_svg = -287,
		atom_tabindex = -288,
		atom_table = -289,
		atom_table_caption = -290,
		atom_table_cell = -291,
		atom_table_column = -292,
		atom_table_column_group = -293,
		atom_table_footer_group = -294,
		atom_table_header_group = -295,
		atom_table_layout = -296,
		atom_table_row = -297,
		atom_table_row_group = -298,
		atom_tbody = -299,
		atom_td = -300,
		atom_template = -301,
		atom_text_align = -302,
		atom_text_decoration = -303,
		atom_text_indent = -304,
		atom_text_overflow = -305,
		atom_text_rendering = -306,
		atom_text_shadow = -307,
		atom_text_transform = -308,
		atom_textarea = -309,
		atom_tfoot = -310,
		atom_th = -311,
		atom_thead = -312,
		atom_time = -313,
		atom_title = -314,
		atom_top = -315,
		atom_tr = -316,
		atom_transparent = -317,
		atom_tt = -318,
		atom_type = -319,
		atom_u = -320,
		atom_ul = -321,
		atom_underline = -322,
		atom_unicode_bidi = -323,
		atom_uppercase = -324,
		atom_user_drag = -325,
		atom_valign = -326,
		atom_value = -327,
		atom_var = -328,
		atom_vertical_align = -329,
		atom_video = -330,
		atom_visibility = -331,
		atom_white_space = -332,
		atom_width = -333,
		atom_word_break = -334,
		atom_word_spacing = -335,
		atom_word_wrap = -336,
		atom_xmp = -337,
		atom_z_index = -338,
		atom_zoom = -339,
	};

}
