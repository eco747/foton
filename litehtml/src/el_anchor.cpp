#include "html.h"
#include "el_anchor.h"
#include "document.h"

namespace litehtml
{

	el_anchor::el_anchor(document* doc)
		: html_tag(doc)
	{
	}

	el_anchor::~el_anchor()
	{
	}

	void el_anchor::on_click()
	{
		const tchar_t* href = get_attr( atom_href );

		if(href) {
			get_document()->container()->on_anchor_click(href, this);
		}
	}

	void el_anchor::apply_stylesheet( const litehtml::css& stylesheet )
	{
		if( get_attr(atom_href) ) {
			m_pseudo_classes.push_back(_t("link"));
		}

		html_tag::apply_stylesheet(stylesheet);
	}
}
