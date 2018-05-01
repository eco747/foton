#include "html.h"
#include "el_script.h"
#include "document.h"

namespace litehtml {

	el_script::el_script( document* doc )
		: element( doc )
	{
	}

	el_script::~el_script()
	{
	}

	void el_script::parse_attributes()
	{
		//TODO: pass script text to document container
	}

	bool el_script::appendChild( const ptr& el )
	{
		el->get_text( m_text );
		return true;
	}

	atom el_script::get_tagName() const
	{
		return atom_create( "script" );
	}
}
