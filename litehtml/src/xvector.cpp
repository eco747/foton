#include "types.h"

#define 	mem_alloc	malloc
#define 	mem_free 	free

xVectorBase::xVectorBase( uint32_t initial_capacity /*=0*/ ) 
{
	m_length = 0;
	m_capacity = initial_capacity;
	if (initial_capacity > 0) {
		m_data = mem_alloc( sizeof(void*) * initial_capacity );
	}
	else {
		m_data = NULL;
	}
}

xVectorBase:: ~xVectorBase( ) {
	if (m_capacity > 0) {
		mem_free( m_data );
	}
}

void xVectorBase::_add( void* el )
{
	_grow_if_full( );
	assert(m_data);
	assert(m_length < m_capacity);

	m_data[m_length++] = el;
}

void* xVectorBase::_pop( )
{
	if ( m_length == 0) {
		return NULL;
	}

	return m_data[--m_length];
}

int xVectorBase::_index_of( void* el )
{
	for (uint32_t i = 0; i < m_length; ++i) {
		if (m_data[i] == el) {
			return i;
		}
	}

	return -1;
}

void xVectorBase::_insert_at( uint32_t index, void* el )
{
	assert(index <= m_length);
	_grow_if_full();

	++m_length;
	memmove( &m_data[index + 1], &m_data[index], sizeof(void*) * (m_length - index - 1) );
	m_data[index] = el;
}

void xVectorBase::_remove( type* el )
{
	int index = _index_of( el );
	if (index == -1) {
		return;
	}

	_remove_at( index );
}

void* xVectorBase::_remove_at( uint32_t index )
{
	assert(index < m_length);
	void* result = m_data[index];

	memmove( &m_data[index], &m_data[index + 1], sizeof(void*) * (m_length - index - 1));
	--m_length;
	
	return result;
}

void xVectorBase::_grow_if_full( ) 
{
	if ( m_length >= m_capacity ) {
		
		if( m_capacity ) {
			size_t old_num_bytes = sizeof(void*) * m_capacity;
			m_capacity *= 2;

			size_t num_bytes = sizeof(void*) * m_capacity;
			void** temp = mem_alloc( num_bytes );

			memcpy(temp, m_data, old_num_bytes);
			mem_free( m_data );
	
			m_data = temp;
	}
	else {
		// 0-capacity vector; no previous array to deallocate.
		m_capacity = 2;
		m_data = mem_alloc( sizeof(void*) * m_capacity);
	}
}	

