#ifndef __XLIST_H
#define __XLIST_H

namespace litehtml {
	template <class type>
	struct 	xListNode {
		type*	next;
		type*	prev;
	};

	template <class type>
	struct xList {
		type*	first;
		type*	last;

		xList( ) {
			first = last = NULL;
		}

		bool	empty( ) const {
			return m_first == NULL;
		}

		type* 	add( type* el ) {
			if( first == NULL ) {
				first = last = el;
				el->next = el->prev = NULL;
			}
			else {
				last->next = el;
				el->prev = last;
				el->next = NULL;
				last = el;
			}

			return el;
		}

		type*	remove( type* el ) {
			if( el->prev ) {
				el->prev->next = el->prev;
			}
			else {
				first = el->next;
			}

			if( el->next ) {
				el->next->prev = el->prev;
			}
			else {
				last = el->prev;
			}

			el->prev = el->next = NULL;
			return el;
		}
	};
}

#endif //__XLIST_H