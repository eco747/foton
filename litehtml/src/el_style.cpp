#include "html.h"
#include "el_style.h"
#include "document.h"

namespace litehtml
{
	el_style::el_style(const std::shared_ptr<document>& doc)
		: element(doc)
	{

	}

	el_style::~el_style()
	{

	}

	void el_style::parse_attributes()
	{
		tstring text;

		for(auto& el : m_children) {
			el->get_text(text);
		}

		get_document()->add_stylesheet( text.c_str(), 0, get_attr(atom_media) );
	}

	bool el_style::appendChild(const ptr &el)
	{
		m_children.push_back(el);
		return true;
	}

	atom el_style::get_tagName() const
	{
		return atom_style;	//_t("style");
	}
}
