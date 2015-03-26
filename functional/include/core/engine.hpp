#pragma once
#include "actor/actor.hpp"
#include "../window/gameInput.hpp"
#include "../window/window.hpp"
namespace hp_fp
{
	enum struct EngineState : UInt8
	{
		Initialized,
		Running,
		Terminated
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  +  ][  +  ]
	struct Engine
	{
		Engine( String&& name, EngineState&& state, GameInput&& gameInput )
			: name( std::move( name ) ), state( std::move( state ) ),
			gameInput( std::move( gameInput ) )
		{ }
		Engine( const Engine& ) = delete;
		Engine( Engine&& e ) : name( std::move( e.name ) ), state( std::move( e.state ) ),
			gameInput( std::move( e.gameInput ) )
		{ }
		Engine operator = ( const Engine& ) = delete;
		Engine operator = ( Engine&& e )
		{
			return Engine{ std::move( e ) };
		}
		const String name;
		EngineState state;
		GameInput gameInput;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	Engine init( String&& name );
	void run_IO( Engine& engine, std::vector<ActorDef>&& actorDefs,
		const WindowConfig& windowConfig = defaultWindowConfig_IO( ) );
	namespace
	{
		void renderActors_IO( Renderer& renderer, std::vector<Actor>& actors,
			const GameInput& gameInput, const float deltaMs,
			const Mat4x4& parentLocalTransform = Mat4x4::identity );
		std::vector<Actor> initActors_IO( Renderer& renderer, Resources& resources,
			std::vector<ActorDef>&& actorsDef );
	}
}