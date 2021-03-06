#ifndef __KEYWORDS_H
#define __KEYWORDS_H


namespace litehtml {



	// -------------------------------------------------

	int __get_media_type( const tchar_t* text, int len = -1, int def = 0 );

	enum media_type {
		media_type_none = 0,
		media_type_all = 1,
		media_type_screen = 2,
		media_type_print = 3,
		media_type_braille = 4,
		media_type_embossed = 5,
		media_type_handheld = 6,
		media_type_projection = 7,
		media_type_speech = 8,
		media_type_tty = 9,
		media_type_tv = 10,
	};



	// -------------------------------------------------

	int __get_atom( const tchar_t* text, int len = -1, int def = 0 );

	enum atom {
		atom_null = 0,
		atom_star = 1,
		atom__xx_border_spacing_x = 2,
		atom__xx_border_spacing_y = 3,
		atom___after = 4,
		atom___before = 5,
		atom_a = 6,
		atom_abbr = 7,
		atom_address = 8,
		atom_after = 9,
		atom_align_content = 10,
		atom_align_items = 11,
		atom_align_self = 12,
		atom_alt = 13,
		atom_area = 14,
		atom_article = 15,
		atom_aside = 16,
		atom_audio = 17,
		atom_auto = 18,
		atom_b = 19,
		atom_background = 20,
		atom_background_attachment = 21,
		atom_background_baseurl = 22,
		atom_background_clip = 23,
		atom_background_color = 24,
		atom_background_image = 25,
		atom_background_origin = 26,
		atom_background_position = 27,
		atom_background_position_x = 28,
		atom_background_position_y = 29,
		atom_background_repeat = 30,
		atom_background_size = 31,
		atom_base = 32,
		atom_bdi = 33,
		atom_bdo = 34,
		atom_before = 35,
		atom_bgcolor = 36,
		atom_big = 37,
		atom_blockquote = 38,
		atom_body = 39,
		atom_border = 40,
		atom_border_bottom = 41,
		atom_border_bottom_color = 42,
		atom_border_bottom_left_radius = 43,
		atom_border_bottom_left_radius_x = 44,
		atom_border_bottom_left_radius_y = 45,
		atom_border_bottom_right_radius = 46,
		atom_border_bottom_right_radius_x = 47,
		atom_border_bottom_right_radius_y = 48,
		atom_border_bottom_style = 49,
		atom_border_bottom_width = 50,
		atom_border_collapse = 51,
		atom_border_color = 52,
		atom_border_left = 53,
		atom_border_left_color = 54,
		atom_border_left_style = 55,
		atom_border_left_width = 56,
		atom_border_radius = 57,
		atom_border_radius_x = 58,
		atom_border_radius_y = 59,
		atom_border_right = 60,
		atom_border_right_color = 61,
		atom_border_right_style = 62,
		atom_border_right_width = 63,
		atom_border_spacing = 64,
		atom_border_style = 65,
		atom_border_top = 66,
		atom_border_top_color = 67,
		atom_border_top_left_radius = 68,
		atom_border_top_left_radius_x = 69,
		atom_border_top_left_radius_y = 70,
		atom_border_top_right_radius = 71,
		atom_border_top_right_radius_x = 72,
		atom_border_top_right_radius_y = 73,
		atom_border_top_style = 74,
		atom_border_top_width = 75,
		atom_border_width = 76,
		atom_bottom = 77,
		atom_box_shadow = 78,
		atom_box_sizing = 79,
		atom_br = 80,
		atom_button = 81,
		atom_canvas = 82,
		atom_caption = 83,
		atom_cellpadding = 84,
		atom_cellspacing = 85,
		atom_center = 86,
		atom_charset = 87,
		atom_checked = 88,
		atom_circle = 89,
		atom_cite = 90,
		atom_class = 91,
		atom_clear = 92,
		atom_code = 93,
		atom_col = 94,
		atom_colgroup = 95,
		atom_color = 96,
		atom_cols = 97,
		atom_colspan = 98,
		atom_column_count = 99,
		atom_column_gap = 100,
		atom_column_width = 101,
		atom_content = 102,
		atom_controls = 103,
		atom_cursor = 104,
		atom_cx = 105,
		atom_cy = 106,
		atom_datalist = 107,
		atom_datetime = 108,
		atom_dd = 109,
		atom_del = 110,
		atom_details = 111,
		atom_dfn = 112,
		atom_dialog = 113,
		atom_dir = 114,
		atom_direction = 115,
		atom_disabled = 116,
		atom_display = 117,
		atom_div = 118,
		atom_dl = 119,
		atom_dt = 120,
		atom_em = 121,
		atom_embed = 122,
		atom_face = 123,
		atom_fieldset = 124,
		atom_figcaption = 125,
		atom_figure = 126,
		atom_fill = 127,
		atom_flex_basis = 128,
		atom_flex_direction = 129,
		atom_flex_flow = 130,
		atom_flex_grow = 131,
		atom_flex_wrap = 132,
		atom_float = 133,
		atom_font = 134,
		atom_font_face = 135,
		atom_font_family = 136,
		atom_font_size = 137,
		atom_font_style = 138,
		atom_font_variant = 139,
		atom_font_weight = 140,
		atom_footer = 141,
		atom_for = 142,
		atom_form = 143,
		atom_frame = 144,
		atom_frameset = 145,
		atom_h1 = 146,
		atom_h2 = 147,
		atom_h3 = 148,
		atom_h4 = 149,
		atom_h5 = 150,
		atom_h6 = 151,
		atom_halign = 152,
		atom_head = 153,
		atom_header = 154,
		atom_height = 155,
		atom_hgroup = 156,
		atom_hr = 157,
		atom_href = 158,
		atom_html = 159,
		atom_i = 160,
		atom_id = 161,
		atom_iframe = 162,
		atom_img = 163,
		atom_inherit = 164,
		atom_initial = 165,
		atom_input = 166,
		atom_ins = 167,
		atom_isindex = 168,
		atom_justify_content = 169,
		atom_kbd = 170,
		atom_keygen = 171,
		atom_label = 172,
		atom_lang = 173,
		atom_layer = 174,
		atom_left = 175,
		atom_legend = 176,
		atom_letter_spacing = 177,
		atom_li = 178,
		atom_line_height = 179,
		atom_link = 180,
		atom_list = 181,
		atom_list_style = 182,
		atom_list_style_baseurl = 183,
		atom_list_style_image = 184,
		atom_list_style_position = 185,
		atom_list_style_type = 186,
		atom_listing = 187,
		atom_main = 188,
		atom_map = 189,
		atom_margin = 190,
		atom_margin_bottom = 191,
		atom_margin_left = 192,
		atom_margin_right = 193,
		atom_margin_top = 194,
		atom_mark = 195,
		atom_marquee = 196,
		atom_max = 197,
		atom_max_height = 198,
		atom_max_width = 199,
		atom_media = 200,
		atom_menu = 201,
		atom_meta = 202,
		atom_meter = 203,
		atom_min = 204,
		atom_min_height = 205,
		atom_min_width = 206,
		atom_name = 207,
		atom_nav = 208,
		atom_nobr = 209,
		atom_noframes = 210,
		atom_none = 211,
		atom_normal = 212,
		atom_nowrap = 213,
		atom_object = 214,
		atom_object_fit = 215,
		atom_ol = 216,
		atom_opacity = 217,
		atom_optgroup = 218,
		atom_option = 219,
		atom_outline = 220,
		atom_outline_offset = 221,
		atom_output = 222,
		atom_overflow = 223,
		atom_overflow_x = 224,
		atom_overflow_y = 225,
		atom_p = 226,
		atom_padding = 227,
		atom_padding_bottom = 228,
		atom_padding_left = 229,
		atom_padding_right = 230,
		atom_padding_top = 231,
		atom_param = 232,
		atom_placeholder = 233,
		atom_plaintext = 234,
		atom_pointer_events = 235,
		atom_position = 236,
		atom_pre = 237,
		atom_progress = 238,
		atom_pseudo = 239,
		atom_pseudo_el = 240,
		atom_q = 241,
		atom_r = 242,
		atom_readonly = 243,
		atom_rel = 244,
		atom_resize = 245,
		atom_right = 246,
		atom_role = 247,
		atom_rows = 248,
		atom_rowspan = 249,
		atom_rp = 250,
		atom_rt = 251,
		atom_ruby = 252,
		atom_s = 253,
		atom_samp = 254,
		atom_script = 255,
		atom_section = 256,
		atom_select = 257,
		atom_size = 258,
		atom_small = 259,
		atom_span = 260,
		atom_src = 261,
		atom_strike = 262,
		atom_strong = 263,
		atom_style = 264,
		atom_sub = 265,
		atom_summary = 266,
		atom_sup = 267,
		atom_svg = 268,
		atom_tabindex = 269,
		atom_table = 270,
		atom_table_layout = 271,
		atom_tbody = 272,
		atom_td = 273,
		atom_template = 274,
		atom_text_align = 275,
		atom_text_decoration = 276,
		atom_text_indent = 277,
		atom_text_overflow = 278,
		atom_text_rendering = 279,
		atom_text_shadow = 280,
		atom_text_transform = 281,
		atom_textarea = 282,
		atom_tfoot = 283,
		atom_th = 284,
		atom_thead = 285,
		atom_time = 286,
		atom_title = 287,
		atom_top = 288,
		atom_tr = 289,
		atom_transparent = 290,
		atom_tt = 291,
		atom_type = 292,
		atom_u = 293,
		atom_ul = 294,
		atom_unicode_bidi = 295,
		atom_unset = 296,
		atom_user_drag = 297,
		atom_valign = 298,
		atom_value = 299,
		atom_var = 300,
		atom_vertical_align = 301,
		atom_video = 302,
		atom_visibility = 303,
		atom_white_space = 304,
		atom_width = 305,
		atom_word_break = 306,
		atom_word_spacing = 307,
		atom_word_wrap = 308,
		atom_xmp = 309,
		atom_z_index = 310,
		atom_zoom = 311,
	};



