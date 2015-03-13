#include <pch.hpp>
#include <core/engine.hpp>
#include <functional>
#include <adt/maybe.hpp>
#include <core/timer.hpp>
#include <core/actors/actorInputMut.hpp>
#include <graphics/mesh.hpp>
#include <graphics/meshMat.hpp>
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

	template<typename A, typename B>
	// sense - game input sample function
	// actuate - process output sample
	// sf - signal function
	void reactimate_IO( A( *sense )( void ), void( *actuate )( B ), B( *sf )( A ) )
	{
		actuate( sf( sense( ) ) );
	}
	typedef ActorOutput( *actor_sf )( ActorInputMut& );
	std::function<ActorOutput( ActorInputMut& )> simpleGun( float x0, float y0 )
	{
		return [=]( ActorInputMut& input )
		{
			return ActorOutput{ input.gameInput.mouse.x * 0.1f + x0, y0 };
		};
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

				// should be function that takes renderer and returns rendering function
				auto renderGun = []( RendererMut& renderer )
				{
					// use renderer to load the mesh and material and then pass ref to lambda
					Maybe<MeshesMut> meshes = loadModelFromFile_IO(
						"Assets/Models/Characters/Knight/knight.fbx", 0.009f );
					ifThenElse( meshes, [&renderer]( MeshesMut& meshes )
					{
						MeshMatMut meshMat = defaultMat( );
						if ( init_IO( meshMat, renderer ) )
						{
							if ( loadTexture_IO( meshMat.diffuseTexture, renderer,
								"Assets/Textures/Characters/Knight/T_Black_Knight_D.jpg" ) )
							{
								if ( loadTexture_IO( meshMat.bumpTexture, renderer,
									"Assets/Textures/Characters/Knight/T_Black_Knight_N.jpg" ) )
								{
									return [&]( ActorOutput& output )
									{
										/*setProjection( meshMat, camera.getProjection( ) );
										pMeshMaterial->VSetProjection( pCamera->GetProjection( ) );
										pMeshMaterial->VSetView( pCamera->GetView( ) );
										pMeshMaterial->VSetWorld( m_toWorld );

										pMeshMaterial->VSetAbientLightColor( pScene->GetAmbientLightColor( ) );
										pMeshMaterial->VSetDiffuseLightColor( pScene->GetDiffuseLightColor( ) );
										pMeshMaterial->VSetSpecularLightColor( pScene->GetSpecularLightColor( ) );
										pMeshMaterial->VSetLightDirection( pScene->GetLightDirection( ) );

										pMeshMaterial->VSetCameraPosition( pCamera->GetWorldPosition( ) );

										pMeshMaterial->VSetTextures( );
										pMeshMaterial->VSetMaterials( );

										pMeshMaterial->VBindInputLayout( );
										for ( uint32 i = 0; i < pMeshMaterial->VGetPassCount( ); ++i )
										{
										pMeshMaterial->VApplyPass( i );
										for ( uint32 j = 0; j < pMeshComponent->GetGeometryCount( ); ++j )
										{
										MeshGeometry* pMeshGeometry = pMeshComponent->GetGeometry( j );
										pMeshGeometry->SetBuffers( );
										g_pGame->GetRenderer( )->DrawIndexed( pMeshGeometry->GetIndexCount( ), 0, 0 );
										}
										}*/
									};
								}
								else
								{
									ERR( "Failed to load T_Black_Knight_N.jpg texture" );
								}
							}
							else
							{
								ERR( "Failed to load T_Black_Knight_D.jpg texture" );
							}

						}
						else
						{
							ERR( "Failed to initalize knight.fbx material" );
						}
					}, [] // nothing<MeshesMut>
					{
						return [&]( ActorOutput& output )
						{

						};
						ERR( "Failed to load knight.fbx model." );
					} );
					return [&]( ActorOutput& output )
					{

					};
				};

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
					ActorInputMut actorInput{ engine.gameInput };
					//engine.sf( actorInput );
					/*auto sf = []( ActorInputMut& input )
					{
					return ActorOutput{ input.gameInput.mouse.x * 0.1f, 0.0f };
					};*/

					auto render = [&renderer]( ActorOutput& output )
					{
						preRender_IO( renderer );

						//std::cout << output.x << std::endl;
						present_IO( renderer );
					};

					render( simpleGun( 10.0f, 5.0f )( actorInput ) );
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