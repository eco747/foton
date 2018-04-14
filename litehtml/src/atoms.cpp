#include "html.h"

#include <qDebug>

namespace litehtml {
	#include "atoms_internal.cpp"

	struct	symbol
	{
		void*		value;
		uint16_t	len;
		uint32_t	hash;
		symbol*		next;

		symbol( const tchar_t* s, uint16_t ln, uint32_t h ) {
			len = ln;
			hash = h;
			next = NULL;
			value = malloc( ln );
			memcpy( value, s, ln );
		}
	};

	struct	symbol_table
	{
		symbol**	table;
		uint32_t	modulo;
	};

	symbol_table	g_symbols = {NULL, 0 };

	void	atom_init( )
	{
		g_symbols.modulo = 797;
		g_symbols.table = (symbol**)malloc( g_symbols.modulo * sizeof(symbol*) );
		memset( g_symbols.table, 0, g_symbols.modulo * sizeof(symbol*) );
	}

	static	uint32_t	hash( uint32_t d, const tchar_t* text ) {

		if(d == 0) {
			d = 0x811c9dc5;
		}

		while( *text ) {
			// multiply by the 32 bit FNV magic prime mod 2^32
			d += (d << 1) + (d << 4) + (d << 7) + (d << 8) + (d << 24);
			// xor the bottom with the current octet
			d ^= *text;	//tolower(*text);
			text++;
		}

		return d & 0x7fffffff;
	}

	atom atom_create( const tchar_t* text, bool create_it )
	{
		if( *text==0 ) {
			return atom_null;
		}

		//	lowercase it
		char	lowercase[128];
		int		len = 0;
		while( len<countof(lowercase)-1 && *text ) {
			lowercase[len] = tolower(*text);
			len++;
			text++;
		}

		lowercase[len] = 0;

//		if( strcmp(lowercase,"section)")==0 ) {
//			__asm int 3;
//		}

		//	looking for our known elements (fast hash)
		int			a;
		uint32_t	h = hash(0,lowercase);
		int			d = GT[ h % countof(GT) ];
		if( d<0 )	{
			a = VT[ 0-d-1 ];
		}
		else {
			a = VT[hash(d,lowercase) % countof(VT)];
		}

		//	known element ?
		if( h==RH[0-1-a] ) {
			return (atom)a;
		}

		if( !create_it ) {
			return atom_null;
		}

		// no, add it to our temp table
		if( !g_symbols.table ) {
			atom_init( );
		}

		h	&= 0x7FFFFFFF;

		uint32_t	modulo = h % g_symbols.modulo;
		symbol*		sym	= g_symbols.table[modulo];

		while( sym != NULL ) {
			if (sym->hash==h && sym->len==len ) {
				if (memcmp(sym->value, lowercase, len) == 0)
					break;
			}

			sym = sym->next;
		}

		if (sym == NULL) {

			qDebug() << lowercase << ",";

			sym = new symbol( lowercase, len, h );
			sym->next = g_symbols.table[modulo];
			g_symbols.table[modulo] = sym;
		}

		return (atom)h;
	}

	tstring atom_get( const tchar_t* v ) {

		/*
		int		d = GT[ hash(0,text) % countof(G) ];
		if( d<0 )	{
			return input + VT[ 0-d-1 ];
		}

		return input + VT[hash(d,text) % countof(V)];

		//symbol*	x = (symbol*)v;
		//return	std::string((const char*)x->value,x->len);
		*/
		return "";
	}
}
