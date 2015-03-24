#pragma once
namespace hp_fp
{
	template<typename A>
	struct S
	{
		A val;
		double deltaMs;
	};
	template<>
	struct S < void >
	{
		double deltaMs;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A>
	S<A> signal( const A& a )
	{
		return S < A > { a };
	}
	template<typename A>
	S<A> signal( const A&& a )
	{
		return S < A > { std::move( a ) };
	}
	template<typename A>
	S<A> signal( const A& a, const double deltaMs )
	{
		return S < A > { a, deltaMs };
	}
	template<typename A>
	S<A> signal( const A&& a, const double deltaMs )
	{
		return S < A > { std::move( a ), deltaMs };

	}
}