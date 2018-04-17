#pragma once
#include "html_tag.h"

namespace litehtml
{
	class el_before_after_base
		: public html_tag
	{
	public:
		el_before_after_base( document* doc, bool before);
		virtual ~el_before_after_base();

		virtual void add_style(const litehtml::style& st) override;
		virtual void apply_stylesheet(const litehtml::css& stylesheet) override;
	private:
		void	add_text(const char* txt, int len=-1 );
		void	add_function(const tstring& fnc, const tstring& params);
		tchar_t convert_escape(const tchar_t* txt);
	};

	class el_before 
		: public el_before_after_base
	{
	public:
		el_before(document* doc) 
			: el_before_after_base(doc, true)
		{

		}
	};

	class el_after 
		: public el_before_after_base
	{
	public:
		el_after( document* doc) 
			: el_before_after_base(doc, false)
		{

		}
	};
}
