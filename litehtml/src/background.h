#pragma once

#include "types.h"
#include "attributes.h"
#include "css_length.h"
#include "css_position.h"
#include "css_color.h"
#include "borders.h"

namespace litehtml
{
	/**
	 * 
	 */

	class background
	{
	public:
		xstring					m_image;
		xstring					m_baseurl;
		background_attachment	m_attachment;
		background_repeat		m_repeat;
		web_color				m_color;
		
		css_position			m_position;
		background_box			m_clip;			// border,padding,content
		background_box			m_origin;		// border,padding,content
		css_border_radius		m_radius;		

	public:
		background(void);
		background(const background& val);
		~background(void);

		background& operator=(const background& val);
	};

	/**
	 * 
	 */

	class background_paint
	{
	public:
		xstring					image;
		xstring					baseurl;
		background_attachment	attachment;
		background_repeat		repeat;
		web_color				color;

		position				clip_box;	
		position				origin_box;
		position				border_box;
		border_radiuses			border_radius;
		
		size					image_size;
		int						position_x;
		int						position_y;
		bool					is_root;

	public:
		background_paint();
		background_paint(const background_paint& val);
		void operator=(const background& val);
	};

}
