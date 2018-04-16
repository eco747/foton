#include "html.h"
#include "stylesheet.h"
#include <algorithm>
#include "document.h"

namespace litehtml
{
	/**
	 * @brief extract_string
	 * @param text - must be on the separator
	 * @param result
	 * @return next text position
	 */

	const tchar_t* extract_string( const tchar_t* text, xstring& result ) {

		tchar_t	sep = *text++;
		const tchar_t* start = text;

		while( *text && *text!=sep ) {
			text++;
		}

		if( *text==sep ) {
			result.set( start, text-start );
			text++;
		}
		else {
			//todo: warning unterminated string
		}

		return text;
	}

	/**
	 * @brief extract_url
	 * @param text
	 * @param result
	 * @return
	 */

	const tchar_t* extract_url( const tchar_t* text, xstring& result ) {

		// direct string
		if( *text=='\'' || *text=='"' ) {
			return extract_string( text, result );
		}

		// url( ... )
		if( t_strncmp(text,"url",3)==0 ) {
			text = skip_sp( text+3 );

			if( *text=='(' ) {
				text = skip_sp( text+1 );

				// string
				if( *text=='\'' || *text=='"' ) {
					text = extract_string( text, result );

					// skip to ')'
					while( *text && *text!=')' ) {
						text++;
					}

					if( *text==')' ) {
						text;;
					}

					return text;
				}

				// direct value
				const tchar_t* start = text;

				while( *text && *text!=')' ) {
					text++;
				}

				if( *text==')' ) {
					const tchar_t* q = text-1;
					while( q>start && (*q==' ' || *q=='\t') ) {
						q--;
					}

					result.set( start, q );
					return text;
				}
				else {
					//todo: warning malformed url
				}
			}
			else {
				//todo: warning malformed url
			}
		}
		else {
			//todo: warning malformed url
		}

		result.clear();
		return text;
	}




	/**
	 * @brief css::parse_stylesheet
	 * @param str
	 * @param baseurl
	 * @param doc
	 * @param media
	 */

	void css::parse_stylesheet( const tchar_t* str, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media )
	{
		//todo: memory optim, think about not creating xstring but using directly str
		xstring		text( str );

		bool			cmt = false;
		const tchar_t*	p = text.c_str( );

		while( *p )
		{
			p = skip_sp( p );
			if( *p==0 ) {
				break;
			}

			if( !cmt ) {

				//	start of comment
				if( *p=='/' && p[1]=='*' ) {
					cmt = true;
					p += 2;
					continue;
				}

				//  @import
				//	@media ...

				if( *p=='@' ) {
					p = parse_at_rule( p, baseurl, doc, media);
					continue;
				}

				// standard rule

				//	selector
				const tchar_t* sel_p = p;
				while( *p && *p!='{ ') {
					p++;
				}

				//	rule
				if( *p=='{' ) {

					xstring		selector( sel_p, p-sel_p );

					const tchar_t* rule_p = ++p;
					while( *p && *p!='}' ) {
						p++;
					}

					if( *p=='}' ) {

						xstring		rule( rule_p, p-rule_p );

						style::ptr st = std::make_shared<style>();
						st->add( rule.c_str(), baseurl);

						parse_selectors(selector, st, media);

						if( media && doc ) {
							doc->add_media_list(media);
						}

						p++;
					}
				}
			}
			else {

				if( ch=='*' && p[1]=='/' ) {
					cmt = false;
					p += 2;
					continue;
				}

				p++;
			}
		}
	}

	/**
	 * @brief css::parse_css_url
	 * @param str
	 * @param url
	 */

	void	css::parse_css_url( const xstring& str, xstring& url )
	{
		url.clear( );
		extract_url( str.c_str(), url );
	}

	/**
	 * @brief css::parse_selectors
	 * @param txt
	 * @param styles
	 * @param media
	 * @return true if something added
	 */

	bool css::parse_selectors( const xstring& txt, const style::ptr& styles, const media_query_list::ptr& media )
	{
		const tchar_t*	p = txt;

		while( *p ) {

			p = skip_sp( p );

			tchar_t*	start = p;
			while( *p && *p!=',' ) {
				p++;
			}

			if( p>start ) {

				// trim
				const tchar_t* q = p-1;
				while( q>start && (*q==' ' || *q=='\t') ) {
					q--;
				}

				if( q>start ) {

					// parse selector
					css_selector::ptr selector = std::make_shared<css_selector>(media);
					selector->m_style = styles;

					tstring	tok( start, q-start );
					if(selector->parse(tok))
					{
						selector->calc_specificity();
						add_selector(selector);
						added_something = true;
					}
				}
			}
		}

		return added_something;
	}

	/**
	 * @brief css::sort_selectors
	 */

	void css::sort_selectors()
	{
		std::sort(m_selectors.begin(), m_selectors.end(),
			 [](const css_selector::ptr& v1, const css_selector::ptr& v2)
			 {
				 return (*v1) < (*v2);
			 }
		);
	}

	/**
	 * @brief css::parse_atrule
	 * @param text
	 * @param baseurl
	 * @param doc
	 * @param media
	 */

	void css::parse_atrule( const tchar_t* text, const tchar_t* baseurl, const std::shared_ptr<document>& doc, const media_query_list::ptr& media)
	{
		if( t_strncmp(text,"@import",7)==0 ) {

			text += 7;

			//	@import url;
			//	@import url liste-requetes-media;

			xstring	url;
			text = extract_url( text, url );
			text = skip_sp( text );

			const tchar_t*	start = text;
			while( *text && *text!=';' ) {
				text++;
			}

			/*
			trim(iStr);
			string_vector tokens;
			split_string(iStr, tokens, _t(" "), _t(""), _t("(\""));

			if(!tokens.empty())
			{
				// todo: do not use token.front.c_str() but token.front
				xstring url;
				parse_css_url( tokens.front().c_str(), url);

				if(url.empty()) {
					url = tokens.front().c_str();
				}

				tokens.erase(tokens.begin());
				if(doc)
				{
					document_container* doc_cont = doc->container();
					if(doc_cont)
					{
						xstring css_text;
						xstring css_baseurl;
						if(baseurl) {
							css_baseurl = baseurl;
						}

						doc_cont->import_css(css_text, url, css_baseurl);
						if(!css_text.empty())
						{
							media_query_list::ptr new_media = media;
							if(!tokens.empty())
							{
								tstring media_str;
								for(string_vector::iterator iter = tokens.begin(); iter != tokens.end(); iter++)
								{
									if(iter != tokens.begin())
									{
										media_str += _t(" ");
									}
									media_str += (*iter);
								}
								new_media = media_query_list::create_from_string(media_str, doc);
								if(!new_media)
								{
									new_media = media;
								}
							}
							parse_stylesheet(css_text.c_str(), css_baseurl.c_str(), doc, new_media);
						}
					}
				}
			}
			*/
		}

		if( t_strncmp(text,"@media",6)==0 ) {

			tstring::size_type b1 = text.find_first_of(_t('{'));
			tstring::size_type b2 = text.find_last_of(_t('}'));

			if(b1 != tstring::npos)
			{
				tstring media_type = text.substr(6, b1 - 6);
				trim(media_type);
				media_query_list::ptr new_media = media_query_list::create_from_string(media_type, doc);

				tstring media_style;
				if(b2 != tstring::npos)
				{
					media_style = text.substr(b1 + 1, b2 - b1 - 1);
				}
				else {
					media_style = text.substr(b1 + 1);
				}

				parse_stylesheet(media_style.c_str(), baseurl, doc, new_media);
			}
		}
	}
}
