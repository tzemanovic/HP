#pragma once
#include "sf.hpp"
#include "../../math/vec3.hpp"
#include "../../math/quat.hpp"
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
	template<typename A>
	SF<A, A> sub( const S<A>& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a.val - b.val, b.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> sub( const A& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a - b.val, b.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> mul( const S<A>& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a.val * b.val, b.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> mul( const A& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a * b.val, b.deltaMs );
			}
		};
	}
	template<typename A, typename B>
	SF<A, A> mul( const B& b )
	{
		return SF < A, A > {
			[&b]( const S<A>& a )
			{
				return signal( a.val * b, a.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> div( const S<A>& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a.val / b.val, b.deltaMs );
			}
		};
	}
	template<typename A>
	SF<A, A> div( const A& a )
	{
		return SF < A, A > {
			[&a]( const S<A>& b )
			{
				return signal( a / b.val, b.deltaMs );
			}
		};
	}
	SF<FVec3, FVec3> integral( );
	SF<FVec3, FVec3> rotate( const S<FQuat>& rot );
}