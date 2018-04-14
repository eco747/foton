#pragma once

namespace litehtml
{
	class document_container;

	// todo: grab uint32_t directly

	struct web_color
	{
		uint32_t	color;

		web_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		{
			color = ((uint32_t)a)<<24 | ((uint32_t)r)<<16 | ((uint32_t)g)<<8 | ((uint32_t)b)<<0;
		}

		web_color( uint32_t c )
		{
			color = c;
		}

		web_color()
		{
			color = 0xff000000;
		}

		web_color(const web_color& other)
		{
			color = other.color;
		}

		web_color& operator=(const web_color& other)
		{
			color = other.color;
			return *this;
		}

		uint8_t	blue( ) const	{ return (color>>0)&0xff; }
		uint8_t	green( ) const	{ return (color>>8)&0xff; }
		uint8_t	red( ) const	{ return (color>>16)&0xff; }
		uint8_t	alpha( ) const	{ return (color>>24)&0xff; }

		static web_color	from_string(const tchar_t* str, litehtml::document_container* callback);
		static bool         is_color(const tchar_t* str);
	};

	// todo: optimize: hash
	struct def_color
	{
		const tchar_t*	name;
		uint32_t		color;
	};

	extern def_color g_def_colors[];
}
