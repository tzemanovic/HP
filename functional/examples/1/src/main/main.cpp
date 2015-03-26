#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
using namespace hp_fp;
SF<FVec3, FVec3> clampMag( const float min, const float max )
{
	return arr<FVec3, FVec3>( [=]( const S<FVec3>& vel ) -> FVec3
	{
		return clampMag( vel.val, min, max );
	} );
}
SF<FVec3, FQuat> rollBall( )
{
	return arr<FVec3, FQuat>( []( const S<FVec3>& vel )
	{
		return eulerRadToQuat( FVec3{ vel.val.z, 0.0f, -vel.val.x } *10.0f );
	} );
}
SF<ActorInput, ActorOutput> ball( )
{
	return arr<ActorInput, ActorOutput>( []( const S<ActorInput>& input )
	{
		S<ActorState> state = input.val.state;
		static const float acceleration = 0.001f;
		static const float rotSpeed = 0.005f;
		FVec3 acc;
		if ( input.val.gameInput.val[Key::W] )
		{
			acc.z = acceleration;
		}
		if ( input.val.gameInput.val[Key::S] )
		{
			acc.z = -acceleration;
		}
		if ( input.val.gameInput.val[Key::Q] )
		{
			acc.x = -acceleration;
		}
		if ( input.val.gameInput.val[Key::E] )
		{
			acc.x = acceleration;
		}
		float rotZ = 0.0f;
		if ( input.val.gameInput.val[Key::A] )
		{
			rotZ -= input.deltaMs * rotSpeed;
		}
		if ( input.val.gameInput.val[Key::D] )
		{
			rotZ += input.deltaMs * rotSpeed;
		}
		// rotation
		state.val.rot = mul( state.val.rot )
			< signal( eulerRadToQuat( FVec3{ 0.0f, rotZ, 0.0f } ), input.deltaMs );
		// add integral of acceleration to velocity, then damp and clamp it
		state.val.vel = clampMag( -0.01f, 0.01f ) < mul<FVec3>( 0.9f )
			< add( state.val.vel ) < integral( ) < signal( acc, input.deltaMs );
		// add integral of velocity to position
		state.val.pos = add( state.val.pos ) < rotate( state.val.rot )
			< integral( ) < state.val.vel;
		// model rotation
		state.val.modelRot = mul( state.val.modelRot ) < rollBall( ) < state.val.vel;
		return ActorOutput{ state };
	} );
}
SF<ActorInput, ActorOutput> staticActor( )
{
	return arr<ActorInput, ActorOutput>( []( const S<ActorInput>& input )
	{
		return ActorOutput{ input.val.state };
	} );
}
InitCamRenderFn initCameraRenderFn( )
{
	return []( const ActorCameraDef& cameraDef, const WindowConfig& windowConfig )
		-> CamRenderFn
	{
		const Frustum frustum = init( static_cast<float>( PI ) / 4.f,
			static_cast<float>( windowConfig.width ) / windowConfig.height,
			cameraDef.nearClipDist, cameraDef.farClipDist );
		const Mat4x4 projection = matrixPerspectiveFovLH( frustum.fieldOfView,
			frustum.aspectRatio, frustum.nearClipDist, frustum.farClipDist );
		return[projection]( Renderer& renderer, const ActorState& state,
			const Mat4x4& transform ) mutable
		{
			setCamera_IO( renderer.cameraBuffer, { projection,
				trasformMatFromActorState( state ) * transform } );
		};
	};
}
int main( )
{
	std::vector<ActorDef> actors
	{
		{
			actorModelDef( {
				loadedModelDef( { // model
					"assets/models/basketball/basketball.fbx", // filename
					0.02f // scale
				} ),
				{ // material
					"assets/textures/basketball/basketball-diffuse.jpg", // diffuseTextureFilename
					"", // specularTextureFilename
					"assets/textures/basketball/basketball-bump.jpg", // bumpTextureFilename
					"", // parallaxTextureFilename
					"" // evnMapTextureFilename
				}
			} ),
			{ // startingState
				{ 0.0f, 0.45f, 0.0f }, // pos
				{ 0.0f, 0.0f, 0.0f }, // vel
				{ 1.0f, 1.0f, 1.0f }, // scl
				FQuat::identity, // rot
				FQuat::identity // modelRot
			},
			ball( ), // sf
			{ // children
				{
					actorCameraDef( {
						0.001f, // nearClipDist
						1000.0f, // farClipDist
						initCameraRenderFn( ) // render
					} ),
					{ // startingState
						{ 0.0f, 2.0f, -7.0f }, // pos
						{ 0.0f, 0.0f, 0.0f }, // vel
						{ 1.0f, 1.0f, 1.0f }, // scl
						FQuat::identity, // rot
						FQuat::identity // modelRot
					},
					staticActor( ), // sf
					{ } // children
				}
			}
		},
		{
			actorModelDef( {
				builtInModelDef( { // model
					BuiltInModelType::Box, // type
					{ 500.0f, 0.1f, 500.0f } // scale
				} ),
				{ // material
					"assets/textures/ground/OrangeHerringbone-ColorMap.png", // diffuseTextureFilename
					"", // specularTextureFilename
					"assets/textures/ground/OrangeHerringbone-NormalMap.png", // bumpTextureFilename
					"", // parallaxTextureFilename
					"", // evnMapTextureFilename
					{ 250.0f, 250.0f } // textureRepeat
				}
			} ),
			{ // startingState
				{ 0.0f, 0.0f, 0.0f }, // pos
				{ 0.0f, 0.0f, 0.0f }, // vel
				{ 1.0f, 1.0f, 1.0f }, // scl
				FQuat::identity, // rot
				FQuat::identity // modelRot
			},
			staticActor( ), // sf
			{ } // children
		}
	};
	for ( int i = 0; i < 10; ++i )
	{
		for ( int j = 0; j < 10; ++j )
		{
			actors.push_back( {
				actorModelDef( {
					loadedModelDef( { // model
						"assets/models/basketball/basketball.fbx", // filename
						0.02f // scale
					} ),
					{ // material
						"assets/textures/basketball/basketball-diffuse.jpg", // diffuseTextureFilename
						"", // specularTextureFilename
						"assets/textures/basketball/basketball-bump.jpg", // bumpTextureFilename
						"", // parallaxTextureFilename
						"" // evnMapTextureFilename
					}
				} ),
				{ // startingState
					{ 0.0f + i*1.0f, 0.45f, 0.0f + j*1.0f }, // pos
					{ 0.0f, 0.0f, 0.0f }, // vel
					{ 1.0f, 1.0f, 1.0f }, // scl
					FQuat::identity, // rot
					FQuat::identity // modelRot
				},
				staticActor( ), // sf
				{ } // children
			} );
		}
	}


	Engine engine = init( "example1" );
	run_IO( engine, std::move( actors ) );
	return 0;
}