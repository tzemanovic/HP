#pragma once
namespace hp_ip
{
	typedef void* TypeId;
	template<typename A>
	// get type id without RTTI
	TypeId typeId( )
	{
		static A* typeUniqueMarker = NULL;
		return &typeUniqueMarker;
	}
}