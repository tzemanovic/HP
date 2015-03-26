#pragma once
namespace hp_fp
{
	template<typename A>
	struct S
	{
		// TODO: change to std::function<A(const float)> f;
		A val;
		float deltaMs;
	};
	template<>
	struct S < void >
	{
		float deltaMs;
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
	S<A> signal( const A& a, const float deltaMs )
	{
		return S < A > { a, deltaMs };
	}
	template<typename A>
	S<A> signal( const A&& a, const float deltaMs )
	{
		return S < A > { std::move( a ), deltaMs };

	}
}