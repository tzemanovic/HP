#pragma once
#include "../utils/typeId.hpp"
namespace hp_fp
{
	template<typename TypesHead, typename... TypesTail>
	struct SumHelper
	{
		inline static void destroy( const TypeId id, void * data )
		{
			if ( id == typeId<TypesHead>( ) )
			{
				reinterpret_cast<TypesHead*>( data )->~TypesHead( );
			}
			else
			{
				SumHelper<TypesTail...>::destroy( id, data );
			}
		}
		inline static void copy( const TypeId id, const void* oldData, void* newData )
		{
			if ( id == typeId<TypesHead>( ) )
			{
				new (newData) TypesHead( *reinterpret_cast<const TypesHead*>( oldData ) );
			}
			else
			{
				SumHelper<TypesTail...>::copy( id, oldData, newData );
			}
		}
		static const size_t val = sizeof( TypesHead ) > SumHelper<TypesTail...>::val ? sizeof( TypesHead ) : SumHelper<TypesTail...>::val;
	};
	template<typename Type>
	struct SumHelper < Type >
	{
		inline static void destroy( const TypeId id, void * data )
		{
			reinterpret_cast<Type*>( data )->~Type( );
		}
		inline static void copy( const TypeId id, const void* oldData, void* newData )
		{

			const Type* t = reinterpret_cast<const Type*>( oldData );
			new (newData) Type( *t );
		}
		static const size_t val = sizeof( Type );
	};
	template<typename... Types>
	struct Sum
	{
	public:
		template<typename Type>
		explicit Sum( Type&& value )
		{
			new ( &_data ) Type( std::forward<Type>( value ) );
			_typeId = typeId<Type>( );
		}
		template<typename Type>
		explicit Sum( const Type& value )
		{
			new ( &_data ) Type( value );
			_typeId = typeId<Type>( );
		}
		Sum( const Sum& s ) : _typeId( s._typeId )
		{
			SumHelper<Types...>::copy( s._typeId, &s._data, &_data );
		}
		Sum( Sum&& s ) : _typeId( std::move( s._typeId ) ), _data( std::move( s._data ) )
		{ }
		Sum operator = ( const Sum& s )
		{
			return Sum( s );
		}
		Sum operator = ( Sum&& s )
		{
			return Sum( std::move( s ) );
		}
		~Sum( )
		{
			SumHelper<Types...>::destroy( _typeId, &_data );
		}
		template<typename Type>
		bool is( ) const
		{
			return ( _typeId == typeId<Type>( ) );
		}
		template<typename Type>
		Type& get( ) const
		{
			return reinterpret_cast<Type&>( _data );
		}
	private:
		//const static size_t size = SumHelper<Types...>::val;
		TypeId _typeId;
	public:
		std::aligned_union<SumHelper<Types...>::val, Types...> _data;
	};
}
// TODO: might have to add deleter like so
//struct placement_deleter
//{
//	template <typename T>
//	void operator () ( T* ptr ) const
//	{
//		ptr->~T( );
//	}
//};
//
//template <typename T>
//using stack_ptr = std::unique_ptr<T, placement_deleter>;

