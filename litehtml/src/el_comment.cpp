#include "html.h"
#include "el_comment.h"

namespace litehtml
{

	el_comment::el_comment(document* doc)
		: element(doc)
	{
		m_skip = true;
	}

	el_comment::~el_comment()
	{
	}

	void el_comment::get_text( tstring& text )
	{
		text += m_text;
	}

	void el_comment::set_data( const tchar_t* data )
	{
		if(data) {
			m_text += data;
		}
	}
}