	// -------------------------------------------------

	int __get_style_display( const tchar_t* text, int len = -1, int def = 0 );

	enum style_display {
		style_display_none = 0,
		style_display_block = 1,
		style_display_inline = 2,
		style_display_inline_block = 3,
		style_display_inline_table = 4,
		style_display_list_item = 5,
		style_display_table = 6,
		style_display_table_caption = 7,
		style_display_table_cell = 8,
		style_display_table_column = 9,
		style_display_table_column_group = 10,
		style_display_table_footer_group = 11,
		style_display_table_header_group = 12,
		style_display_table_row = 13,
		style_display_table_row_group = 14,
		style_display_inline_text = 15,
		style_display_flex = 16,
	};



	// -------------------------------------------------

	int __get_font_size( const tchar_t* text, int len = -1, int def = 0 );

	enum font_size {
		font_size_xx_small = 0,
		font_size_x_small = 1,
		font_size_small = 2,
		font_size_medium = 3,
		font_size_large = 4,
		font_size_x_large = 5,
		font_size_xx_large = 6,
		font_size_smaller = 7,
		font_size_larger = 8,
	};



	// -------------------------------------------------

	int __get_list_style_type( const tchar_t* text, int len = -1, int def = 0 );

	enum list_style_type {
		list_style_type_none = 0,
		list_style_type_circle = 1,
		list_style_type_disc = 2,
		list_style_type_square = 3,
		list_style_type_armenian = 4,
		list_style_type_cjk_ideographic = 5,
		list_style_type_decimal = 6,
		list_style_type_decimal_leading_zero = 7,
		list_style_type_georgian = 8,
		list_style_type_hebrew = 9,
		list_style_type_hiragana = 10,
		list_style_type_hiragana_iroha = 11,
		list_style_type_katakana = 12,
		list_style_type_katakana_iroha = 13,
		list_style_type_lower_alpha = 14,
		list_style_type_lower_greek = 15,
		list_style_type_lower_latin = 16,
		list_style_type_lower_roman = 17,
		list_style_type_upper_alpha = 18,
		list_style_type_upper_latin = 19,
		list_style_type_upper_roman = 20,
	};



