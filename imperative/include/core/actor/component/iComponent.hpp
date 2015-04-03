#pragma once
#include "../../../window/gameInput.hpp"
namespace hp_ip
{
	enum class ComponentType : UInt8
	{
		Transform, Other, Count
	};
	class Resources;
	class Renderer;
	class Actor;
	class iComponent
	{
	public:
		friend class Actor;
		iComponent( ComponentType type = ComponentType::Other ) : _owner( nullptr ),
			_type( type )
		{ }
		virtual ~iComponent( )
		{ }
		virtual void vInit( Resources& resources, Renderer* pRenderer )
		{ }
		virtual void vUpdate( const float deltaMs, const GameInput& input )
		{ }
		virtual void vRender( Renderer* pRenderer )
		{ }
	protected:
		Actor* _owner;
	private:
		ComponentType _type;
	};
}