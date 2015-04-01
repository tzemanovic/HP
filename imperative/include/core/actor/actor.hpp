#pragma once
#include <vector>
#include "component/iComponent.hpp"
#include "component/transformComponent.hpp"
namespace hp_ip
{
	class Actor
	{
	public:
		Actor( TransformComponent&& transform )
			: _transformComponent( std::forward<TransformComponent>( transform ) )
		{ }
		Actor( Actor&& actor ) : _transformComponent( std::move( actor._transformComponent ) ),
			_components( std::move( actor._components ) ),
			_children( std::move( actor._children ) )
		{ }
		~Actor( )
		{
		//TODO: fix this
			for ( iComponent* component : _components )
			{
				component->_owner = nullptr;
				HP_DELETE( component );
			}
		}
		void init( Resources& resources, Renderer* pRenderer );
		void update( const float deltaMs );
		void render( Renderer* pRenderer );
		void addComponent( iComponent* component );
		void addComponent( TransformComponent&& component );
	private:
		TransformComponent _transformComponent;
		std::vector<iComponent*> _components;
		std::vector<Actor> _children;
	public:
		TransformComponent& transformComponent( )
		{
			return _transformComponent;
		}
		void addChild( Actor&& actor )
		{
			_children.push_back( std::forward<Actor>( actor ) );
		}
	};
}