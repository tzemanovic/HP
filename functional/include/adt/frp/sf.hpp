#pragma once
#include <functional>
#include "s.hpp"
namespace hp_fp
{
	template<typename A, typename B>
	struct SF
	{
		SF( std::function<S<B>( const S<A>& a )> f ) : f( f )
		{ }
		SF( const SF& sf ) : f( sf.f )
		{ }
		SF( SF&& sf ) : f( std::move( sf.f ) )
		{ }
		SF operator = ( const SF& sf )
		{
			return SF{ sf };
		}
		SF operator = ( SF&& sf )
		{
			return SF{ std::move( sf ) };
		}
		std::function<S<B>( const S<A>& a )> f;
		template<typename C>
		// compose two SF ( this >>> sf )
		SF<A, C> operator > ( SF<B, C>& sf )
		{
			return compose( *this, sf );
		}
		template<typename C>
		// compose two SF ( sf >>> *this )
		SF<C, B> operator < ( SF<C, A>& sf )
		{
			return compose( sf, *this );
		}
		// apply signal to SF
		S<B> operator < ( const S<A>& a )
		{
			return f( a );
		}
		// apply signal to SF
		S<B> operator () ( const S<A>& a )
		{
			return f( a );
		}
		// compose SF and constant value ( a >>> *this )
		SF<void, B> operator < ( const A& a )
		{
			return compose<void, A, B>( constant( a ), *this );
		}
	};
	template<typename B>
	struct SF < void, B >
	{
		std::function<S<B>( const S<void>& a )> f;
		template<typename C>
		// compose two SF ( *this >>> sf )
		SF<void, C> operator > ( SF<B, C> sf )
		{
			return compose( *this, sf );
		}
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A, typename B>
	SF<A, B> arr( B( *f )( const A& a ) )
	{
		return SF < A, B > {
			[f]( const S<A>& a )
			{
				return signal( f( a.val ), a.deltaMs );
			}
		};
	}
	template<typename A, typename B>
	SF<A, B> arr( B( *f )( const S<A>& a ) )
	{
		return SF < A, B > {
			[f]( const S<A>& a )
			{
				return signal( f( a ), a.deltaMs );
			}
		};
	}
	template<typename A, typename B, typename C>
	SF<A, B> compose( const SF<A, B>& fst, const SF<B, C>& snd )
	{
		return SF < A, B > {
			[fst, snd]( const S<A>& a )
			{
				return snd.f( signal( fst.f( a ).val, a.deltaMs ) );
			}
		};
	}
}