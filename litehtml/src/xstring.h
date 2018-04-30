
#if		1
#	include <stdint.h>
#	include <string.h>
#	include <stdlib.h>

typedef char	tchar_t;

#define		t_strlen	strlen
#endif


/**
* @brief The xstring class
*/

namespace litehtml
{

	class xstring {
		static const int	BSIZE = 16;

		tchar_t*	m_ptr;
		tchar_t		m_buf[BSIZE];
		uint32_t	m_len;

	public:
		xstring( );
		xstring( const xstring& other );
		xstring( const tchar_t* txt, int32_t len=-1 );
		~xstring( );

		xstring& operator = (const xstring& other );
		xstring& operator = (const tchar_t* txt )	{ return set( txt ); }

		xstring& set( const tchar_t* txt, int32_t len=-1, bool lowercase=false );
		void	clear( );

		const tchar_t*	c_str( ) const				{ return m_ptr; }
		tchar_t*		buffer( )					{ return m_ptr; }
		uint32_t		length( ) const				{ return m_len; }
		bool empty( ) const							{ return *m_ptr==0; }


	private:
		void	__release( );
		void	__alloc( uint32_t len );
		void	__realloc( uint32_t len );

		uint32_t adj_size( uint32_t size ) {
			return	((size + BSIZE - 1) / BSIZE) * BSIZE;
		}
	};

	/**
	 * @brief The xstringpart class
	 */

	struct	xstringpart
	{
		const tchar_t*	str;
		uint32_t		len;

	public:
		xstringpart( )
			:	str( NULL),
				len( 0 )
		{
		}
		xstringpart( const xstring& s ) 
			: 	str( s.c_str() ),
				len( s.length() )
		{
		}

		xstringpart( const tchar_t* s )
			:	str( s )
		{
			len = t_strlen( s );
		}

		xstringpart( const tchar_t* s, uint32_t l )
			:	str( s ),
				len( l )
		{
		}

		xstringpart&	operator = (const xstringpart& other) {
			str = other.str;
			len = other.len;
			return *this;
		}

		void 	set( const tchar_t* s, uint32_t l ) {
			str = s;
			len = l;
		}

		const tchar_t*	end( ) const { return str+len; }
		const tchar_t*	start( ) const { return str; }

		void 			move_start( int n );
		bool			extract( tchar_t sep_start, tchar_t sep_end, xstringpart* out );

		const tchar_t* 	trim_left( );
		const tchar_t*	trim_right( );
		const tchar_t*	trim( ) { trim_right(); return trim_left(); }

		bool 	split( tchar_t sep, xstringpart* left, xstringpart* right ) const;
		int 	split( tchar_t sep, xstringpart* parts, int pcount ) const;
		
		int 	rsplit( tchar_t sep, xstringpart* left, xstringpart* right );

		bool	i_equ( const xstringpart& o );
		bool	equ( const xstringpart& o );
		
		bool	i_equ( const tchar_t* s, uint32_t len );
		bool	equ( const tchar_t* s, uint32_t len );
	};
}
