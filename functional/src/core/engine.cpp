#include <pch.hpp>
#include "../../include/core/engine.hpp"
#include <functional>
#include "../../include/adt/maybe.hpp"
#include "../../include/core/resources.hpp"
#include "../../include/core/timer.hpp"
#include "../../include/core/actor/actor.hpp"
#include "../../include/graphics/renderer.hpp"
#include "../../include/math/frustum.hpp"
namespace hp_fp
{
	Engine init( String&& name )
	{
		return Engine{ std::move( name ), EngineState::Initialized, { } };
	}
	void run_IO( Engine& engine, std::vector<ActorDef>&& actorDefs,
		const WindowConfig& windowConfig )
	{
		Maybe<Window> window = open_IO( engine, windowConfig );
		ifThenElse( window, [&engine, &windowConfig, &actorDefs]( Window& window )
		{
			Maybe<Renderer> renderer = init_IO( window.handle, windowConfig );
			ifThenElse( renderer, [&engine, &window, &actorDefs]( Renderer& renderer )
			{
				engine.state = EngineState::Running;
				Resources resources;
				Timer timer = initTimer_IO( );
				std::vector<Actor> actors = initActors_IO( renderer, resources,
					std::move( actorDefs ) );
				while ( engine.state == EngineState::Running )
				{
					processMessages_IO( window.handle );
					updateTimer_IO( timer );
					preRender_IO( renderer );
					renderActors_IO( renderer, actors, engine.gameInput,
						static_cast<float>( timer.deltaMs ) );
					present_IO( renderer );
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
	namespace
	{
		void renderActors_IO( Renderer& renderer, std::vector<Actor>& actors,
			const GameInput& gameInput, const float deltaMs,
			const Mat4x4& parentLocalTransform )
		{
			for ( auto& actor : actors )
			{
				ActorInput actorInput{
					signal( gameInput, deltaMs ),
					signal( ActorState{
						signal( actor.state.val.pos.val, deltaMs ),
						signal( actor.state.val.vel.val, deltaMs ),
						signal( actor.state.val.scl.val, deltaMs ),
						signal( actor.state.val.rot.val, deltaMs )
					}, deltaMs )
				};
				auto actorOutput = actor.sf( signal( actorInput, deltaMs ) );
				actor.state = actorOutput.val.state;
				actor.render_IO( renderer, actorOutput.val, parentLocalTransform );
				renderActors_IO( renderer, actor.children, gameInput, deltaMs,
					trasformMatFromActorState( actor.state.val ) );
			}
		}
		std::vector<Actor> initActors_IO( Renderer& renderer, Resources& resources,
			std::vector<ActorDef>&& actorsDef )
		{
			std::vector<Actor> actors{ };
			actors.reserve( actorsDef.size( ) );
			for ( auto& actorDef : actorsDef )
			{
				ActorState startingState{
					signal( actorDef.startingState.pos, 0.0 ),
					signal( actorDef.startingState.vel, 0.0 ),
					signal( actorDef.startingState.scl, 0.0 ),
					signal( actorDef.startingState.rot, 0.0 )
				};
				actors.push_back( Actor{ signal( startingState, 0.0 ), actorDef.sf,
					initActorRenderFunction_IO( renderer, resources, actorDef ),
					initActors_IO( renderer, resources, std::move( actorDef.children ) ) } );
			}
			return actors;
		}
	}
}