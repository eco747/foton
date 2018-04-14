#include "html.h"
#include "el_table.h"
#include "document.h"
#include "iterators.h"
#include <algorithm>


litehtml::el_table::el_table(const std::shared_ptr<litehtml::document>& doc) : html_tag(doc)
{
	m_table_infos = std::unique_ptr<table_infos>(new table_infos());
	//m_table_infos->m_border_spacing_x	= 0;
	//m_table_infos->m_border_spacing_y	= 0;
	//m_table_infos->m_border_collapse	= border_collapse_separate;
}


litehtml::el_table::~el_table()
{

}

bool litehtml::el_table::appendChild(const litehtml::element::ptr& el)
{
	if(!el)	return false;

	//if(!t_strcmp(el->get_tagName(), _t("tbody")) ||
	//   !t_strcmp(el->get_tagName(), _t("thead")) ||
	//   !t_strcmp(el->get_tagName(), _t("tfoot")))
	if(	el->get_tagName()==atom_tbody ||
		el->get_tagName()==atom_thead ||
		el->get_tagName()==atom_tfoot )
	{
		return html_tag::appendChild(el);
	}

	return false;
}

void litehtml::el_table::parse_styles(bool is_reparse)
{
	html_tag::parse_styles(is_reparse);

	m_table_infos->m_border_collapse = (border_collapse) atom_index(get_style_property(atom_border_collapse, true, _t("separate")), border_collapse_separate, border_collapse_atoms );

	if(m_table_infos->m_border_collapse == border_collapse_separate)
	{
		m_table_infos->m_css_border_spacing_x.fromString(get_style_property(atom__xx_border_spacing_x, true, _t("0px")));
		m_table_infos->m_css_border_spacing_y.fromString(get_style_property(atom__xx_border_spacing_y, true, _t("0px")));

		int fntsz = get_font_size();
		document::ptr doc = get_document();
		m_table_infos->m_border_spacing_x = doc->cvt_units(m_table_infos->m_css_border_spacing_x, fntsz);
		m_table_infos->m_border_spacing_y = doc->cvt_units(m_table_infos->m_css_border_spacing_y, fntsz);
	}
	else
	{
		m_table_infos->m_border_spacing_x	= 0;
		m_table_infos->m_border_spacing_y	= 0;
		m_padding.bottom	= 0;
		m_padding.top		= 0;
		m_padding.left		= 0;
		m_padding.right		= 0;
		m_css_padding.bottom.set_value(0, css_units_px);
		m_css_padding.top.set_value(0, css_units_px);
		m_css_padding.left.set_value(0, css_units_px);
		m_css_padding.right.set_value(0, css_units_px);
	}
}

void litehtml::el_table::parse_attributes()
{
	const tchar_t* str = get_attr(atom_width);
	if(str)
	{
		m_style.add_property(atom_width, str, 0, false);
	}

	str = get_attr(atom_align);
	if(str)
	{
		int align = value_index(str, _t("left;center;right"));
		switch(align)
		{
		case 1:
			m_style.add_property(atom_margin_left, _t("auto"), 0, false);
			m_style.add_property(atom_margin_right, _t("auto"), 0, false);
			break;
		case 2:
			m_style.add_property(atom_margin_left, _t("auto"), 0, false);
			m_style.add_property(atom_margin_right, _t("0"), 0, false);
			break;
		}
	}

	str = get_attr(atom_cellspacing);
	if(str)
	{
		tstring val = str;
		val += _t(" ");
		val += str;
		m_style.add_property(atom_border_spacing, val.c_str(), 0, false);
	}
	
	str = get_attr(atom_border);
	if(str)
	{
		m_style.add_property(atom_border_width, str, 0, false);
	}

	str = get_attr(atom_bgcolor);
	if (str)
	{
		m_style.add_property(atom_background_color, str, 0, false);
	}

	html_tag::parse_attributes();
}
