#pragma once
namespace hp_fp
{
	template<typename A>
	struct Maybe
	{
		template<typename B> friend Maybe<B> just( B&& b );
		template<typename B> friend Maybe<B> nothing( );
		template<typename B, typename C, typename D> friend auto ifThenElse( const Maybe<B>& maybe, C ifJust, D ifNothing ) -> decltype( ifJust( *maybe._a ) );
		Maybe( const Maybe<A>& ) = delete;
		Maybe( Maybe&& m ) : _a( std::move( m._a ) )
		{
			m._a = nullptr;
		}
		Maybe<A> operator = ( const Maybe<A>& ) = delete;
		Maybe<A> operator = ( Maybe<A>&& m )
		{
			return Maybe<A>( std::move( m ) );
		}
		~Maybe( )
		{
			HP_DELETE( _a );
		}
	private:
		Maybe( A&& a ) : _a( HP_NEW A( std::move( a ) ) )
		{ }
		Maybe( ) : _a( nullptr )
		{ }
	private:
		A* _a;
	};
	template<typename A>
	Maybe<A> just( A&& a )
	{
		return Maybe<A>( std::move( a ) );
	}
	template<typename A>
	Maybe<A> nothing( )
	{
		return Maybe<A>( );
	}
	template<typename A, typename B, typename C>
	auto ifThenElse( const Maybe<A>& maybe, B ifJust, C ifNothing ) -> decltype( ifJust( *maybe._a ) )
	{
		//static_assert( std::is_function<B>::value, "ifJust has to be a function." );
		//static_assert( std::is_function<decltype( ifNothing )>::value, "ifNothing has to be a function." );
		static_assert( std::is_same<decltype( ifJust( *maybe._a ) ), decltype( ifNothing( ) )>::value, "ifJust and ifNothing functions' return types have to be the same." );
		if ( maybe._a == nullptr )
		{
			return ifNothing( );
		}
		return ifJust( *maybe._a );
	}
}

