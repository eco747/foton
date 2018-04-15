namespace litehtml
{
	/**
	 * @brief xstring::xstring
	 */

	xstring::xstring( )
		: __ptr( __buf )
	{
		*__buf = 0;
		__len = 0;
	}

	/**
	 * @brief xstring::xstring
	 * @param other
	 */

	xstring::xstring( const xstring& other )
	{
		*__buf = 0;
		__ptr = __buf;
		__len = 0;

		operator = (other);
	}

	/**
	 * @brief xstring::xstring
	 * @param txt
	 * @param len
	 */

	xstring::xstring( const tchar_t* txt, int32_t len=-1 )
	{
		if( len<0 ) {
			len = strlen( txt );
		}

		__len = len;
		__alloc( __len );
		memcpy( __ptr, txt, __len*sizeof(tchar_t) );
		__ptr[__len] = 0;
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

	xstring& xstring::operator = (const xstring& other ) {

		if( &other==this ) {
			__asm int 3;
		}

		__realloc( other.__len );
		__len = other.__len;
		memcpy( __ptr, other.__ptr, (__len+1)*sizeof(tchar_t) );
		return *this;
	}

	/**
	 * @brief xstring::set
	 * @param txt
	 * @param len
	 * @param lowercase
	 * @return
	 */

	xstring& xstring::set( const tchar_t* txt, int32_t len=-1, bool lowercase=false )
	{
		if( txt>=__ptr && txt<=__ptr+__len ) {
			__asm int 3;
		}

		if( len<0 ) {
			len = strlen( txt );
		}

		__realloc( len );
		__len	= len;

		if( len ) {
			if( lowercase==false ) {
				memcpy( __ptr, txt, __len*sizeof(tchar_t) );
			}
			else {
				int32_t l = __len;
				tchar_t* p = __ptr;

				while( l-- ) {
					*p++ = tolower(*txt++);
				}
			}
		}

		__ptr[__len] = 0;

		return *this;
	}

	/**
	 * @brief xstring::clear
	 */

	void	xstring::clear( ) {
		__release( );
		*__buf	= 0;
		__ptr	= __buf;
		__len	= 0;
	}

	/**
	 * @brief xstring::__release
	 */

	void	xstring::__release( ) {
		if( __ptr!=__buf ) {
			free( __ptr );
		}
	}

	/**
	 * @brief xstring::__alloc
	 * @param len
	 */

	void	xstring::__alloc( uint32_t len ) {
		len++;
		if( len<BSIZE ) {
			__ptr = __buf;
		}
		else {
			__ptr = (tchar_t*)malloc( adj_size(len) * sizeof(tchar_t) );
		}
	}

	/**
	 * @brief xstring::__realloc
	 * @param len
	 */

	void	xstring::__realloc( uint32_t len )
	{
		uint32_t	ol = adj_size( __len+1 );
		uint32_t	nl = adj_size( len+1 );

		if( ol!=nl ) {
			if( __ptr==__buf ) {
				__ptr = (tchar_t*)malloc( nl );
			}
			else {
				if( nl<=BSIZE ) {
					free( __ptr );
					__ptr = __buf;
				}
				else {
					__ptr = (tchar_t*)realloc( __ptr, nl*sizeof(tchar_t) );
				}
			}
		}
	}
}
