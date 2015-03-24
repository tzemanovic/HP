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
	Engine init( String&& name, std::vector<ActorDef_S>&& actors )
	{
		return Engine{ std::move( name ), EngineState::Initialized, { }, std::move( actors ) };
	}
	void run_IO( Engine& engine, const WindowConfigImm& windowConfig )
	{
		Maybe<WindowMut> window = open_IO( engine, windowConfig );
		ifThenElse( window, [&engine, &windowConfig]( WindowMut& window )
		{
			Maybe<Renderer> renderer = init_IO( window.handle, windowConfig );
			ifThenElse( renderer, [&engine, &window]( Renderer& renderer )
			{
				engine.state = EngineState::Running;
				Resources resources;
				Timer timer = initTimer_IO( );
				std::vector<Actor> actors = initActors_IO( renderer, resources, engine.actors );
				// camera
				Frustum frustum = init( static_cast<float>( PI ) / 4.f,
					1980.0f / 1024.0f, 0.001f, 1000.f );
				Mat4x4 camProjection = matrixPerspectiveFovLH( frustum.fieldOfView, frustum.aspectRatio, frustum.nearClipDist, frustum.farClipDist );
				Mat4x4 camView = Mat4x4::identity;
				camView.SetPosition( FVec3{ 0.0f, 2.0f, -7.0f } );
				setCamera_IO( renderer.cameraBuffer, { camProjection, camView } );
				swap_IO( renderer.cameraBuffer );
				setCamera_IO( renderer.cameraBuffer, { camProjection, camView } );
				/*FVec3 camPos = camView.GetPosition( );
				camView = inverse( camView );*/
				while ( engine.state == EngineState::Running )
				{
					// TODO: collect all the inputs with time into a signal state GameInput that will be wrapped in ActorInput
					// ActorOutput will have Transform (mandatory), Sum type with Actor specific optional components such as Mesh or RigidBody, and Events that can change the dynamic collection of Actor
					// make Actor signal functions - one for update (from ActorInput to ActorOutput), one for rendering

					// TOOD: sf should take ActorInput and return scene graph, that gets passed to render function
					processMessages_IO( window.handle );
					updateTimer_IO( timer );
					engine.gameInput.deltaMs = timer.deltaMs;
					engine.gameInput.timeMs = timer.timeMs( );
					// camera props need to be a part of actor output to use in shaders
					//ActorInput actorInput{ engine.gameInput, camProjection, camView, camPos };
					preRender_IO( renderer );
					renderActors_IO( renderer, actors, engine.gameInput );
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
			const GameInputMut& gameInput, const Mat4x4& parentLocalTransform )
		{
			for ( auto& actor : actors )
			{
				// TODO: move the deltaMs from gameInput to this function's args
				// and apply it to all the signals
				ActorInput_S actorInput{
					signal( gameInput, gameInput.deltaMs ),
					signal( ActorState_S{
						signal( actor.state.val.pos.val, gameInput.deltaMs ),
						signal( actor.state.val.vel.val, gameInput.deltaMs ),
						signal( actor.state.val.scl.val, gameInput.deltaMs ),
						signal( actor.state.val.rot.val, gameInput.deltaMs )
					}, gameInput.deltaMs )
				};
				auto actorOutput = actor.sf( signal( actorInput, gameInput.deltaMs ) );
				actor.state = actorOutput.val.state;
				actor.render_IO( renderer, actorOutput.val, parentLocalTransform );
				renderActors_IO( renderer, actor.children, gameInput,
					trasformMatFromActorState( actor.state.val ) );
			}
		}
		std::vector<Actor> initActors_IO( Renderer& renderer, Resources& resources,
			const std::vector<ActorDef_S>& actorsDef )
		{
			std::vector<Actor> actors{ };
			actors.reserve( actorsDef.size( ) );
			for ( auto& actorDef : actorsDef )
			{
				ActorState_S startingState{
					signal( actorDef.startingState.pos, 0.0 ),
					signal( actorDef.startingState.vel, 0.0 ),
					signal( actorDef.startingState.scl, 0.0 ),
					signal( actorDef.startingState.rot, 0.0 )
				};
				actors.push_back( Actor{ signal( startingState, 0.0 ), actorDef.sf,
					initActorRenderFunction_IO( renderer, resources, actorDef ),
					initActors_IO( renderer, resources, actorDef.children ) } );
			}
			return actors;
		}
	}
}