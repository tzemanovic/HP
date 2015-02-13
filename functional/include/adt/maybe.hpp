#pragma once

namespace hp_fp
{
	template<typename A>
	struct Maybe
	{
		template<typename B>
		friend const Maybe<B> Just( const B* a );

		template<typename B>
		friend const Maybe<B> Nothing( );

	private:
		Maybe( const A* a ) : _a( a )
		{

		}
		Maybe( ) : _a( nullptr )
		{

		}
	public:
		~Maybe( )
		{
			HP_DELETE( _a );
		}
	private:
		const A* _a;
	};

	template<typename B>
	const Maybe<B> Just( const B* a )
	{
		return Maybe<B>( a );
	}

	template<typename B>
	const Maybe<B> Nothing( )
	{
		return Maybe<B>( );
	}
}