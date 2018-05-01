#pragma once
#include "types.h"

namespace litehtml {
	typedef uint32_t css_color_value;

	struct	web_color {
		css_color_value	color;

		web_color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 ) {
			color = ( ( uint32_t )a ) << 24 | ( ( uint32_t )r ) << 16 | ( ( uint32_t )g ) << 8 | ( ( uint32_t )b ) << 0;
		}

		web_color( css_color_value c ) {
			color = c;
		}

		web_color() {
			color = 0xff000000;
		}

		web_color( const web_color& other ) {
			color = other.color;
		}

		web_color& operator = ( const web_color& other ) {
			color = other.color;
			return *this;
		}

		uint8_t	blue( ) const	{
			return ( color >> 0 ) & 0xff;
		}
		uint8_t	green( ) const	{
			return ( color >> 8 ) & 0xff;
		}
		uint8_t	red( ) const	{
			return ( color >> 16 ) & 0xff;
		}
		uint8_t	alpha( ) const	{
			return ( color >> 24 ) & 0xff;
		}

		static bool		from_string( const tchar_t* str, web_color* pcolor );
		static bool     is_color( const tchar_t* str );
	};
}
