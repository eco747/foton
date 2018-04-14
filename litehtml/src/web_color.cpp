#include "html.h"
#include "web_color.h"
#include <string.h>

#define wc	litehtml::web_color

litehtml::def_color litehtml::g_def_colors[] = 
{
	{_t("transparent"),			0x00FFFFFF },
	{_t("AliceBlue"),			0xFFF0F8FF },
	{_t("AntiqueWhite"),		0xFFFAEBD7 },
	{_t("Aqua"),				0xFF00FFFF },
	{_t("Aquamarine"),			0xFF7FFFD4 },
	{_t("Azure"),				0xFFF0FFFF },
	{_t("Beige"),				0xFFF5F5DC },
	{_t("Bisque"),				0xFFFFE4C4 },
	{_t("Black"),		        0xFF000000 },
	{_t("BlanchedAlmond"),      0xFFFFEBCD },
	{_t("Blue"),		        0xFF0000FF },
	{_t("BlueViolet"),	        0xFF8A2BE2 },
	{_t("Brown"),		        0xFFA52A2A },
	{_t("BurlyWood"),	        0xFFDEB887 },
	{_t("CadetBlue"),	        0xFF5F9EA0 },
	{_t("Chartreuse"),	        0xFF7FFF00 },
	{_t("Chocolate"),	        0xFFD2691E },
	{_t("Coral"),		        0xFFFF7F50 },
	{_t("CornflowerBlue"),      0xFF6495ED },
	{_t("Cornsilk"),	        0xFFFFF8DC },
	{_t("Crimson"),		        0xFFDC143C },
	{_t("Cyan"),		        0xFF00FFFF },
	{_t("DarkBlue"),	        0xFF00008B },
	{_t("DarkCyan"),	        0xFF008B8B },
	{_t("DarkGoldenRod"),       0xFFB8860B },
	{_t("DarkGray"),	        0xFFA9A9A9 },
	{_t("DarkGrey"),	        0xFFA9A9A9 },
	{_t("DarkGreen"),	        0xFF006400 },
	{_t("DarkKhaki"),	        0xFFBDB76B },
	{_t("DarkMagenta"),	        0xFF8B008B },
	{_t("DarkOliveGreen"),      0xFF556B2F },
	{_t("Darkorange"),	        0xFFFF8C00 },
	{_t("DarkOrchid"),	        0xFF9932CC },
	{_t("DarkRed"),		        0xFF8B0000 },
	{_t("DarkSalmon"),	        0xFFE9967A },
	{_t("DarkSeaGreen"),        0xFF8FBC8F },
	{_t("DarkSlateBlue"),       0xFF483D8B },
	{_t("DarkSlateGray"),       0xFF2F4F4F },
	{_t("DarkSlateGrey"),       0xFF2F4F4F },
	{_t("DarkTurquoise"),       0xFF00CED1 },
	{_t("DarkViolet"),	        0xFF9400D3 },
	{_t("DeepPink"),	        0xFFFF1493 },
	{_t("DeepSkyBlue"),	        0xFF00BFFF },
	{_t("DimGray"),		        0xFF696969 },
	{_t("DimGrey"),		        0xFF696969 },
	{_t("DodgerBlue"),	        0xFF1E90FF },
	{_t("FireBrick"),	        0xFFB22222 },
	{_t("FloralWhite"),	        0xFFFFFAF0 },
	{_t("ForestGreen"),	        0xFF228B22 },
	{_t("Fuchsia"),		        0xFFFF00FF },
	{_t("Gainsboro"),	        0xFFDCDCDC },
	{_t("GhostWhite"),	        0xFFF8F8FF },
	{_t("Gold"),		        0xFFFFD700 },
	{_t("GoldenRod"),	        0xFFDAA520 },
	{_t("Gray"),		        0xFF808080 },
	{_t("Grey"),		        0xFF808080 },
	{_t("Green"),		        0xFF008000 },
	{_t("GreenYellow"),	        0xFFADFF2F },
	{_t("HoneyDew"),	        0xFFF0FFF0 },
	{_t("HotPink"),		        0xFFFF69B4 },
	{_t("Ivory"),		        0xFFFFFFF0 },
	{_t("Khaki"),		        0xFFF0E68C },
	{_t("Lavender"),	        0xFFE6E6FA },
	{_t("LavenderBlush"),       0xFFFFF0F5 },
	{_t("LawnGreen"),	        0xFF7CFC00 },
	{_t("LemonChiffon"),        0xFFFFFACD },
	{_t("LightBlue"),	        0xFFADD8E6 },
	{_t("LightCoral"),	        0xFFF08080 },
	{_t("LightCyan"),	        0xFFE0FFFF },
	{_t("LightGoldenRodYellow"),0xFFFAFAD2 },
	{_t("LightGray"),			0xFFD3D3D3 },
	{_t("LightGrey"),	        0xFFD3D3D3 },
	{_t("LightGreen"),	        0xFF90EE90 },
	{_t("LightPink"),	        0xFFFFB6C1 },
	{_t("LightSalmon"),	        0xFFFFA07A },
	{_t("LightSeaGreen"),       0xFF20B2AA },
	{_t("LightSkyBlue"),        0xFF87CEFA },
	{_t("LightSlateGray"),      0xFF778899 },
	{_t("LightSlateGrey"),      0xFF778899 },
	{_t("LightSteelBlue"),		0xFFB0C4DE },
	{_t("LightYellow"),			0xFFFFFFE0 },
	{_t("Lime"),				0xFF00FF00 },
	{_t("LimeGreen"),			0xFF32CD32 },
	{_t("Linen"),				0xFFFAF0E6 },
	{_t("Magenta"),				0xFFFF00FF },
	{_t("Maroon"),				0xFF800000 },
	{_t("MediumAquaMarine"),	0xFF66CDAA },
	{_t("MediumBlue"),			0xFF0000CD },
	{_t("MediumOrchid"),		0xFFBA55D3 },
	{_t("MediumPurple"),		0xFF9370D8 },
	{_t("MediumSeaGreen"),		0xFF3CB371 },
	{_t("MediumSlateBlue"),		0xFF7B68EE },
	{_t("MediumSpringGreen"),   0xFF00FA9A },
	{_t("MediumTurquoise"),		0xFF48D1CC },
	{_t("MediumVioletRed"),		0xFFC71585 },
	{_t("MidnightBlue"),		0xFF191970 },
	{_t("MintCream"),			0xFFF5FFFA },
	{_t("MistyRose"),			0xFFFFE4E1 },
	{_t("Moccasin"),			0xFFFFE4B5 },
	{_t("NavajoWhite"),			0xFFFFDEAD },
	{_t("Navy"),				0xFF000080 },
	{_t("OldLace"),				0xFFFDF5E6 },
	{_t("Olive"),				0xFF808000 },
	{_t("OliveDrab"),			0xFF6B8E23 },
	{_t("Orange"),				0xFFFFA500 },
	{_t("OrangeRed"),			0xFFFF4500 },
	{_t("Orchid"),				0xFFDA70D6 },
	{_t("PaleGoldenRod"),       0xFFEEE8AA },
	{_t("PaleGreen"),	        0xFF98FB98 },
	{_t("PaleTurquoise"),       0xFFAFEEEE },
	{_t("PaleVioletRed"),       0xFFD87093 },
	{_t("PapayaWhip"),	        0xFFFFEFD5 },
	{_t("PeachPuff"),	        0xFFFFDAB9 },
	{_t("Peru"),				0xFFCD853F },
	{_t("Pink"),				0xFFFFC0CB },
	{_t("Plum"),				0xFFDDA0DD },
	{_t("PowderBlue"),	        0xFFB0E0E6 },
	{_t("Purple"),				0xFF800080 },
	{_t("Red"),					0xFFFF0000 },
	{_t("RosyBrown"),	        0xFFBC8F8F },
	{_t("RoyalBlue"),	        0xFF4169E1 },
	{_t("SaddleBrown"),	        0xFF8B4513 },
	{_t("Salmon"),				0xFFFA8072 },
	{_t("SandyBrown"),	        0xFFF4A460 },
	{_t("SeaGreen"),			0xFF2E8B57 },
	{_t("SeaShell"),			0xFFFFF5EE },
	{_t("Sienna"),				0xFFA0522D },
	{_t("Silver"),				0xFFC0C0C0 },
	{_t("SkyBlue"),				0xFF87CEEB },
	{_t("SlateBlue"),	        0xFF6A5ACD },
	{_t("SlateGray"),	        0xFF708090 },
	{_t("SlateGrey"),	        0xFF708090 },
	{_t("Snow"),				0xFFFFFAFA },
	{_t("SpringGreen"),	        0xFF00FF7F },
	{_t("SteelBlue"),	        0xFF4682B4 },
	{_t("Tan"),					0xFFD2B48C },
	{_t("Teal"),				0xFF008080 },
	{_t("Thistle"),				0xFFD8BFD8 },
	{_t("Tomato"),				0xFFFF6347 },
	{_t("Turquoise"),			0xFF40E0D0 },
	{_t("Violet"),				0xFFEE82EE },
	{_t("Wheat"),				0xFFF5DEB3 },
	{_t("White"),				0xFFFFFFFF },
	{_t("WhiteSmoke"),			0xFFF5F5F5 },
	{_t("Yellow"),				0xFFFFFF00 },
	{_t("YellowGreen"),			0xFF9ACD32 },
	{0,0}
};