	// -------------------------------------------------

	int __get_vertical_align( const tchar_t* text, int len = -1, int def = 0 );

	enum vertical_align {
		vertical_align_baseline = 0,
		vertical_align_sub = 1,
		vertical_align_super = 2,
		vertical_align_top = 3,
		vertical_align_text_top = 4,
		vertical_align_middle = 5,
		vertical_align_bottom = 6,
		vertical_align_text_bottom = 7,
	};



	// -------------------------------------------------

	int __get_border_style( const tchar_t* text, int len = -1, int def = 0 );

	enum border_style {
		border_style_none = 0,
		border_style_hidden = 1,
		border_style_dotted = 2,
		border_style_dashed = 3,
		border_style_solid = 4,
		border_style_double = 5,
		border_style_groove = 6,
		border_style_ridge = 7,
		border_style_inset = 8,
		border_style_outset = 9,
	};



	// -------------------------------------------------

	int __get_font_style( const tchar_t* text, int len = -1, int def = 0 );

	enum font_style {
		font_style_normal = 0,
		font_style_italic = 1,
	};



	// -------------------------------------------------

	int __get_font_variant( const tchar_t* text, int len = -1, int def = 0 );

	enum font_variant {
		font_variant_normal = 0,
		font_variant_small_caps = 1,
	};



