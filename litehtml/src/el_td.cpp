#include "html.h"
#include "el_td.h"


litehtml::el_td::el_td(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{

}

litehtml::el_td::~el_td()
{

}

void litehtml::el_td::parse_attributes()
{
	const tchar_t* str = get_attr(atom_width);
	if(str)
	{
		m_style.add_property(atom_width, str, 0, false);
	}
	str = get_attr(atom_background);
	if(str)
	{
		tstring url = _t("url('");
		url += str;
		url += _t("')");
		m_style.add_property(atom_background_image, url.c_str(), 0, false);
	}
	str = get_attr(atom_align);
	if(str)
	{
		m_style.add_property(atom_text_align, str, 0, false);
	}

	str = get_attr(atom_bgcolor);
	if (str)
	{
		m_style.add_property(atom_background_color, str, 0, false);
	}

	str = get_attr(atom_valign);
	if(str)
	{
		m_style.add_property(atom_vertical_align, str, 0, false);
	}
	html_tag::parse_attributes();
}

