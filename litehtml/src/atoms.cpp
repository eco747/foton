#include "html.h"

#include <qDebug>

namespace litehtml {

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

	atom atom_create( const tchar_t* text, bool create_it ) {
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

	/**
	 * 
	 **/

	int hash_find(const tchar_t* text, int len,
						int* GT, int GT_len,
						int* VT, int VT_len,
						uint32_t* RH, int RH_len,
						int def )
	{
		if( len==0 || *text==0 ) {
			return def;
		}

		char	lowercase[128];
		if( len<0 ) {
			//	lowercase it
			len 	= 0;
			while( len<countof(lowercase)-1 && *text ) {
				lowercase[len] = tolower(*text);
				len++;
				text++;
			}
		}
		else {
			if( len>countof(lowercase)-1 ) {
				len = countof(lowercase)-1;
			}

			char* lc = lowercase;
			while( len ) {
				lc++ = tolower(*text);
				len--;
				text++;
			}
		}

		lowercase[len] = 0;

//		if( strcmp(lowercase,"section)")==0 ) {
//			__asm int 3;
//		}

		//	looking for our known elements (fast hash)
		int			a;
		uint32_t	h = hash(0,lowercase);
		int			d = GT[ h % GT_len ];
		if( d<0 )	{
			a = VT[ 0-d-1 ];
		}
		else {
			a = VT[hash(d,lowercase) % VT_len];
		}

		//	known element ?
		if( h==RH[0-1-a] ) {
			return a;
		}

		return def;
	}

	/**
	 * 
	 */
	
	int 	string_find( const tchar_t* text, int len, const tchar_t* inputs, int def )
	{
		int idx = 1;
		tchar_t	c1 = text[0];
		tchar_t	c2 = text[len-1];

		while( *inputs )  {
			int 	l = *inputs++;
			if( len==l && inputs[0]==c1 && inputs[l-1]==c2 ) {
				if( t_strncasecmp(inputs+1,text+1,l-2)==0 ) {
					return idx;
				}
			}

			inputs += l;
			idx++;
		}

		return def;
	}
}
