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
		SF<A, C> operator > ( SF<B, C>& sf ) const
		{
			return compose( *this, sf );
		}
		template<typename C>
		// compose two SF ( this <<< sf )
		SF<C, B> operator < ( SF<C, A>& sf ) const
		{
			return compose( sf, *this );
		}
		// apply signal to SF
		S<B> operator () ( const S<A>& a ) const
		{
			return f( a );
		}
		// apply constant value to SF
		S<B> operator () ( const A& a ) const
		{
			return f( constant( a ) );
		}
		// apply signal to SF
		S<B> operator < ( const S<A>& a ) const
		{
			return f( a );
		}
		// apply constant value to SF
		S<B> operator < ( const A& a ) const
		{
			return f( constant( a ) );
		}
	};
	template<typename B>
	struct SF < void, B >
	{
		std::function<S<B>( const S<void>& )> f;
		template<typename C>
		// compose two SF ( *this >>> sf )
		SF<void, C> operator > ( SF<B, C> sf )
		{
			return compose( *this, sf );
		}
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A, typename B>
	SF<A, B> arr( B( *f )( const A& ) )
	{
		return SF < A, B > {
			[f]( const S<A>& a ) -> S < B >
			{
				return S < B > {
					[f, a]( const float deltaMs )
					{
						return f( std::forward<A>( a < deltaMs ) );
					}
				};
			}
		};
	}
	template<typename A, typename B>
	SF<A, B> arrAlt( std::function<B( const A& )> f )
	{
		return SF < A, B > {
			[f]( const S<A>& a ) -> S < B >
			{
				return S < B > {
					[f, a]( const float deltaMs )
					{
						// TODO: forward?
						return f( a < deltaMs );
					}
				};
			}
		};
	}
	/*template<typename A, typename B>
	SF<A, B> arr( std::function<B( const S<A>& )> f )
	{
	return SF < A, B > {
	[f]( const S<A>& a )
	{
	return signal( f( a ), a.deltaMs );
	}
	};
	}*/
	template<typename A, typename B, typename C>
	SF<A, C> compose( const SF<A, B>& fst, const SF<B, C>& snd )
	{
		return SF < A, C > {
			[fst, snd]( const S<A>& a ) -> S < C >
			{
				return snd.f( fst.f( a ) );
			}
		};
	}
}