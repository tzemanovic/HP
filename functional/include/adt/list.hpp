// inspired by http://bartoszmilewski.com/2013/11/13/functional-data-structures-in-c-lists/
#pragma once
#include <cassert>
#include <initializer_list>
#include <memory>
namespace hp_fp
{
	template<typename A>
	struct List
	{
	private:
		struct Item
		{
			Item( A v, std::shared_ptr<const Item> const & tail ) : _val( v ), _next( tail )
			{ }
			A _val;
			std::shared_ptr<const Item> _next;
		};
		friend Item;
		explicit List( Item const * items ) : _head( items )
		{ }
		std::shared_ptr<const Item> _head;
	public:
		List( )
		{ }
		List( A v, List tail ) : _head( std::make_shared<Item>( v, tail._head ) )
		{ }
		List( A&& v, List tail ) : _head( std::make_shared<Item>( v, tail._head ) )
		{ }
		List( std::initializer_list<A> init )
		{
			for ( auto it = std::rbegin( init ); it != std::rend( init ); ++it )
			{
				_head = std::make_shared<Item>( *it, _head );
			}
		}
		bool isEmpty( ) const
		{
			return _head == nullptr;
		}
		A head( ) const
		{
			assert( !isEmpty( ) );
			return _head->_val;
		}
		List tail( ) const
		{
			assert( !isEmpty( ) );
			return List( _head->_next );
		}
		List push( A v ) const
		{
			return List( v, *this );
		}
		List push( A&& v ) const
		{
			return List( v, *this );
		}
		List insertAt( int i, A v ) const
		{
			if ( i == 0 )
			{
				return push( v );
			}
			else
			{
				assert( !isEmpty( ) );
				return List( head( ), tail( ).insertAt( i - 1, v ) );
			}
		}
	};
	template<typename A>
	List<A> concat( List<A> a, List<A> b )
	{
		if ( a.isEmpty( ) )
		{
			return b;
		}
		return List<A>( a.head( ), concat( a.tail( ), b ) );
	}
	template<typename A, typename B>
	auto fmap( B f, List<A> lst ) -> List < decltype( f( lst.head( ) ) ) >
	{
		static_assert( std::is_convertible<B, std::function<B( A )>>::value, "fmap requires a function type B(A)" );
		if ( lst.isEmpty( ) )
		{
			return List<decltype( f( lst.front( ) ) )>( );
		}
		return List<decltype( f( lst.front( ) ) )>( f( lst.head( ) ), fmap<U>( f, lst.tail( ) ) );
	}
	template<typename A, typename B>
	List<A> filter( B p, List<A> lst )
	{
		static_assert( std::is_convertible<B, std::function<bool( A )>>::value, "filter requires a function type bool(A)" );
		if ( lst.isEmpty( ) )
		{
			return List<A>( );
		}
		if ( p( lst.head( ) ) )
		{
			return List<A>( lst.head( ), filter( p, lst.tail( ) ) );
		}
		return filter( p, lst.tail( ) );
	}
	template<typename A, typename B, typename C>
	B foldr( C f, B acc, List<A> lst )
	{
		static_assert( std::is_convertible<C, std::function<B( A, B )>>::value, "foldr requires a function type B(A, B)" );
		if ( lst.isEmpty( ) )
		{
			return acc;
		}
		return f( lst.head( ), foldr( f, acc, lst.tail( ) ) );
	}
	template<typename A, typename B, typename C>
	B foldl( C f, B acc, List<A> lst )
	{
		static_assert( std::is_convertible<C, std::function<B( B, A )>>::value, "foldl requires a function type B(B, A)" );
		if ( lst.isEmpty( ) )
		{
			return acc;
		}
		return foldl( f, f( acc, lst.head( ) ), lst.tail( ) );
	}
	template<typename A, typename B>
	void forEach( List<A> lst, B f )
	{
		static_assert( std::is_convertible<B, std::function<void( A )>>::value, "forEach requires a function type void(A)" );
		if ( !lst.isEmpty( ) )
		{
			f( lst.head( ) );
			forEach( lst.tail( ), f );
		}
	}
}