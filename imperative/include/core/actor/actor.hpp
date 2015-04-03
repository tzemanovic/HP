#pragma once
#include <vector>
#include "component/iComponent.hpp"
#include "component/transformComponent.hpp"
#include "../../window/gameInput.hpp"
namespace hp_ip
{
	class Actor
	{
	public:
		//Actor( TransformComponent&& transform )
		//	//: _transformComponent( /*std::forward<TransformComponent>(*/ transform/* )*/ )
		//{
		//	_pTransformComponent = HP_NEW TransformComponent( transform );
		//}
		Actor( TransformComponent* pTransform )
		{
			_pTransformComponent = pTransform;
		}
		//Actor( Actor&& actor ) : _transformComponent( /*std::move(*/ actor._transformComponent/* )*/ ),
		//	_components( std::move( actor._components ) ),
		//	_children( std::move( actor._children ) )
		//{
		//	actor._components.clear( );
		//	_transformComponent._owner = this;
		//	for ( auto& component : _components )
		//	{
		//		component->_owner = this;
		//	}
		//}
		~Actor( )
		{ }
		void init( Resources& resources, Renderer* pRenderer );
		void update( const float deltaMs, const GameInput& input );
		void render( Renderer* pRenderer );
		void addComponent( iComponent* component );
		void addComponent( TransformComponent* component );
	private:
		TransformComponent* _pTransformComponent;
		std::vector<iComponent*> _components;
		std::vector<Actor> _children;
	public:
		TransformComponent& transformComponent( )
		{
			return *_pTransformComponent;
		}
		void addChild( Actor&& actor )
		{
			_children.push_back( std::forward<Actor>( actor ) );
		}
	};
}