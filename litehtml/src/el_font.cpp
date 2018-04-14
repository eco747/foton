#include "html.h"
#include "el_font.h"


litehtml::el_font::el_font(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{

}

litehtml::el_font::~el_font()
{

}

void litehtml::el_font::parse_attributes()
{
	const tchar_t* str = get_attr(atom_color);
	if(str)
	{
		m_style.add_property(atom_color, str, 0, false);
	}

	str = get_attr(atom_face);
	if(str)
	{
		m_style.add_property(atom_font_face, str, 0, false);
	}

	str = get_attr(atom_size);
	if(str)
	{
		int sz = t_atoi(str);
		if(sz <= 1)
		{
			m_style.add_property(atom_font_size, _t("x-small"), 0, false);
		}
		else if(sz >= 6)
		{
			m_style.add_property(atom_font_size, _t("xx-large"), 0, false);
		}
		else
		{
			switch(sz)
			{
			case 2:
				m_style.add_property(atom_font_size, _t("small"), 0, false);
				break;
			case 3:
				m_style.add_property(atom_font_size, _t("medium"), 0, false);
				break;
			case 4:
				m_style.add_property(atom_font_size, _t("large"), 0, false);
				break;
			case 5:
				m_style.add_property(atom_font_size, _t("x-large"), 0, false);
				break;
			}
		}
	}

	html_tag::parse_attributes();
}
