#include "html.h"
#include "el_before_after.h"
#include "el_text.h"
#include "el_space.h"
#include "el_image.h"

namespace litehtml
{

	el_before_after_base::el_before_after_base( document* doc, bool before)
		: html_tag(doc)
	{
		if(before){
			set_tagName( atom___before /*_t("::before")*/ );
		}
		else {
			set_tagName( atom___after /*_t("::after")*/ );
		}
	}

	el_before_after_base::~el_before_after_base()
	{
	}

	void el_before_after_base::add_style(const style& st)
	{
		html_tag::add_style(st);

		const tchar_t*	content = get_style_property(atom_content, false, atom_null );

		//	content: normal;
		//	content: none;
		//	content: 'text';
		//	content: url(http:/exemple.com/img.png);
		//	content: counter(c_name);
		//	content: attr(a_name);
		//	content: open-quote;
		//	content: close-quote;
		//	content: no-open-quote;
		//	content: no-close-quote;

		//	Sauf avec normal et none, on peut utiliser plusieurs valeurs de façon simultanée
		//	content: open-quote chapter_counter;

		//	Valeurs globales
		//	content: inherit;
		//	content: initial;
		//	content: unset;


		//<eco: bad old code: if( content.empty() )
		if( content && *content!=0 )
		{
			int idx = get_content_property(content, -1, -1 );
			if(idx < 0) {
				const tchar_t*	p = skip_sp( content );
				if( *p=='"' || *p=='\'' ) {

					tchar_t		sep = *p++;
					const tchar_t*	start = p;

					while( *p && *p!=sep ) {
						p++;
					}

					if( *p==sep ) {
						add_text( start, p-1-start );
					}
				}
				else {
					const tchar_t*	start = p;

					// skip until end or params
					while( *p && *p!='(' && *p!=' ' && *p!='\t' ) {
						p++;
					}

					p = skip_sp( p );

					if( *p=='(' ) {

						//	url
						//	counter
						//	attr

						xstring	t( start, p-start );
						if( t.imatch("url") ) {

							p = skip_sp( ++p );

							xstring	url;

							if( *p=='\'' || *p=='\"' ) {
								tchar_t	sep = *p;
								p++;
								start = p;

								while( *p && *p!=sep ) {
									p++;
								}

								if( *p==sep ) {
									url.set( start, p-start, true );
								}
							}
							else {
								start = p;
								while( *p && *p!=')' ) {
									p++;
								}

								if( *p==')' ) {

									// trim
									while( p>start && (p[-1]==' ' || p[-1]=='\t') )
										p--;

									url.set( start, p-start, true );
								}
							}


							element* el = new el_image(get_document());
							el->set_attr( atom_src, property_value(url) );
							el->set_attr( atom_style, property_value("display:inline-block"));
							el->set_tagName( atom_img /*_t("img")*/);
							appendChild(el);

							el->parse_attributes();
						}
						else if( t.imatch("counter") ) {
							//<todo:
						}
						else if( t.imatch("attr") ) {

							element* el_parent = parent();
							if (el_parent) {

								start = skip_sp( ++p );
								p = skip_nsp( p );

								xstring	param( start, p-start );
								atom	a_name = atom_create(param.c_str(), false );

								if( a_name!=atom_null ) {	// unknown name
									property_value attr_value = el_parent->get_attr( a_name );
									if ( attr_value!=property_value::undefined ) {
										add_text( attr_value );
									}
								}
							}
						}
					}
					else {
						// ignored
					}
				}
			}
		}
	}

	void el_before_after_base::add_text( const char* txt, int len )
	{
		tstring word;
		tstring esc;

		for(tstring::size_type i = 0; i < txt.length(); i++)
		{
			if( (txt.at(i) == _t(' ')) || (txt.at(i) == _t('\t')) || (txt.at(i) == _t('\\') && !esc.empty()) )
			{
				if(esc.empty())
				{
					if(!word.empty())
					{
						element* el = new el_text(word.c_str(), get_document());
						appendChild(el);
						word.clear();
					}

					element* el = new el_space(txt.substr(i, 1).c_str(), get_document());
					appendChild(el);
				}
				else {
					word += convert_escape(esc.c_str() + 1);
					esc.clear();
					if(txt.at(i) == _t('\\')) {
						esc += txt.at(i);
					}
				}
			}
			else {
				if(!esc.empty() || txt.at(i) == _t('\\')) {
					esc += txt.at(i);
				}
				else {
					word += txt.at(i);
				}
			}
		}

		if(!esc.empty()) {
			word += convert_escape(esc.c_str() + 1);
		}

		if(!word.empty()) {
			element* el = new el_text(word.c_str(), get_document());
			appendChild(el);
			word.clear();
		}
	}

	void el_before_after_base::add_function( const tstring& fnc, const tstring& params )
	{
		int idx = value_index(fnc.c_str(), _t("attr;counter;url"));
		switch(idx)
		{
		// attr
		case 0:
			{
				element*	el_parent = parent();
				if (el_parent)
				{
					tstring p_name = params;
					trim(p_name);
					//lcase(p_name);
					atom a_name = atom_create(p_name.c_str(), false );
					//__asm int 3;	//<eco:

					const tchar_t* attr_value = el_parent->get_attr(a_name);
					if (attr_value) {
						add_text(attr_value);
					}
				}
			}
			break;

		// counter
		case 1:
			break;

		// url
		case 2:
			{
				tstring p_url = params;
				trim(p_url);
				if(!p_url.empty())
				{
					if(p_url.at(0) == _t('\'') || p_url.at(0) == _t('\"'))
					{
						p_url.erase(0, 1);
					}
				}
				if(!p_url.empty())
				{
					if(p_url.at(p_url.length() - 1) == _t('\'') || p_url.at(p_url.length() - 1) == _t('\"'))
					{
						p_url.erase(p_url.length() - 1, 1);
					}
				}
				if(!p_url.empty())
				{
					element* el = new el_image(get_document());
					el->set_attr(atom_src, p_url.c_str());
					el->set_attr(atom_style, _t("display:inline-block"));
					el->set_tagName( atom_img /*_t("img")*/);
					appendChild(el);
					el->parse_attributes();
				}
			}
			break;
		}
	}

	tchar_t el_before_after_base::convert_escape( const tchar_t* txt )
	{
		tchar_t* sss = 0;
		return (tchar_t) t_strtol(txt, &sss, 16);
	}

	void el_before_after_base::apply_stylesheet( const css& /*stylesheet*/ )
	{
	}
}
