#include "html.h"
#include "el_title.h"
#include "document.h"

namespace litehtml {

	el_title::el_title( document* doc )
		: html_tag( doc )
	{
	}

	el_title::~el_title()
	{
	}

	void el_title::parse_attributes()
	{
		tstring text;
		get_text( text );
		get_document()->container()->set_caption( text.c_str() );
	}
}
