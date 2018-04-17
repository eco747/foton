#include "html.h"
#include "el_base.h"
#include "document.h"

namespace litehtml {

	el_base::el_base(document* doc)
		: html_tag(doc)
	{
	}

	el_base::~el_base()
	{
	}

	void el_base::parse_attributes()
	{
		get_document()->container()->set_base_url(get_attr(atom_href));
	}
}
