#include <pch.hpp>
#include "../../include/core/engine.hpp"
#include <functional>
#include "../../include/adt/maybe.hpp"
#include "../../include/core/timer.hpp"
#include "../../include/core/actor/actor.hpp"
#include "../../include/graphics/mesh.hpp"
#include "../../include/graphics/renderer.hpp"
#include "../../include/math/frustum.hpp"
namespace hp_fp
{
	Engine init( String&& name, ActorsDef&& actors )
	{
		return Engine{ std::move( name ), EngineState::Initialized, { }, std::move( actors ) };
	}
	template<typename A, typename B>
	// sense - game input sample function
	// actuate - process output sample
	// sf - signal function
	void reactimate_IO( A( *sense )( void ), void( *actuate )( B ), B( *sf )( A ) )
	{
		actuate( sf( sense( ) ) );
	}
	void renderActors_IO( Renderer& renderer, ActorInput& actorInput, Actors& actors,
		const Mat4x4& parentLocalTransTo = Mat4x4::identity )
	{
		auto actorOutput = actors.sf( actorInput );
		actors.render_IO( renderer, actorOutput );
		for ( Actors child : actors.children )
		{
			renderActors_IO( renderer, actorInput, child );
		}
	}
	Actors initActors_IO( Renderer& renderer, const ActorsDef& actorsDef )
	{
		std::vector<Actors> children{ actorsDef.children.size( ) };
		for ( auto child : actorsDef.children )
		{
			children.push_back( initActors_IO( renderer, child ) );
		}
		return Actors{ actorsDef.sf, initActorRenderFunction_IO( renderer, actorsDef ),
			children };
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
				Timer timer = initTimer_IO( );
				Actors actors = initActors_IO( renderer, engine.actors );
				// camera
				Frustum frustum = init( static_cast<float>( PI ) / 4.f,
					1980.0f / 1024.0f, 0.001f, 1000.f );
				Mat4x4 projection = matrixPerspectiveFovLH( frustum.fieldOfView, frustum.aspectRatio, frustum.nearClipDist, frustum.farClipDist );
				Mat4x4 camView = Mat4x4::identity;
				camView.SetPosition( FVec3( 0.0f, 2.0f, -7.0f ) );
				FVec3 camPos = camView.GetPosition( );
				camView = inverse( camView );
				while ( engine.state == EngineState::Running )
				{
					// TODO: collect all the inputs with time into a signal state GameInput that will be wrapped in ActorInput
					// ActorOutput will have Transform (mandatory), Sum type with Actor specific optional components such as Mesh or RigidBody, and Events that can change the dynamic collection of Actors
					// make Actors signal functions - one for update (from ActorInput to ActorOutput), one for rendering

					// TOOD: sf should take ActorInput and return scene graph, that gets passed to render function
					processMessages_IO( window.handle );
					updateTimer_IO( timer );
					engine.gameInput.deltaMs = timer.deltaMs;
					engine.gameInput.timeMs = timer.timeMs( );
					// camera props need to be a part of actor output to use in shaders
					ActorInput actorInput{ engine.gameInput, projection, camView, camPos };
					preRender_IO( renderer );
					/*auto actorOutput = actors.sf( actorInput );
					actors.render_IO( renderer, actorOutput );
					for ( Actors child : actors.children )
					{
					child.render_IO( renderer, child.sf( actorInput ) );
					}*/
					renderActors_IO( renderer, actorInput, actors );
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
}