#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
using namespace hp_fp;
SF<ActorInput, ActorOutput> ballSF( )
{
	return arr<ActorInput, ActorOutput>( []( const S<ActorInput>& input )
	{
		S<ActorState> state = input.val.state;
		static const float speed = 0.01f;
		static const float rotSpeed = 0.005f;
		if ( input.val.gameInput.val[Key::W] )
		{
			state.val.vel.val.z += speed;
		}
		if ( input.val.gameInput.val[Key::S] )
		{
			state.val.vel.val.z -= speed;
		}
		if ( input.val.gameInput.val[Key::Q] )
		{
			state.val.vel.val.x -= speed;
		}
		if ( input.val.gameInput.val[Key::E] )
		{
			state.val.vel.val.x += speed;
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
		// velocity damping
		state.val.vel = mul<FVec3>( 0.5f ) < state.val.vel;
		// translation
		state.val.pos = add( state.val.pos ) < rotate( state.val.rot )
			< integral( ) < state.val.vel;
		return ActorOutput{ state };
	} );
}
SF<ActorInput, ActorOutput> staticActorSF( )
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
		return[projection]( Renderer& renderer, const ActorOutput& output,
			const Mat4x4& transform ) mutable
		{
			setCamera_IO( renderer.cameraBuffer, { projection,
				trasformMatFromActorState( output.state.val ) * transform } );
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
				FQuat::identity // rot
			},
			ballSF( ), // sf
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
						FQuat::identity // rot
					},
					staticActorSF( ), // sf
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
					"assets/textures/ground/ground6_color.jpg", // diffuseTextureFilename
					"assets/textures/ground/ground6_spec.jpg", // specularTextureFilename
					"assets/textures/ground/ground6_normal.jpg", // bumpTextureFilename
					"", //"assets/textures/ground/ground6_height.jpg", // parallaxTextureFilename
					"", // evnMapTextureFilename
					{ 150.0f, 150.0f } // textureRepeat
				}
			} ),
			{ // startingState
				{ 0.0f, 0.0f, 0.0f }, // pos
				{ 0.0f, 0.0f, 0.0f }, // vel
				{ 1.0f, 1.0f, 1.0f }, // scl
				FQuat::identity // rot
			},
			staticActorSF( ), // sf
			{ } // children
		}
	};


	Engine engine = init( "example1" );
	run_IO( engine, std::move( actors ) );
	return 0;
}