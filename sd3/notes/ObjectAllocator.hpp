#pragma once

#include "AICommon.hpp"

#include <queue>
#include <mutex>
#include <algorithm>

//-----------------------------------------------------------------------------------------------
inline size_t Align( size_t value, size_t alignment ) 
{
	size_t mod = value % alignment; 
	if (mod == 0) {
		return value; 
	} else {
		return value + (alignment - mod); 
	}
}

//-----------------------------------------------------------------------------------------------
template <typename T>
class ObjectAllocator
{
	public:
		//-----------------------------------------------------------------------------------------------
		ObjectAllocator()
		{
		}

		//-----------------------------------------------------------------------------------------------
		~ObjectAllocator()
		{
			destroy_all_chunks(); 
		}

		//-----------------------------------------------------------------------------------------------
		template <typename ...ARGS>
		T* alloc( ARGS& ...args )
		{
			node_t* node = pop_free_node();
			while (node == nullptr) 
			{
				if (!alloc_chunk()) {
					std::this_thread::yield(); 
				}
				node = pop_free_node(); 
			}

			return new(node) T( args... ); 
		}

		//-----------------------------------------------------------------------------------------------
		void free( T* obj )
		{
			if (obj != nullptr) 
			{
				obj->~T(); 
				push_free_node( (node_t*)obj ); 
			}
		}

	private:
		//-----------------------------------------------------------------------------------------------
		struct node_t
		{
			node_t* next; 
		};

		//-----------------------------------------------------------------------------------------------
		struct chunk_t
		{
			chunk_t* next; 
		};

		//-----------------------------------------------------------------------------------------------
		node_t* pop_free_node()
		{
			std::scoped_lock lk(m_lock); 
			node_t* node = m_free_list; 
			if (node != nullptr) {
				m_free_list = m_free_list->next; 
			}
			return node; 
		}

		//-----------------------------------------------------------------------------------------------
		void push_free_node( node_t* node )
		{
			std::scoped_lock lk(m_lock); 
			node->next = m_free_list; 
			m_free_list = node; 
		}

		//-----------------------------------------------------------------------------------------------
		bool alloc_chunk()
		{
			if (m_chunk_lock.try_lock() == false) {
				return false; 
			}

			// allocate memory for the chunk;
			size_t alignment = std::max<size_t>( 128, alignof(T) ); 
			size_t head_size = std::max<size_t>( sizeof(chunk_t), alignment ); 
			size_t stride = Align( sizeof(T), alignment ); 

			size_t size = head_size + stride * m_blocks_per_chunk; 
			byte* buffer = (byte*) ::_aligned_malloc(size, alignment); 

			// add it to the list to be cleaned up at the end
			chunk_t* chunk = (chunk_t*)buffer; 
			chunk->next = m_chunk_list; 
			m_chunk_list = chunk; 
			buffer += head_size; 

			// construct a chain of free nodes; 
			node_t* prev = nullptr; 
			node_t* last = (node_t*)buffer; 

			for (int i = 0; i < m_blocks_per_chunk; ++i) {
				node_t* cur = (node_t*)buffer; 
				cur->next = prev; 
				prev = cur;

				buffer += stride; 
			}

			// stich up the free list; 
			m_lock.lock(); 
			last->next = m_free_list; 
			m_free_list = prev; 
			m_lock.unlock(); 

			// free up my final lock (do this after
			// so two people don't try to allocate chunks at a time)
			m_chunk_lock.unlock(); 
			return true; 
		}

		//-----------------------------------------------------------------------------------------------
		void destroy_all_chunks()
		{
			chunk_t* iter = m_chunk_list; 
			while (nullptr != iter) {
				chunk_t* next = iter->next; 
				_aligned_free( iter ); 
				iter = next; 
			}
		}
			
	private:
		chunk_t* m_chunk_list	= nullptr; 
		node_t* m_free_list		= nullptr; 

		std::mutex m_lock; 
		std::mutex m_chunk_lock; 

		int m_blocks_per_chunk = 128; 
};
