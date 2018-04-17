#include "html.h"
#include "el_cdata.h"

namespace litehtml
{

	el_cdata::el_cdata(document* doc)
		: element(doc)
	{
		m_skip = true;
	}

	el_cdata::~el_cdata()
	{

	}

	void el_cdata::get_text( tstring& text )
	{
		text += m_text;
	}

	void el_cdata::set_data( const tchar_t* data )
	{
		if(data) {
			m_text += data;
		}
	}
}
