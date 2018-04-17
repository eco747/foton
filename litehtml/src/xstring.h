
/**
* @brief The xstring class
*/

class xstring {
	static const int	BSIZE = 16;

	tchar_t*	__ptr;
	tchar_t		__buf[BSIZE];
	uint32_t	__len;

public:
	xstring( );
	xstring( const xstring& other );
	xstring( const tchar_t* txt, int32_t len=-1 );
	~xstring( );

	xstring& operator = (const xstring& other );
	xstring& operator = (const tchar_t* txt )	{ return set( txt ); }

	xstring& set( const tchar_t* txt, int32_t len=-1, bool lowercase=false );
	void	clear( );

	const tchar_t*	c_str( ) const				{ return __ptr; }
	tchar_t*		buffer( )					{ return __ptr; }
	uint32_t		length( ) const				{ return __len; }
	bool empty( ) const							{ return *__ptr==0; }


private:
	void	__release( );
	void	__alloc( uint32_t len );
	void	__realloc( uint32_t len );

	uint32_t adj_size( uint32_t size ) {
		return	((size + BSIZE - 1) / BSIZE) * BSIZE;
	}
};

