#pragma once
#include "style.h"
#include "css_selector.h"

namespace litehtml
{
	class document_container;

	class css
	{
		xVector<css_selector>	m_selectors;

	public:
		css()
		{
		}
		
		~css()
		{
			m_selectors.clear( );
		}

		const xVector<css_selector>& selectors() const
		{
			return m_selectors;
		}

		void clear()
		{
			m_selectors.clear();
		}

		void	parse_stylesheet(const tchar_t* str, const tchar_t* baseurl, const document* doc, const media_query_list::ptr& media);
		void	sort_selectors();
		static void	parse_css_url(const xstring& str, xstring& url);

	private:
		void	parse_atrule(const tchar_t* text, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media);
		bool	parse_selectors(const tstring& txt, const style* styles, const media_query_list::ptr& media);

		void	add_selector(css_selector* selector)
		{
			selector->m_order = (int)m_selectors.length();
			m_selectors.add( selector );
		}
	};
}
