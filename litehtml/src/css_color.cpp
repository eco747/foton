#include "html.h"
#include "css_color.h"
#include <string.h>

namespace litehtml {
	static uint32_t color_values[] = {
		/* transparent */ 0,
		/* AliceBlue */ 0xffF0F8FF,
		/* AntiqueWhite */ 0xffFAEBD7,
		/* Aqua */ 0xff00FFFF,
		/* Aquamarine */ 0xff7FFFD4,
		/* Azure */ 0xffF0FFFF,
		/* Beige */ 0xffF5F5DC,
		/* Bisque */ 0xffFFE4C4,
		/* Black */ 0xff000000,
		/* BlanchedAlmond */ 0xffFFEBCD,
		/* Blue */ 0xff0000FF,
		/* BlueViolet */ 0xff8A2BE2,
		/* Brown */ 0xffA52A2A,
		/* BurlyWood */ 0xffDEB887,
		/* CadetBlue */ 0xff5F9EA0,
		/* Chartreuse */ 0xff7FFF00,
		/* Chocolate */ 0xffD2691E,
		/* Coral */ 0xffFF7F50,
		/* CornflowerBlue */ 0xff6495ED,
		/* Cornsilk */ 0xffFFF8DC,
		/* Crimson */ 0xffDC143C,
		/* Cyan */ 0xff00FFFF,
		/* DarkBlue */ 0xff00008B,
		/* DarkCyan */ 0xff008B8B,
		/* DarkGoldenRod */ 0xffB8860B,
		/* DarkGray */ 0xffA9A9A9,
		/* DarkGrey */ 0xffA9A9A9,
		/* DarkGreen */ 0xff006400,
		/* DarkKhaki */ 0xffBDB76B,
		/* DarkMagenta */ 0xff8B008B,
		/* DarkOliveGreen */ 0xff556B2F,
		/* Darkorange */ 0xffFF8C00,
		/* DarkOrchid */ 0xff9932CC,
		/* DarkRed */ 0xff8B0000,
		/* DarkSalmon */ 0xffE9967A,
		/* DarkSeaGreen */ 0xff8FBC8F,
		/* DarkSlateBlue */ 0xff483D8B,
		/* DarkSlateGray */ 0xff2F4F4F,
		/* DarkSlateGrey */ 0xff2F4F4F,
		/* DarkTurquoise */ 0xff00CED1,
		/* DarkViolet */ 0xff9400D3,
		/* DeepPink */ 0xffFF1493,
		/* DeepSkyBlue */ 0xff00BFFF,
		/* DimGray */ 0xff696969,
		/* DimGrey */ 0xff696969,
		/* DodgerBlue */ 0xff1E90FF,
		/* FireBrick */ 0xffB22222,
		/* FloralWhite */ 0xffFFFAF0,
		/* ForestGreen */ 0xff228B22,
		/* Fuchsia */ 0xffFF00FF,
		/* Gainsboro */ 0xffDCDCDC,
		/* GhostWhite */ 0xffF8F8FF,
		/* Gold */ 0xffFFD700,
		/* GoldenRod */ 0xffDAA520,
		/* Gray */ 0xff808080,
		/* Grey */ 0xff808080,
		/* Green */ 0xff008000,
		/* GreenYellow */ 0xffADFF2F,
		/* HoneyDew */ 0xffF0FFF0,
		/* HotPink */ 0xffFF69B4,
		/* Ivory */ 0xffFFFFF0,
		/* Khaki */ 0xffF0E68C,
		/* Lavender */ 0xffE6E6FA,
		/* LavenderBlush */ 0xffFFF0F5,
		/* LawnGreen */ 0xff7CFC00,
		/* LemonChiffon */ 0xffFFFACD,
		/* LightBlue */ 0xffADD8E6,
		/* LightCoral */ 0xffF08080,
		/* LightCyan */ 0xffE0FFFF,
		/* LightGoldenRodYellow */ 0xffFAFAD2,
		/* LightGray */ 0xffD3D3D3,
		/* LightGrey */ 0xffD3D3D3,
		/* LightGreen */ 0xff90EE90,
		/* LightPink */ 0xffFFB6C1,
		/* LightSalmon */ 0xffFFA07A,
		/* LightSeaGreen */ 0xff20B2AA,
		/* LightSkyBlue */ 0xff87CEFA,
		/* LightSlateGray */ 0xff778899,
		/* LightSlateGrey */ 0xff778899,
		/* LightSteelBlue */ 0xffB0C4DE,
		/* LightYellow */ 0xffFFFFE0,
		/* Lime */ 0xff00FF00,
		/* LimeGreen */ 0xff32CD32,
		/* Linen */ 0xffFAF0E6,
		/* Magenta */ 0xffFF00FF,
		/* Maroon */ 0xff800000,
		/* MediumAquaMarine */ 0xff66CDAA,
		/* MediumBlue */ 0xff0000CD,
		/* MediumOrchid */ 0xffBA55D3,
		/* MediumPurple */ 0xff9370D8,
		/* MediumSeaGreen */ 0xff3CB371,
		/* MediumSlateBlue */ 0xff7B68EE,
		/* MediumSpringGreen */ 0xff00FA9A,
		/* MediumTurquoise */ 0xff48D1CC,
		/* MediumVioletRed */ 0xffC71585,
		/* MidnightBlue */ 0xff191970,
		/* MintCream */ 0xffF5FFFA,
		/* MistyRose */ 0xffFFE4E1,
		/* Moccasin */ 0xffFFE4B5,
		/* NavajoWhite */ 0xffFFDEAD,
		/* Navy */ 0xff000080,
		/* OldLace */ 0xffFDF5E6,
		/* Olive */ 0xff808000,
		/* OliveDrab */ 0xff6B8E23,
		/* Orange */ 0xffFFA500,
		/* OrangeRed */ 0xffFF4500,
		/* Orchid */ 0xffDA70D6,
		/* PaleGoldenRod */ 0xffEEE8AA,
		/* PaleGreen */ 0xff98FB98,
		/* PaleTurquoise */ 0xffAFEEEE,
		/* PaleVioletRed */ 0xffD87093,
		/* PapayaWhip */ 0xffFFEFD5,
		/* PeachPuff */ 0xffFFDAB9,
		/* Peru */ 0xffCD853F,
		/* Pink */ 0xffFFC0CB,
		/* Plum */ 0xffDDA0DD,
		/* PowderBlue */ 0xffB0E0E6,
		/* Purple */ 0xff800080,
		/* Red */ 0xffFF0000,
		/* RosyBrown */ 0xffBC8F8F,
		/* RoyalBlue */ 0xff4169E1,
		/* SaddleBrown */ 0xff8B4513,
		/* Salmon */ 0xffFA8072,
		/* SandyBrown */ 0xffF4A460,
		/* SeaGreen */ 0xff2E8B57,
		/* SeaShell */ 0xffFFF5EE,
		/* Sienna */ 0xffA0522D,
		/* Silver */ 0xffC0C0C0,
		/* SkyBlue */ 0xff87CEEB,
		/* SlateBlue */ 0xff6A5ACD,
		/* SlateGray */ 0xff708090,
		/* SlateGrey */ 0xff708090,
		/* Snow */ 0xffFFFAFA,
		/* SpringGreen */ 0xff00FF7F,
		/* SteelBlue */ 0xff4682B4,
		/* Tan */ 0xffD2B48C,
		/* Teal */ 0xff008080,
		/* Thistle */ 0xffD8BFD8,
		/* Tomato */ 0xffFF6347,
		/* Turquoise */ 0xff40E0D0,
		/* Violet */ 0xffEE82EE,
		/* Wheat */ 0xffF5DEB3,
		/* White */ 0xffFFFFFF,
		/* WhiteSmoke */ 0xffF5F5F5,
		/* Yellow */ 0xffFFFF00,
		/* YellowGreen */ 0xff9ACD32,
	};


