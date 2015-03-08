#pragma once
namespace hp_fp
{
	template<typename A, typename B>
	struct SF
	{
		B( *f )( A& a );
	};
	template<typename A, typename B>
	SF<A, B> arr( B( *f )( A& a ) )
	{
		return SF{ f };
	}
	template<typename A, typename B, typename C>
	SF<A, B> compose( SF<A, B> fst, SF<B, C> snd )
	{
		return SF{ [=]( A a )
		{
			return snd.f( fst.f( a ) );
		} };
	}
}