static	int		atox( litehtml::tchar_t ch ) {
	if( ch>='0' && ch<='9' ) {
		return ch-'0';
	}

	if( ch>='a' && ch<='f' ) {
		return ch-'a'+10;
	}

	if( ch>='A' && ch<='F' ) {
		return ch-'A'+10;
	}

	__asm int 3;
	return 0;
}

//<todo : remove callback param
litehtml::web_color litehtml::web_color::from_string(const tchar_t* str, litehtml::document_container* /*callback*/)
{
	if(!str || !str[0])	{
		return web_color(0xff000000);
	}

	if( str[0] == '#' )
	{
		str++;

		int	len = t_strlen(str);
		if( len==3 ) {
			return web_color( atox(str[0]), atox(str[1]), atox(str[2]) );
		}
		else if( len==6 ) {
			return web_color(	atox(str[0])<<4 | atox(str[1]),
								atox(str[2])<<4 | atox(str[3]),
								atox(str[4])<<4 | atox(str[5]) );
		}
		else if( len==8 ) {
			return web_color(	atox(str[0])<<4 | atox(str[1]),
								atox(str[2])<<4 | atox(str[3]),
								atox(str[4])<<4 | atox(str[5]),
								atox(str[6])<<4 | atox(str[7]) );
		}
		else {
			__asm int 3;
		}

		return web_color( );
	}
	else if(!t_strncmp(str, _t("rgb"), 3))
	{
		__asm int 3;
		/*
		tstring s = str;

		tstring::size_type pos = s.find_first_of(_t("("));
		if(pos != tstring::npos)
		{
			s.erase(s.begin(), s.begin() + pos + 1);
		}
		pos = s.find_last_of(_t(")"));
		if(pos != tstring::npos)
		{
			s.erase(s.begin() + pos, s.end());
		}

		std::vector<tstring> tokens;
		split_string(s, tokens, _t(", \t"));

		web_color clr;

		if(tokens.size() >= 1)	clr.red		= (byte) t_atoi(tokens[0].c_str());
		if(tokens.size() >= 2)	clr.green	= (byte) t_atoi(tokens[1].c_str());
		if(tokens.size() >= 3)	clr.blue	= (byte) t_atoi(tokens[2].c_str());
		if(tokens.size() >= 4)	clr.alpha	= (byte) (t_strtod(tokens[3].c_str(), 0) * 255.0);

		return clr;
		*/
	}
	else if( atom_create(str,false)==atom_transparent ) {
		return web_color(0,0,0,0);
	}
	else {
		//	todo: optimization -> perfect hash on color names
		//	todo: gradients (linear-gradient)
		for(int i=0; g_def_colors[i].name; i++)
		{
			if(!t_strcasecmp(str, g_def_colors[i].name))
				return web_color( g_def_colors[i].color );
		}
	}

	//todo: handle initial
	return web_color(0, 0, 0);
}

bool litehtml::web_color::is_color(const tchar_t* str)
{
	if( *str=='#' ) {
		return true;
	}

	if( (str[0]=='r' || str[0]=='R') &&
		(str[1]=='g' || str[1]=='G') &&
		(str[2]=='b' || str[2]=='B') ) {
		return true;
	}

	//	todo: real color check optimization -> perfect hash on color names
	//	todo: gradients (linear-gradient)

	switch( *str ) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
			return false;
	}

	return true;
}
