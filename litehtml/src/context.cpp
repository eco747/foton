#include "html.h"
#include "context.h"
#include "stylesheet.h"


namespace litehtml {


	void context::load_master_stylesheet( const tchar_t* str )
	{
		media_query_list::ptr media;

		m_master_css.parse_stylesheet(str, 0, new document(), media_query_list::ptr());
		m_master_css.sort_selectors();
	}

}
