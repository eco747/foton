#include "html.h"
#include "document.h"
#include "el_space.h"

namespace litehtml
{

	el_space::el_space(const tchar_t* text, document* doc)
		: el_text(text, doc)
	{
	}

	el_space::~el_space()
	{

	}

	bool el_space::is_white_space() const
	{
		white_space ws = get_white_space();
		if(	ws == white_space_normal ||
			ws == white_space_nowrap ||
			ws == white_space_pre_line )
		{
			return true;
		}
		return false;
	}

	bool el_space::is_break() const
	{
		white_space ws = get_white_space();
		if(	ws == white_space_pre ||
			ws == white_space_pre_line ||
			ws == white_space_pre_wrap)
		{
			if(m_text == _t("\n"))
			{
				return true;
			}
		}
		return false;
	}
}
