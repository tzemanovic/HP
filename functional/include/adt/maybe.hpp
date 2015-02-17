#pragma once

namespace hp_fp
{
	template<typename Type>
	struct Maybe
	{
		template<typename TypeB>
		friend const Maybe<TypeB> Just( const TypeB* a );

		template<typename TypeB>
		friend const Maybe<TypeB> Nothing( );

	private:
		Maybe( const Type* a ) : _a( a )
		{

		}
		Maybe( ) : _a( nullptr )
		{

		}
	private:
		const Type* _a;
	};

	template<typename Type>
	const Maybe<Type> Just( const Type* a )
	{
		return Maybe<Type>( a );
	}

	template<typename Type>
	const Maybe<Type> Nothing( )
	{
		return Maybe<Type>( );
	}
}