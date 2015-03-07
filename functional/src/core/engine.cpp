#include <pch.hpp>
#include <core/engine.hpp>
#include <core/timer.hpp>
#include <core/actors/actorInputMut.hpp>
#include <adt/maybe.hpp>
#include <graphics/renderer.hpp>
namespace hp_fp
{
	EngineMut init( String&& name )
	{
		return EngineMut{ std::move( name ), EngineState::Initialized, { }, nullptr };
	}
	void setWorld_IO( EngineMut& engine, WorldImm&& world )
	{
		engine.world = HP_NEW WorldImm( world );
	}
	void run_IO( EngineMut& engine, const WindowConfigImm& windowConfig )
	{
		Maybe<WindowMut> window = open_IO( engine, windowConfig );
		ifThenElse( window, [&engine, &windowConfig]( WindowMut& window )
		{
			Maybe<RendererMut> renderer = init_IO( window.handle, windowConfig );
			ifThenElse( renderer, [&engine, &window]( RendererMut& renderer )
			{
				engine.state = EngineState::Running;
				TimerMut timer = initTimer_IO( );
				ActorInputMut actorInput{ };
				while ( engine.state == EngineState::Running )
				{
					// TODO: collect all the inputs with time into a signal state GameInput that will be wrapped in ActorInput
					// ActorOutput will have Transform (mandatory), Sum type with Actor specific optional components such as Mesh or RigidBody, and Events that can change the dynamic collection of Actors
					// make Actors signal functions - one for update (from ActorInput to ActorOutput), one for rendering
					processMessages_IO( window.handle );
					updateTimer_IO( timer );
					engine.gameInput.deltaMs = timer.deltaMs;
					engine.gameInput.timeMs = timer.timeMs();
					ActorInputMut actorInput{ engine.gameInput };
					//render_IO( timer );
					//update_IO( timer );
				}
			}, []
			{
				ERR( "Failed to initialize renderer." );
			} );
		}, []
		{
			ERR( "Failed to open a window." );
		} );
	}
}