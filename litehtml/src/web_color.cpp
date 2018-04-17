#include "html.h"
#include "web_color.h"
#include <string.h>

namespace litehtml
{
	static	int		atox( tchar_t ch ) {
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

	web_color web_color::from_string(const tchar_t* str )
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

	bool web_color::is_color(const tchar_t* str)
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
}
