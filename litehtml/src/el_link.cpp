#include "html.h"
#include "el_link.h"
#include "document.h"

namespace litehtml {

	el_link::el_link( document* doc )
		: html_tag( doc )
	{
	}

	el_link::~el_link()
	{
	}

	void el_link::parse_attributes()
	{
		bool processed = false;
		document::ptr doc = get_document();
		const tchar_t* rel = get_attr( atom_rel );

		if( rel && !t_strcmp( rel, _t( "stylesheet" ) ) ) {
			const tchar_t* media	= get_attr( atom_media );
			const tchar_t* href		= get_attr( atom_href );

			if( href && href[0] ) {
				xstring css_text;
				xstring css_baseurl;
				doc->container()->import_css( css_text, href, css_baseurl );

				if( !css_text.empty() ) {
					doc->add_stylesheet( css_text.c_str(), css_baseurl.c_str(), media );
					processed = true;
				}
			}
		}

		if( !processed ) {
			doc->container()->link( doc, shared_from_this() );
		}
	}
}
