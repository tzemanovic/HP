#pragma once
#include "sf2.hpp"
#include "../../math/vec3.hpp"
namespace hp_fp
{
	template<typename B>
	SF<void, B> constant( const B& b )
	{
		return SF < void, B > {
			[&b]( const S<void> a ) -> S < B >
			{
				return signal( b, a.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> add( const S<A>& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a.val + b.val, b.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> add( const A& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a + b.val, b.deltaMs );
			}
		};
	}
	SF<FVec3, FVec3> integral( );
}