	// -------------------------------------------------

	int __get_border_width( const tchar_t* text, int len = -1, int def = 0 );

	enum border_width {
		border_width_thin = 0,
		border_width_medium = 1,
		border_width_thick = 2,
	};



	// -------------------------------------------------

	int __get_element_float( const tchar_t* text, int len = -1, int def = 0 );

	enum element_float {
		element_float_none = 0,
		element_float_left = 1,
		element_float_right = 2,
	};



	// -------------------------------------------------

	int __get_element_clear( const tchar_t* text, int len = -1, int def = 0 );

	enum element_clear {
		element_clear_none = 0,
		element_clear_left = 1,
		element_clear_right = 2,
		element_clear_both = 3,
	};



	// -------------------------------------------------

	int __get_css_units( const tchar_t* text, int len = -1, int def = 0 );

	enum css_units {
		css_units_none = 0,
		css_units_perc = 1,
		css_units_in = 2,
		css_units_cm = 3,
		css_units_mm = 4,
		css_units_em = 5,
		css_units_ex = 6,
		css_units_pt = 7,
		css_units_pc = 8,
		css_units_px = 9,
		css_units_dpi = 10,
		css_units_dpcm = 11,
		css_units_vw = 12,
		css_units_vh = 13,
		css_units_vmin = 14,
		css_units_vmax = 15,
	};



	// -------------------------------------------------

	int __get_background_attachment( const tchar_t* text, int len = -1, int def = 0 );

	enum background_attachment {
		background_attachment_scroll = 0,
		background_attachment_fixed = 1,
	};



	// -------------------------------------------------

	int __get_background_repeat( const tchar_t* text, int len = -1, int def = 0 );

	enum background_repeat {
		background_repeat_repeat = 0,
		background_repeat_repeat_x = 1,
		background_repeat_repeat_y = 2,
		background_repeat_no_repeat = 3,
	};



	// -------------------------------------------------

	int __get_background_box( const tchar_t* text, int len = -1, int def = 0 );

	enum background_box {
		background_box_border_box = 0,
		background_box_padding_box = 1,
		background_box_content_box = 2,
	};



	// -------------------------------------------------

	int __get_element_position( const tchar_t* text, int len = -1, int def = 0 );

	enum element_position {
		element_position_static = 0,
		element_position_relative = 1,
		element_position_absolute = 2,
		element_position_fixed = 3,
	};



	// -------------------------------------------------

	int __get_text_align( const tchar_t* text, int len = -1, int def = 0 );

	enum text_align {
		text_align_left = 0,
		text_align_right = 1,
		text_align_center = 2,
		text_align_justify = 3,
	};



	// -------------------------------------------------

	int __get_text_transform( const tchar_t* text, int len = -1, int def = 0 );

	enum text_transform {
		text_transform_none = 0,
		text_transform_capitalize = 1,
		text_transform_uppercase = 2,
		text_transform_lowercase = 3,
	};



	// -------------------------------------------------

	int __get_white_space( const tchar_t* text, int len = -1, int def = 0 );

	enum white_space {
		white_space_normal = 0,
		white_space_nowrap = 1,
		white_space_pre = 2,
		white_space_pre_line = 3,
		white_space_pre_wrap = 4,
	};



