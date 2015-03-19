#pragma once
#include "actor/actor.hpp"
#include "../window/gameInputMut.hpp"
#include "../window/window.hpp"
namespace hp_fp
{
	enum class EngineState : UInt8
	{
		Initialized,
		Running,
		Terminated
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  0  ][  0  ]
	struct Engine
	{
		Engine( String&& name, EngineState&& state, GameInputMut&& gameInput,
			ActorsDef&& actors )
			: name( std::move( name ) ), state( std::move( state ) ),
			gameInput( std::move( gameInput ) ), actors( std::move( actors ) )
		{ }
		Engine( const Engine& ) = delete;
		Engine( Engine&& e ) : name( std::move( e.name ) ), state( std::move( e.state ) ),
			gameInput( std::move( e.gameInput ) ), actors( std::move( e.actors ) )
		{ }
		Engine operator=( const Engine& ) = delete;
		Engine operator=( Engine&& e )
		{
			return Engine{ std::move( e ) };
		}
		const String name;
		EngineState state;
		GameInputMut gameInput;
		ActorsDef actors;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/
	Engine init( String&& name, ActorsDef&& actors );
	void run_IO( Engine& engine, const WindowConfigImm& windowConfig =
		defaultWindowConfig_IO( ) );
}