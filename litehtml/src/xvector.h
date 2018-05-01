#ifndef __XVECTOR_H
#define __XVECTOR_H

namespace litehtml {

	class xVectorBase {
		void**		m_data;
		uint32_t	m_length;
		uint32_t	m_capacity;

	protected:
		xVectorBase( uint32_t initial_capacity = 0 );
		virtual ~xVectorBase( );

		void 	_add( void* el );
		void*	_pop( );
		int 	_index_of( void* el );
		void 	_insert_at( uint32_t index, void* el );
		void 	_remove( void* el );
		void* 	_remove_at( uint32_t index );
		void*	_at( uint32_t index ) const;
		void*	_first( ) const;
		void*	_last( ) const;

	public:
		void	 clear( );
		//void 	 copy( const XVectorBase& );
		uint32_t length( ) const {
			return m_length;
		}
		bool 	 empty( ) const {
			return m_length == 0;
		}

	private:
		void 	_grow_if_full( );
	};

	template <class type>
	class 	xVector
		:	public xVectorBase {
	public:
		xVector( uint32_t initial_capacity = 0 )
			: xVectorBase( initial_capacity ) {
		}

		void 	add( type* el ) {
			_add( el );
		}

		type*		pop( ) {
			return ( type* )_pop( );
		}

		type* operator []( uint32_t index ) const {
			return ( type* )_at( index );
		}

		type*	at( uint32_t index ) const {
			return ( type* )_at( index );
		}

		int 	index_of( type* el ) {
			return _index_of( el );
		}

		void 	insert_at( uint32_t index, type* el ) {
			return insert_at( index, el );
		}

		void 	remove( type* el ) {
			_remove( el );
		}

		type* 	remove_at( uint32_t index ) {
			return ( type* )_remove_at( index );
		}

		type*  first( ) const {
			return _first( );
		}

		type*  last( ) const {
			return _last;
		}
	};
}

#endif