	// -------------------------------------------------

	int __get_overflow( const tchar_t* text, int len = -1, int def = 0 );

	enum overflow {
		overflow_visible = 0,
		overflow_hidden = 1,
		overflow_scroll = 2,
		overflow_auto = 3,
		overflow_no_display = 4,
		overflow_no_content = 5,
	};



	// -------------------------------------------------

	int __get_background_size( const tchar_t* text, int len = -1, int def = 0 );

	enum background_size {
		background_size_auto = 0,
		background_size_cover = 1,
		background_size_contain = 2,
	};



	// -------------------------------------------------

	int __get_visibility( const tchar_t* text, int len = -1, int def = 0 );

	enum visibility {
		visibility_visible = 0,
		visibility_hidden = 1,
		visibility_collapse = 2,
	};



	// -------------------------------------------------

	int __get_border_collapse( const tchar_t* text, int len = -1, int def = 0 );

	enum border_collapse {
		border_collapse_collapse = 0,
		border_collapse_separate = 1,
	};



	// -------------------------------------------------

	int __get_pseudo_class( const tchar_t* text, int len = -1, int def = 0 );

	enum pseudo_class {
		pseudo_class_only_child = 0,
		pseudo_class_only_of_type = 1,
		pseudo_class_first_child = 2,
		pseudo_class_first_of_type = 3,
		pseudo_class_last_child = 4,
		pseudo_class_last_of_type = 5,
		pseudo_class_nth_child = 6,
		pseudo_class_nth_of_type = 7,
		pseudo_class_nth_last_child = 8,
		pseudo_class_nth_last_of_type = 9,
		pseudo_class_not = 10,
		pseudo_class_lang = 11,
	};



	// -------------------------------------------------

	int __get_content_property( const tchar_t* text, int len = -1, int def = 0 );

	enum content_property {
		content_property_none = 0,
		content_property_normal = 1,
		content_property_open_quote = 2,
		content_property_close_quote = 3,
		content_property_no_open_quote = 4,
		content_property_no_close_quote = 5,
	};



	// -------------------------------------------------

	int __get_media_orientation( const tchar_t* text, int len = -1, int def = 0 );

	enum media_orientation {
		media_orientation_portrait = 0,
		media_orientation_landscape = 1,
	};



	// -------------------------------------------------

	int __get_media_feature( const tchar_t* text, int len = -1, int def = 0 );

	enum media_feature {
		media_feature_none = 0,
		media_feature_width = 1,
		media_feature_min_width = 2,
		media_feature_max_width = 3,
		media_feature_height = 4,
		media_feature_min_height = 5,
		media_feature_max_height = 6,
		media_feature_device_width = 7,
		media_feature_min_device_width = 8,
		media_feature_max_device_width = 9,
		media_feature_device_height = 10,
		media_feature_min_device_height = 11,
		media_feature_max_device_height = 12,
		media_feature_orientation = 13,
		media_feature_aspect_ratio = 14,
		media_feature_min_aspect_ratio = 15,
		media_feature_max_aspect_ratio = 16,
		media_feature_device_aspect_ratio = 17,
		media_feature_min_device_aspect_ratio = 18,
		media_feature_max_device_aspect_ratio = 19,
		media_feature_color = 20,
		media_feature_min_color = 21,
		media_feature_max_color = 22,
		media_feature_color_index = 23,
		media_feature_min_color_index = 24,
		media_feature_max_color_index = 25,
		media_feature_monochrome = 26,
		media_feature_min_monochrome = 27,
		media_feature_max_monochrome = 28,
		media_feature_resolution = 29,
		media_feature_min_resolution = 30,
		media_feature_max_resolution = 31,
	};



	// -------------------------------------------------

	int __get_box_sizing( const tchar_t* text, int len = -1, int def = 0 );

	enum box_sizing {
		box_sizing_content_box = 0,
		box_sizing_border_box = 1,
	};



	// -------------------------------------------------

	int __get_web_colors( const tchar_t* text, int len = -1, int def = 0 );

