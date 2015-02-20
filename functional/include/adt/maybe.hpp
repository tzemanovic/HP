#pragma once

namespace hp_fp
{
	template<typename A>
	struct Maybe
	{
		template<typename B> friend Maybe<B> just( B* b );
		template<typename B> friend Maybe<B> nothing( );
		template<typename B, typename C> friend C ifThenElse( const Maybe<B>& maybe, std::function<C( B& )> ifJust, std::function<C( )> ifNothing );
		//Maybe( const Maybe<A>& ) = delete;
	private:
		Maybe( A* a ) : _a( a )
		{

		}
		Maybe( ) : _a( nullptr )
		{

		}
		Maybe( Maybe&& m ) : _a( m._a )
		{ }
	private:
		A* _a;
	};

	template<typename A>
	Maybe<A> just( A* a )
	{
		return Maybe<A>( a );
	}

	template<typename A>
	Maybe<A> nothing( )
	{
		return Maybe<A>( );
	}

	template<typename A, typename B>
	B ifThenElse( const Maybe<A>& maybe, std::function<B( A& )> ifJust, std::function<B( )> ifNothing )
	{
		if ( maybe._a == nullptr )
		{
			return ifNothing( );
		}
		return ifJust( *maybe._a );
	}

}