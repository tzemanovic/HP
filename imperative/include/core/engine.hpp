#pragma once
#include <vector>
#include "resources.hpp"
#include "actor/actor.hpp"
#include "../graphics/renderer.hpp"
#include "../window/gameInput.hpp"
#include "../window/window.hpp"
namespace hp_ip
{
	class Renderer;
	enum class EngineState : UInt8
	{
		Initialized,
		Running,
		Terminated
	};
	class Engine
	{
	public:
		Engine( String&& name, EngineState&& state )
			: _name( std::move( name ) ), _state( std::move( state ) ),
			_pWindow( nullptr )
		{ }
		Engine( String&& name ) : Engine( std::move( name ), EngineState::Initialized )
		{ }
		~Engine( )
		{
			HP_DELETE( _pRenderer );
			HP_DELETE( _pWindow );
		}
		void run( const WindowConfig& windowConfig = Window::defaultWindowConfig( ) );
		void addActor( Actor&& actor );
	private:
		const String _name;
		EngineState _state;
		GameInput _gameInput;
		Resources _resources;
		Window* _pWindow;
		Renderer* _pRenderer;
		std::vector<Actor> _actors;
	};
}