	enum web_colors {
		web_colors_transparent = 0,
		web_colors_aliceblue = 1,
		web_colors_antiquewhite = 2,
		web_colors_aqua = 3,
		web_colors_aquamarine = 4,
		web_colors_azure = 5,
		web_colors_beige = 6,
		web_colors_bisque = 7,
		web_colors_black = 8,
		web_colors_blanchedalmond = 9,
		web_colors_blue = 10,
		web_colors_blueviolet = 11,
		web_colors_brown = 12,
		web_colors_burlywood = 13,
		web_colors_cadetblue = 14,
		web_colors_chartreuse = 15,
		web_colors_chocolate = 16,
		web_colors_coral = 17,
		web_colors_cornflowerblue = 18,
		web_colors_cornsilk = 19,
		web_colors_crimson = 20,
		web_colors_cyan = 21,
		web_colors_darkblue = 22,
		web_colors_darkcyan = 23,
		web_colors_darkgoldenrod = 24,
		web_colors_darkgray = 25,
		web_colors_darkgrey = 26,
		web_colors_darkgreen = 27,
		web_colors_darkkhaki = 28,
		web_colors_darkmagenta = 29,
		web_colors_darkolivegreen = 30,
		web_colors_darkorange = 31,
		web_colors_darkorchid = 33,
		web_colors_darkred = 34,
		web_colors_darksalmon = 35,
		web_colors_darkseagreen = 36,
		web_colors_darkslateblue = 37,
		web_colors_darkslategray = 38,
		web_colors_darkslategrey = 39,
		web_colors_darkturquoise = 40,
		web_colors_darkviolet = 41,
		web_colors_deeppink = 42,
		web_colors_deepskyblue = 43,
		web_colors_dimgray = 44,
		web_colors_dimgrey = 45,
		web_colors_dodgerblue = 46,
		web_colors_firebrick = 47,
		web_colors_floralwhite = 48,
		web_colors_forestgreen = 49,
		web_colors_fuchsia = 50,
		web_colors_gainsboro = 51,
		web_colors_ghostwhite = 52,
		web_colors_gold = 53,
		web_colors_goldenrod = 54,
		web_colors_gray = 55,
		web_colors_grey = 56,
		web_colors_green = 57,
		web_colors_greenyellow = 58,
		web_colors_honeydew = 59,
		web_colors_hotpink = 60,
		web_colors_ivory = 61,
		web_colors_khaki = 62,
		web_colors_lavender = 63,
		web_colors_lavenderblush = 64,
		web_colors_lawngreen = 65,
		web_colors_lemonchiffon = 66,
		web_colors_lightblue = 67,
		web_colors_lightcoral = 68,
		web_colors_lightcyan = 69,
		web_colors_lightgoldenrodyellow = 70,
		web_colors_lightgray = 71,
		web_colors_lightgrey = 72,
		web_colors_lightgreen = 73,
		web_colors_lightpink = 74,
		web_colors_lightsalmon = 75,
		web_colors_lightseagreen = 76,
		web_colors_lightskyblue = 77,
		web_colors_lightslategray = 78,
		web_colors_lightslategrey = 79,
		web_colors_lightsteelblue = 80,
		web_colors_lightyellow = 81,
		web_colors_lime = 82,
		web_colors_limegreen = 83,
		web_colors_linen = 84,
		web_colors_magenta = 85,
		web_colors_maroon = 86,
		web_colors_mediumaquamarine = 87,
		web_colors_mediumblue = 88,
		web_colors_mediumorchid = 89,
		web_colors_mediumpurple = 90,
		web_colors_mediumseagreen = 91,
		web_colors_mediumslateblue = 92,
		web_colors_mediumspringgreen = 93,
		web_colors_mediumturquoise = 94,
		web_colors_mediumvioletred = 95,
		web_colors_midnightblue = 96,
		web_colors_mintcream = 97,
		web_colors_mistyrose = 98,
		web_colors_moccasin = 99,
		web_colors_navajowhite = 100,
		web_colors_navy = 101,
		web_colors_oldlace = 102,
		web_colors_olive = 103,
		web_colors_olivedrab = 104,
		web_colors_orange = 105,
		web_colors_orangered = 106,
		web_colors_orchid = 107,
		web_colors_palegoldenrod = 108,
		web_colors_palegreen = 109,
		web_colors_paleturquoise = 110,
		web_colors_palevioletred = 111,
		web_colors_papayawhip = 112,
		web_colors_peachpuff = 113,
		web_colors_peru = 114,
		web_colors_pink = 115,
		web_colors_plum = 116,
		web_colors_powderblue = 117,
		web_colors_purple = 118,
		web_colors_red = 119,
		web_colors_rosybrown = 120,
		web_colors_royalblue = 121,
		web_colors_saddlebrown = 122,
		web_colors_salmon = 123,
		web_colors_sandybrown = 124,
		web_colors_seagreen = 125,
		web_colors_seashell = 126,
		web_colors_sienna = 127,
		web_colors_silver = 128,
		web_colors_skyblue = 129,
		web_colors_slateblue = 130,
		web_colors_slategray = 131,
		web_colors_slategrey = 132,
		web_colors_snow = 133,
		web_colors_springgreen = 134,
		web_colors_steelblue = 135,
		web_colors_tan = 136,
		web_colors_teal = 137,
		web_colors_thistle = 138,
		web_colors_tomato = 139,
		web_colors_turquoise = 140,
		web_colors_violet = 141,
		web_colors_wheat = 142,
		web_colors_white = 143,
		web_colors_whitesmoke = 144,
		web_colors_yellow = 145,
		web_colors_yellowgreen = 146,
	};



