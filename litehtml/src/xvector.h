
class xVectorBase
{
	void**		m_data;
	uint32_t	m_length;
	uint32_t	m_capacity;

protected:
	xVectorBase( uint32_t initial_capacity=0 );
	virtual ~xVectorBase( );

	void 	_add( void* el );
	void*	_pop( );
	int 	_index_of( void* el );
	void 	_insert_at( uint32_t index, void* el );
	void 	_remove( void* el );
	void* 	_remove_at( uint32_t index );

private:
	void 	_grow_if_full( );
};

template <class type>
class 	xVector
	:	public xVectorBase
{
public:
	xVector( uint32_t initial_capacity=0 )
		: xVectorBase( initial_capacity ) 
	{
	}

	void 	add( type* el )	
	{
		_add( el ); 
	}
	
	type*		pop( )
	{
		return (type*)_pop( );
	}

	int 	index_of( type* el )
	{
		return _index_of( el );
	}

	void 	insert_at( uint32_t index, type* el )
	{
		return insert_at( index, el );
	}

	void 	remove( type* el )
	{
		_remove( el );
	}

	type* 	remove_at( uint32_t index )
	{
		return (type*)_remove_at( index );
	}
};



