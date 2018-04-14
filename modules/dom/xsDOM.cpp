#include "litehtml.h"

extern "C" {
	#include "xsAll.h"
	#include "xsmc.h"

//	void	dom_document_create_element(xsMachine *the);
//	void	dom_document_get_root(xsMachine *the);

	void	dom_element_ctor(xsMachine* the);
	void	dom_element_dtor(void* data);
	void	dom_element_set_tag_name(xsMachine *the);
	void	dom_element_get_tag_name(xsMachine *the);
	void	dom_element_set_left(xsMachine *the);
	void	dom_element_get_left(xsMachine *the);
	void	dom_element_set_top(xsMachine *the);
	void	dom_element_get_top(xsMachine *the);
	void	dom_element_set_right(xsMachine *the);
	void	dom_element_get_right(xsMachine *the);
	void	dom_element_set_bottom(xsMachine *the);
	void	dom_element_get_bottom(xsMachine *the);
	void	dom_element_set_width(xsMachine *the);
	void	dom_element_get_width(xsMachine *the);
	void	dom_element_set_height(xsMachine *the);
	void	dom_element_get_height(xsMachine *the);
	void	dom_element_set_margins(xsMachine *the);
	void	dom_element_get_margins(xsMachine *the);
	void	dom_element_set_paddings(xsMachine *the);
	void	dom_element_get_paddings(xsMachine *the);
	void	dom_element_set_borders(xsMachine *the);
	void	dom_element_get_borders(xsMachine *the);
	void	dom_element_set_attr(xsMachine *the);
	void	dom_element_get_attr(xsMachine *the);
	void	dom_element_set_style(xsMachine *the);
	void	dom_element_get_style(xsMachine *the);
	void	dom_element_append_child(xsMachine *the);
	void	dom_element_remove_child(xsMachine *the);
	void	dom_element_get_parent(xsMachine *the);
}

/**
 * @brief create a DOMElement
 * @param tag_name - the tag to create
 * by default, the element is added to the current document.
 */

void	dom_element_ctor( xsMachine* the )
{
	const char*			tag = fxToString( the, &xsArg(0) );
	litehtml::document*	doc = (litehtml::document*)the->machine_data;

	litehtml::attr_map		attrs;

	litehtml::element::ptr	el = doc->create_element( tag, attrs );
	fxSetHostData( the, &xsThis, el.get() );
	memset( &el, 0, sizeof(el) );
}

void	dom_element_dtor(void* data)
{
	litehtml::element*	el = (litehtml::element*)data;
	if( el ) {
//		delete el;
	}
}

void	dom_element_set_tag_name(xsMachine *the)
{
}

void	dom_element_get_tag_name(xsMachine *the)
{
}

void	dom_element_set_left(xsMachine *the)
{
}

void	dom_element_get_left(xsMachine *the)
{
}

void	dom_element_set_top(xsMachine *the)
{
}

void	dom_element_get_top(xsMachine *the)
{
}

void	dom_element_set_right(xsMachine *the)
{
}

void	dom_element_get_right(xsMachine *the)
{
}

void	dom_element_set_bottom(xsMachine *the)
{
}

void	dom_element_get_bottom(xsMachine *the)
{
}

void	dom_element_set_width(xsMachine *the)
{
}

void	dom_element_get_width(xsMachine *the)
{
}

void	dom_element_set_height(xsMachine *the)
{
}

void	dom_element_get_height(xsMachine *the)
{
}

void	dom_element_set_margins(xsMachine *the)
{
}

void	dom_element_get_margins(xsMachine *the)
{
}

void	dom_element_set_paddings(xsMachine *the)
{
}

void	dom_element_get_paddings(xsMachine *the)
{
}

void	dom_element_set_borders(xsMachine *the)
{
}

void	dom_element_get_borders(xsMachine *the)
{
}

void	dom_element_set_attr(xsMachine *the)
{
}

void	dom_element_get_attr(xsMachine *the)
{
}

void	dom_element_set_style(xsMachine *the)
{
}

void	dom_element_get_style(xsMachine *the)
{
}

void	dom_element_append_child(xsMachine *the)
{
}

void	dom_element_remove_child(xsMachine *the)
{
}

void	dom_element_get_parent(xsMachine *the)
{
}
