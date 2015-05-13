// Includes
#include <functional>
#include <tuple>

// Two-dimensional vector is a product type
struct Vector2
{
	float x, y;
};

// Type identifitication without RTTI
typedef void* TypeId;

template<typename A>
TypeId typeId( )
{
	static A* typeUniqueMarker = NULL;
	return &typeUniqueMarker;
}

// Example of sum type
struct BuiltInModelDef;
struct LoadedModelDef;
struct ModelDef
{
private:
	TypeId _typeId;
public:
	union
	{
		BuiltInModelDef builtIn;
		LoadedModelDef loaded;
	};
	template<typename A>
	bool is( ) const
	{
		return ( _typeId == typeId<A>( ) );
	}
	friend ModelDef builtInModelDef( BuiltInModelDef&& m );
	friend ModelDef loadedModelDef( LoadedModelDef&& m );
};

// Example of a sum type's factory function
ModelDef builtInModelDef( BuiltInModelDef&& m )
{
	ModelDef model;
	model.builtIn = m;
	model._typeId = typeId<BuiltInModelDef>( );
	return model;
}

// Unit type implementation
struct Unit
{ };

// C style function
struct A
{ };
struct B
{ };
B f_1( A )
{
	return B{ };
}
B( *fPtr_1 )( A ) = f_1;
// Callable object
struct f_2
{
	B operator () ( A )
	{
		return B{ };
	}
};
f_2 fPtr_2{ };
// C++11 function definition with trailing return type
auto f_3( A ) -> B
{
	return B{ };
};
B( *fPtr_3 )( A ) = f_3;
// C++11 lambda function
auto fPtr_4 = []( A ) -> B
{
	return B{ };
};
// STL function wrapper
auto fPtr_5 = std::function < B( A ) >
{
	[]( A )
	{
		return B{ };
	}
};

// Generic function in C++
template<A, B>
A fst( std::tuple<A, B> pair )
{
	return std::get<0>( pair );
}

// List is a recursive type
template<typename A>
struct List
{
	struct Cons
	{
		A head;
		List<A> tail;
	};
	Cons* cons;
};

// Naive C++ implementation of IO type
template<typename A>
struct IO
{
	typedef std::function<A( )> type;
};

// Naive implementation of maybe type
template<typename A>
struct Maybe
{
	union
	{
		Unit nothing;
		A something;
	};
};

// Simpler implementation of maybe type
template<typename A>
struct Maybe
{
	A* something;
};

// Maybe's visitor function
template<typename A, typename B, typename C>
auto ifThenElse( const Maybe<A>& maybe, B ifJust, C ifNothing ) ->
decltype( ifJust( *maybe._a ) )
{
	static_assert( std::is_same < decltype( ifJust( *maybe._a ) ),
		decltype( ifNothing( ) ) > ::value );
	if ( maybe._a == nullptr )
	{
		return ifNothing( );
	}
	return ifJust( *maybe._a );
}

// Maybe's visitor function alternative implementation
template<typename A, typename B>
B ifThenElse1( const Maybe<A>& maybe, B( *ifJust )( const A& a ), B( *ifNothing )( ) );

// Another Maybe's visitor function alternative implementation
template<typename A, typename B>
B ifThenElse2( const Maybe<A>& maybe, std::function<B( A )> ifJust, std::function<B( A )> ifNothing );

// Signal implementation
template<typename A>
struct S
{
	std::function<A( const float )> f;
};

// Signal Function implementation
template<typename A, typename B>
struct SF
{
	std::function<S<B>( const S<A>& a )> f;
};

// Application of signal to a signal function and composition of two signal functions
template<typename A, typename B>
struct SF
{
	std::function<S<B>( const S<A>& a )> f;

	S<B> operator < ( const S<A>& a ) const
	{
		return f( a );
	}
	SF<C, B> operator < ( SF<C, A>& sf ) const
	{
		return SF < A, C > {
			[sf, *this]( const S<A>& a ) -> S < C >
			{
				return snd.f( fst.f( a ) );
			}
		};
	}
};

struct GameInput;
struct ActorState;
// Actor input and output
struct ActorInput
{
	GameInput gameInput;
	ActorState state;
};
struct ActorOutput
{
	ActorState state;
};