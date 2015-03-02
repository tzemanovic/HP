#pragma once
namespace hp_fp
{
	typedef void* TypeId;
	template<class T>
	TypeId TypeIdNoRTTI( )
	{
		static T* TypeUniqueMarker = NULL;
		return &TypeUniqueMarker;
	}

	template<typename TypesHead, typename... TypesTail>
	struct SumHelper
	{
		inline static void destroy( TypeId id, void * data )
		{
			if ( id == TypeIdNoRTTI<TypesHead>( ) )
				reinterpret_cast< TypesHead* >( data )->~TypesHead( );
			else
				SumHelper<TypesTail...>::destroy( id, data );
		}
		static const size_t val = sizeof( TypesHead ) > SumHelper<TypesTail...>::val ? sizeof( TypesHead ) : SumHelper<TypesTail...>::val;
	};

	template<typename Type>
	struct SumHelper < Type >
	{
		inline static void destroy( TypeId id, void * data )
		{ }
		static const size_t val = sizeof( Type );
	};

	template<typename... Types>
	struct Sum
	{
	public:
		template<typename Type>
		explicit Sum( Type&& value )
		{
			new ( &data ) Type( std::forward<Type>( value ) );
			typeId = TypeIdNoRTTI<Type>( );
		}

		~Sum( )
		{
			SumHelper<Types...>::destroy( typeId, &data );
		}
	public:
		//const static size_t size = SumHelper<Types...>::val;
		TypeId typeId;
		std::aligned_union<SumHelper<Types...>::val, Types...> data;
	};
}