#pragma once
#include "css_length.h"
#include "types.h"

namespace litehtml
{
	struct border_radiuses;

	struct css_border
	{
		css_length		width;
		web_color		color;
		uint8_t			style;	// border_style: none... outset
		
		css_border()
		{
			style = border_style_none;
		}

		css_border( const css_border& val )
		{
			width	= val.width;
			style	= val.style;
			color	= val.color;
		}

		css_border& operator = (const css_border& val)
		{
			width	= val.width;
			style	= val.style;
			color	= val.color;
			return *this;
		}
	};


	struct border_radiuses
	{
		struct 	radius {
			int 	x;
			int 	y;
		};

		radius	top_left;
		radius	top_right;
		radius 	bottom_right;
		radius 	bottom_left;

		border_radiuses()
		{
			top_left.x = 0;
			top_left.y = 0;
			top_right.x = 0;
			top_right.y = 0;
			bottom_right.x = 0;
			bottom_right.y = 0;
			bottom_left.x = 0;
			bottom_left.y = 0;
		}

		border_radiuses(const border_radiuses& val)
		{
			top_left.x = val.top_left.x;
			top_left.y = val.top_left.y;
			top_right.x = val.top_right.x;
			top_right.y = val.top_right.y;
			bottom_right.x = val.bottom_right.x;
			bottom_right.y = val.bottom_right.y;
			bottom_left.x = val.bottom_left.x;
			bottom_left.y = val.bottom_left.y;
		}

		border_radiuses& operator = (const border_radiuses& val)
		{
			top_left.x = val.top_left.x;
			top_left.y = val.top_left.y;
			top_right.x = val.top_right.x;
			top_right.y = val.top_right.y;
			bottom_right.x = val.bottom_right.x;
			bottom_right.y = val.bottom_right.y;
			bottom_left.x = val.bottom_left.x;
			bottom_left.y = val.bottom_left.y;
			return *this;
		}

		void operator += (const margins& mg)
		{
			top_left.x += mg.left;
			top_left.y += mg.top;
			top_right.x += mg.right;
			top_right.y += mg.top;
			bottom_right.x += mg.right;
			bottom_right.y += mg.bottom;
			bottom_left.x += mg.left;
			bottom_left.y += mg.bottom;
			fix_values();
		}

		void operator -= (const margins& mg)
		{
			top_left.x -= mg.left;
			top_left.y -= mg.top;
			top_right.x -= mg.right;
			top_right.y -= mg.top;
			bottom_right.x -= mg.right;
			bottom_right.y -= mg.bottom;
			bottom_left.x -= mg.left;
			bottom_left.y -= mg.bottom;
			fix_values();
		}

		void fix_values()
		{
			if (top_left.x < 0)		top_left.x = 0;
			if (top_left.y < 0)		top_left.y = 0;
			if (top_right.x < 0) 	top_right.x = 0;
			if (bottom_right.x < 0) bottom_right.x = 0;
			if (bottom_right.y < 0) bottom_right.y = 0;
			if (bottom_left.x < 0) 	bottom_left.x = 0;
			if (bottom_left.y < 0) 	bottom_left.y = 0;
		}
	};


	struct css_border_radius
	{
		struct	radius
		{
			css_length	x;
			css_length 	y;
		};

		radius	top_left;
		radius	top_right;
		radius	bottom_right;
		radius	bottom_left;
		
		css_border_radius()
		{
		}

		css_border_radius( const css_border_radius& val )
		{
			top_left.x		= val.top_left.x;
			top_left.y		= val.top_left.y;
			top_right.x		= val.top_right.x;
			top_right.y		= val.top_right.y;
			bottom_left.x	= val.bottom_left.x;
			bottom_left.y	= val.bottom_left.y;
			bottom_right.x	= val.bottom_right.x;
			bottom_right.y	= val.bottom_right.y;
		}

		css_border_radius& operator = (const css_border_radius& val)
		{
			top_left.x		= val.top_left.x;
			top_left.y		= val.top_left.y;
			top_right.x		= val.top_right.x;
			top_right.y		= val.top_right.y;
			bottom_left.x	= val.bottom_left.x;
			bottom_left.y	= val.bottom_left.y;
			bottom_right.x	= val.bottom_right.x;
			bottom_right.y	= val.bottom_right.y;
			return *this;
		}

		border_radiuses calc_percents(int width, int height)
		{
			border_radiuses ret;
			ret.bottom_left.x 	= bottom_left.x.calc_percent(width);
			ret.bottom_left.y 	= bottom_left.y.calc_percent(height);
			ret.top_left.x 		= top_left.x.calc_percent(width);
			ret.top_left.y 		= top_left.y.calc_percent(height);
			ret.top_right.x 	= top_right.x.calc_percent(width);
			ret.top_right.y 	= top_right.y.calc_percent(height);
			ret.bottom_right.x 	= bottom_right.x.calc_percent(width);
			ret.bottom_right.y 	= bottom_right.y.calc_percent(height);
			return ret;
		}
	};

	struct css_borders
	{
		css_border			left;
		css_border			top;
		css_border			right;
		css_border			bottom;
		css_border_radius	radius;

		css_borders()
		{
		}

		css_borders(const css_borders& val)
		{
			left	= val.left;
			right	= val.right;
			top		= val.top;
			bottom	= val.bottom;
			radius	= val.radius;
		}

		css_borders& operator=(const css_borders& val)
		{
			left	= val.left;
			right	= val.right;
			top		= val.top;
			bottom	= val.bottom;
			radius	= val.radius;
			return *this;
		}
	};











	struct border
	{
		int				width;
		border_style	style;
		web_color		color;

		border()
		{
			width = 0;
		}

		border(const border& val)
		{
			width = val.width;
			style = val.style;
			color = val.color;
		}

		border(const css_border& val)
		{
			width = (int)val.width.value();
			style = (border_style)val.style;
			color = val.color;
		}

		border& operator=(const border& val)
		{
			width = val.width;
			style = val.style;
			color = val.color;
			return *this;
		}

		border& operator=(const css_border& val)
		{
			width = (int) val.width.value();
			style = (border_style)val.style;
			color = val.color;
			return *this;
		}
	};

	struct borders
	{
		border			left;
		border			top;
		border			right;
		border			bottom;
		border_radiuses	radius;

		borders()
		{
		}

		borders(const borders& val)
		{
			left 	= val.left;
			right 	= val.right;
			top 	= val.top;
			bottom 	= val.bottom;
			radius 	= val.radius;
		}

		borders(const css_borders& val)
		{
			left 	= val.left;
			right 	= val.right;
			top 	= val.top;
			bottom 	= val.bottom;
		}

		borders& operator=(const borders& val)
		{
			left 	= val.left;
			right 	= val.right;
			top 	= val.top;
			bottom 	= val.bottom;
			radius 	= val.radius;
			return *this;
		}

		borders& operator=(const css_borders& val)
		{
			left 	= val.left;
			right 	= val.right;
			top 	= val.top;
			bottom 	= val.bottom;
			return *this;
		}
	};
}
