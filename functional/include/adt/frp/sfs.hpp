#pragma once
#include <tuple>
#include "e.hpp"
#include "sf.hpp"
#include "../../math/vec3.hpp"
#include "../../math/quat.hpp"
namespace hp_fp
{
	template<typename A>
	SF<A, A> add( const S<A>& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) + b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> add( const A& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a + b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> sub( const S<A>& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) - b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> sub( const A& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a - b( deltaMs );
					}
				}
			}
		};
	}
	template<typename A>
	SF<A, A> mul( const S<A>& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) * b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> mul( const A& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a * b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A, typename B>
	SF<A, A> mul( const S<B>& b )
	{
		return SF < A, A > {
			[b]( const S<A>& a )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) * b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A, typename B>
	SF<A, A> mul( const B& b )
	{
		return SF < A, A > {
			[b]( const S<A>& a )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) * b;
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> div( const S<A>& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a( deltaMs ) / b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> div( const A& a )
	{
		return SF < A, A > {
			[a]( const S<A>& b )
			{
				return S < A >
				{
					[a, b]( const float deltaMs ) -> A
					{
						return a / b( deltaMs );
					}
				};
			}
		};
	}
	template<typename A>
	SF<A, A> integral( )
	{
		return SF < A, A >
		{
			[]( const S<A>& a ) -> S < A >
			{
				return S < A >
				{
					[a]( const float deltaMs ) -> A
					{
						return a( deltaMs ) * deltaMs;
					}
				};
			}
		};
	}
	SF<FVec3, FVec3> rotate( const S<FQuat>& rot );
	SF<FVec3, FVec3> rotate( const FQuat& rot );

	template<typename A, typename B, typename C>
	SF<A, B> sw( const SF<A, std::tuple<B, E<C>>>& sf, std::function<SF<A, B>( C )> f )
	{
		return SF < A, B > {
			[sf, f]( const S<A>& a )
			{
				return S < B >
				{
					[sf, f, a]( const float deltaMs ) -> B
					{
						//std::tuple<B, E<C>> sfRes = sf < a < deltaMs;
						return ifThenElse( std::get<1>( sf < a < deltaMs ),
							[f, a, deltaMs]( const C& c )
						{
							return f( c ) < a < deltaMs;
						},
							[sf, a, deltaMs]( )
						{
							return std::get<0>( sf < a < deltaMs );
						} );
					}
				};
			}
		};
	}
}