	static	int		atox( tchar_t ch )
	{
		if( ch >= '0' && ch <= '9' ) {
			return ch - '0';
		}

		if( ch >= 'a' && ch <= 'f' ) {
			return ch - 'a' + 10;
		}

		if( ch >= 'A' && ch <= 'F' ) {
			return ch - 'A' + 10;
		}

		__asm int 3;
		return 0;
	}

	bool	web_color::from_string( const xstringpart& str, web_color* pcolor )
	{
		if( !str.len )	{
			*pcolor = web_color( 0xff000000 );
			return false;
		}

		const tchar_t*	p = str.start();
		const tchar_t*	e = str.end();

		if( *p == '#' ) {
			p++;

			if( str.len == 4 ) {
				*pcolor = 	web_color(	atox( p[0] ),
										atox( p[1] ),
										atox( p[2] ) );
				return true;
			}
			else if( str.len == 7 ) {
				*pcolor =  	web_color(	atox( p[0] ) << 4 | atox( p[1] ),
										atox( p[2] ) << 4 | atox( p[3] ),
										atox( p[4] ) << 4 | atox( p[5] ) );
				return true;
			}
			else if( str.len == 9 ) {
				*pcolor = 	web_color(	atox( p[0] ) << 4 | atox( p[1] ),
										atox( p[2] ) << 4 | atox( p[3] ),
										atox( p[4] ) << 4 | atox( p[5] ),
										atox( p[6] ) << 4 | atox( p[7] ) );
				return true;
			}

			//__asm int 3;
			//todo: malformed color
			*pcolor = web_color( 0xff000000 );
			return false;
		}
		else if( ( p[0] == 'r' || p[0] == 'R' ) &&
				 ( p[1] == 'g' || p[1] == 'G' ) &&
				 ( p[2] == 'b' || p[2] == 'B' ) ) {
			bool  rgba = false;

			if( p[3] == 'a' || p[3] == 'A' ) {
				p = skip_sp( p + 4 );
				rgba = true;
			}
			else {
				p = skip_sp( p + 3 );
			}

			if( *p == '(' ) {
				uint8_t		clr[4] = {0, 0, 0, 0xff};
				//	red
				clr[0] = ( uint8_t )atoi( skip_sp( p + 1 ) );

				while( *p && *p != ',' ) {
					p++;
				}

				if( *p == ',' ) {
					//	green
					clr[1] = ( uint8_t )atoi( skip_sp( p + 1 ) );

					while( *p && *p != ',' ) {
						p++;
					}

					if( *p == ',' ) {
						//	blue
						clr[2] = ( uint8_t )atoi( skip_sp( p + 1 ) );

						if( rgba ) {
							//	alpha
							clr[3] = ( uint8_t )atoi( skip_sp( p + 1 ) );
						}

						while( *p && *p != ')' ) {
							p++;
						}

						if( *p == ')' ) {
							*pcolor = web_color( clr[0], clr[1], clr[2], clr[3] );
							return true;
						}
					}
				}
			}

			//todo: warning bad color
			*pcolor = web_color( 0xff000000 );
			return false;
		}

		int idx = __get_web_colors( str, -1, -1 );

		if( idx >= 0 ) {
			*pcolor = web_color( color_values [idx] );
			return true;
		}

		//todo: handle initial
		*pcolor = web_color( 0xff000000 );
		return false;
	}

	bool web_color::is_color( const tchar_t* str )
	{
		if( *str == '#' ) {
			return true;
		}

		//todo: best
		if( ( str[0] == 'r' || str[0] == 'R' ) &&
				( str[1] == 'g' || str[1] == 'G' ) &&
				( str[2] == 'b' || str[2] == 'B' ) ) {
			return true;
		}

		if( __get_web_colors( str, -1, -1 ) >= 0 ) {
			return true;
		}

		//	todo: gradients (linear-gradient)
		/*
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
		*/
		return false;
	}



}
