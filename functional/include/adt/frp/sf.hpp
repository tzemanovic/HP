#pragma once
#include "../../math/vec3.hpp"
namespace hp_fp
{
	template<typename A, typename B>
	struct SF
	{
		std::function<B( const double deltaMs, const A& a )> f;
		template<typename C>
		// compose two SF ( this >>> sf )
		SF<A, C> operator > ( SF<B, C> sf )
		{
			return compose( *this, sf );
		}
		// compose two SF ( sf >>> *this )
		template<typename C>
		SF<C, B> operator < ( SF<C, A> sf )
		{
			return compose( sf, *this );
		}
		// compose SF and constant value ( a >>> *this )
		SF<void, B> operator <= ( const A& a )
		{
			return compose( constant( a ), *this );
		}
	};
	template<typename B>
	struct SF < void, B >
	{
		std::function<B( const double deltaMs )> f;
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
			[&f]( const double deltaMs, const A& a )
			{
				return f( a );
			}
		};
	}
	template<typename A, typename B, typename C>
	SF<A, B> compose( const SF<A, B>& fst, const SF<B, C>& snd )
	{
		return SF < A, B > {
			[&fst, &snd]( const double deltaMs, const A& a )
			{
				return snd.f( deltaMs, fst.f( deltaMs, a ) );
			}
		};
	}
	template<typename B, typename C>
	SF<void, B> compose( const SF<void, B>& fst, const SF<B, C>& snd )
	{
		return SF < void, B > {
			[&fst, &snd]( const double deltaMs )
			{
				return snd.f( deltaMs, fst.f( deltaMs ) );
			}
		};
	}
	SF<FVec3, FVec3> integral( )
	{
		return SF < FVec3, FVec3 > {
			[]( const double deltaMs, const FVec3& a ) -> FVec3
			{
				return a * static_cast<float>( deltaMs );
			}
		};
	}
	template<typename B>
	SF<void, B> constant( const B& b )
	{
		return SF < void, B > {
			[&b]( const double deltaMs ) -> B
			{
				return B( b );
			}
		};
	}
}