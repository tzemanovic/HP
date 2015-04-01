#include <pch.hpp>
#include "../../../include/core/actor/actor.hpp"
#include "../../../include/math/mat4x4.hpp"
namespace hp_ip
{
	void Actor::init( Resources& resources, Renderer* pRenderer )
	{
		for ( auto* component : _components )
		{
			component->vInit( resources, pRenderer );
		}
		for ( auto& child : _children )
		{
			child.init( resources, pRenderer );
		}
	}
	void Actor::update( const float deltaMs )
	{
		_transformComponent.vUpdate( deltaMs );
		Mat4x4 transform = _transformComponent.transform( );
		for ( auto* component : _components )
		{
			component->vUpdate( deltaMs );
		}
		for ( auto& child : _children )
		{
			child.transformComponent( ).setParentTransform( transform );
			child.update( deltaMs );
		}
	}
	void Actor::render( Renderer* pRenderer )
	{
		for ( auto* component : _components )
		{
			component->vRender( pRenderer );
		}
		for ( auto& child : _children )
		{
			child.render( pRenderer );
		}
	}
	void Actor::addComponent( iComponent* component )
	{
		_components.push_back( component );
		component->_owner = this;
	}
	void Actor::addComponent( TransformComponent&& component )
	{
		_transformComponent = std::forward<TransformComponent>( component );
		_transformComponent._owner = this;
	}
}