#pragma once
namespace hp_fp
{
	template<typename A>
	struct S
	{
		S( std::function<A( const float )> f ) : f( f )
		{ }
		S( const S& s ) : f( s.f )
		{ }
		S( S&& s ) : f( std::move( s.f ) )
		{ }
		S operator = ( const S& s )
		{
			return S{ s };
		}
		S operator = ( S&& s )
		{
			return S{ std::move( s ) };
		}
		std::function<A( const float )> f;
		A operator () ( const float deltaMs ) const
		{
			return f( deltaMs );
		}
		A operator < ( const float deltaMs ) const
		{
			return f( deltaMs );
		}
		A apply( const float deltaMs ) const
		{
			return f( deltaMs );
		}
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A>
	S<A> constant( const A& a )
	{
		return S < A >
		{
			[a]( const float deltaMs ) -> A
			{
				return a;
			}
		};
	}
}