	// -------------------------------------------------

	int __get_list_style_position( const tchar_t* text, int len = -1, int def = 0 );

	enum list_style_position {
		list_style_position_inside = 0,
		list_style_position_outside = 1,
	};



	// -------------------------------------------------

	int __get_font_weight( const tchar_t* text, int len = -1, int def = 0 );

	enum font_weight {
		font_weight_normal = 0,
		font_weight_bold = 1,
		font_weight_bolder = 2,
		font_weight_lighter = 3,
		font_weight_100 = 4,
		font_weight_200 = 5,
		font_weight_300 = 6,
		font_weight_400 = 7,
		font_weight_500 = 8,
		font_weight_600 = 9,
		font_weight_700 = 10,
	};



	// -------------------------------------------------

	int __get_style_border( const tchar_t* text, int len = -1, int def = 0 );

	enum style_border {
		style_border_none = 0,
		style_border_hidden = 1,
		style_border_dotted = 2,
		style_border_dashed = 3,
		style_border_solid = 4,
		style_border_double = 5,
	};



	// -------------------------------------------------

	int __get_text_decoration( const tchar_t* text, int len = -1, int def = 0 );

	enum text_decoration {
		text_decoration_underline = 0,
		text_decoration_line_through = 1,
		text_decoration_overline = 2,
	};



	// -------------------------------------------------

	int __get_background_position( const tchar_t* text, int len = -1, int def = 0 );

	enum background_position {
		background_position_left = 0,
		background_position_right = 1,
		background_position_top, bottom = 2,
		background_position_center = 3,
	};

	enum css_value_type {
		css_value_type_null,
		css_value_type_string,
		css_value_type_int,
		css_value_type_color,
		css_value_type_css_length,
		css_value_type_media_type,
		css_value_type_atom,
		css_value_type_style_display,
		css_value_type_font_size,
		css_value_type_list_style_type,
		css_value_type_vertical_align,
		css_value_type_border_style,
		css_value_type_font_style,
		css_value_type_font_variant,
		css_value_type_border_width,
		css_value_type_element_float,
		css_value_type_element_clear,
		css_value_type_css_units,
		css_value_type_background_attachment,
		css_value_type_background_repeat,
		css_value_type_background_box,
		css_value_type_element_position,
		css_value_type_text_align,
		css_value_type_text_transform,
		css_value_type_white_space,
		css_value_type_overflow,
		css_value_type_background_size,
		css_value_type_visibility,
		css_value_type_border_collapse,
		css_value_type_pseudo_class,
		css_value_type_content_property,
		css_value_type_media_orientation,
		css_value_type_media_feature,
		css_value_type_box_sizing,
		css_value_type_web_colors,
		css_value_type_list_style_position,
		css_value_type_font_weight,
		css_value_type_style_border,
		css_value_type_text_decoration,
		css_value_type_background_position,
	};



}

#endif