#include <pch.hpp>
#include "../../../include/core/actor/actor.hpp"
namespace hp_ip
{
	void Actor::init( )
	{
		for ( auto& component : _components )
		{
			component.vInit( );
		}
	}
	void Actor::addComponent( iComponent&& component )
	{
		_components.push_back( std::move( component ) );
	}
}