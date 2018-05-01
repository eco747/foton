#include "xstring.h"
#include <ctype.h>

namespace litehtml {
	/**
	 * @brief xstring::xstring
	 */

	xstring::xstring( )
		: m_ptr( m_buf )
	{
		*m_buf = 0;
		m_len = 0;
	}

	/**
	 * @brief xstring::xstring
	 * @param other
	 */

	xstring::xstring( const xstring& other )
	{
		*m_buf = 0;
		m_ptr = m_buf;
		m_len = 0;
		operator = ( other );
	}

	/**
	 * @brief xstring::xstring
	 * @param txt
	 * @param len
	 */

	xstring::xstring( const tchar_t* txt, int32_t len/*=-1*/ )
	{
		if( len < 0 ) {
			len = strlen( txt );
		}

		m_len = len;
		__alloc( m_len );
		memcpy( m_ptr, txt, m_len * sizeof( tchar_t ) );
		m_ptr[m_len] = 0;
	}

	/**
	 * @brief xstring::~xstring
	 */

	xstring::~xstring( )
	{
		__release( );
	}

	/**
	 * @brief xstring::operator =
	 * @param other
	 * @return
	 */

	xstring& xstring::operator = ( const xstring& other )
	{
		if( &other == this ) {
			__asm int 3;
		}

		__realloc( other.m_len );
		m_len = other.m_len;
		memcpy( m_ptr, other.m_ptr, ( m_len + 1 )*sizeof( tchar_t ) );
		return *this;
	}

	/**
	 * @brief xstring::set
	 * @param txt
	 * @param len
	 * @param lowercase
	 * @return
	 */

	xstring& xstring::set( const tchar_t* txt, int32_t len/*=-1*/, bool lowercase/*=false*/ )
	{
		if( txt >= m_ptr && txt <= m_ptr + m_len ) {
			__asm int 3;
		}

		if( len < 0 ) {
			len = strlen( txt );
		}

		__realloc( len );
		m_len	= len;

		if( len ) {
			if( lowercase == false ) {
				memcpy( m_ptr, txt, m_len * sizeof( tchar_t ) );
			}
			else {
				int32_t l = m_len;
				tchar_t* p = m_ptr;

				while( l-- ) {
					*p++ = tolower( *txt++ );
				}
			}
		}

		m_ptr[m_len] = 0;
		return *this;
	}

	/**
	 * @brief xstring::clear
	 */

	void	xstring::clear( )
	{
		__release( );
		*m_buf	= 0;
		m_ptr	= m_buf;
		m_len	= 0;
	}

	/**
	 * @brief xstring::__release
	 */

	void	xstring::__release( )
	{
		if( m_ptr != m_buf ) {
			free( m_ptr );
		}
	}

	/**
	 * @brief xstring::__alloc
	 * @param len
	 */

	void	xstring::__alloc( uint32_t len )
	{
		len++;

		if( len < BSIZE ) {
			m_ptr = m_buf;
		}
		else {
			m_ptr = ( tchar_t* )malloc( adj_size( len ) * sizeof( tchar_t ) );
		}
	}

	/**
	 * @brief xstring::__realloc
	 * @param len
	 */

	void	xstring::__realloc( uint32_t len )
	{
		uint32_t	ol = adj_size( m_len + 1 );
		uint32_t	nl = adj_size( len + 1 );

		if( ol != nl ) {
			if( m_ptr == m_buf ) {
				m_ptr = ( tchar_t* )malloc( nl );
			}
			else {
				if( nl <= BSIZE ) {
					free( m_ptr );
					m_ptr = m_buf;
				}
				else {
					m_ptr = ( tchar_t* )realloc( m_ptr, nl * sizeof( tchar_t ) );
				}
			}
		}
	}





	/**
	 * @brief xstringpart::trim_left
	 * @return
	 */

	const tchar_t* xstringpart::trim_left( )
	{
		return skip( ' ', '\t' );
	}

	/**
	 * @brief xstringpart::trim_right
	 * @return
	 */

	const tchar_t*	xstringpart::trim_right( )
	{
		const tchar_t* e = str + len - 1;

		while( len && ( *e == ' ' || *e == '\t' ) ) {
			len--;
		}

		return str;
	}

	/**
	 * @brief xstringpart::i_equ
	 * @param o
	 * @return
	 */

	bool	xstringpart::i_equ( const xstringpart& o )
	{
		return i_equ( o.str, o.len ) == 0;
	}

	bool	xtringpart::i_equ( const tchar_t* s, uint32_t len )
	{
		return o.len == len && _memicmp( str, o.str, len ) == 0;
	}



	/**
	 * @brief xstringpart::equ
	 * @param o
	 * @return
	 */

	bool	xstringpart::equ( const xstringpart& o )
	{
		return o.len == len && strncmp( str, o.str, len ) == 0;
	}

	int 	xstringpart::split( tchar_t sep, xstringpart* parts, int pcount ) const
	{
		int  		cidx = 0;
		xstringpart	temp( str, len );

		while( cidx < ( pcount - 1 ) ) {
			if( !temp.split( sep, &parts[cidx], &temp ) ) {
				parts[cidx] = temp;
				return cidx;
			}

			cidx++;
		}

		parts[cidx] = temp;
		return cidx + 1;
	}

	bool 	xstringpart::split( tchar_t sep, xstringpart* left, xstringpart* right ) const
	{
		int 	cidx = 0;
		const tchar_t*	p = start( );
		const tchar_t*	e = end( );
		const tchar_t*	q = p;

		while( p < e ) {
			if( *p == sep )  {
				left->set( q, p - q );
				p++;
				right->set( p, e - p );
				return true;
			}

			p++;
		}

		return false;
	}

	void xstringpart::move_start( int n )
	{
		if( n > len ) {
			n = len;
		}

		str += n;
		len -= n;
	}

	void xstringpart::skip( tchar_t c1, tchar_t c2 = 0 )
	{
		tchar_t*	s = str;
		tchar_t*	e = end( );

		if( c1 && c2 ) {
			while( s < e && *s == c1 ) {
				s++;
			}
		}
		else {
			while( s < e && ( *s == c1 || *s == c2 ) ) {
				s++;
			}
		}

		str	= s;
		len = e - s;
		return str;
	}

	bool xstringpart::extract( tchar_t sep_start, tchar_t sep_end, xstringpart* out )
	{
		tchar_t*	s = str;
		tchar_t*	e = end( );

		while( s < e && ( *s == ' ' || *s == '\t' ) ) {
			s++;
		}

		if( *s != sep_start ) {
			return false;
		}

		tchar_t*	v = s;
		s++;

		while( s < e ) {
			if( *s == sep_end ) {
				out.set( v, s - v );
				return true;
			}

			s++;
		}

		return false;
	}
}

