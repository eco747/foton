#include "html.h"
#include "el_div.h"

namespace litehtml
{
	el_div::el_div(const std::shared_ptr<document>& doc)
		: html_tag(doc)
	{

	}

	el_div::~el_div()
	{
	}

	void el_div::parse_attributes()
	{
		//todo: align is obsolete now
		//const tchar_t* str = get_attr( atom_align );
		//if(str) {
		//	m_style.add_property(atom_text_align, str, 0, false);
		//}

		html_tag::parse_attributes();
	}
}
