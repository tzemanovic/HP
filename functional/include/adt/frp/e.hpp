#pragma once
#include "../maybe.hpp"
namespace hp_fp
{
	template<typename A>
	using E = Maybe<A>;
	template<typename A>
	E<A> e( A&& a )
	{
		return E<A>( std::move( a ) );
	}
	template<typename A>
	E<A> noE( )
	{
		return E<A>( );
	}
}