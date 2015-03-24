#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
#include <graphics/material.hpp>
#include <math/frustum.hpp>
//#include <adt/frp/sf.hpp>
#include <adt/frp/sf2.hpp>
#include <adt/frp/sfs.hpp>
using namespace hp_fp;
//SF<ActorInput, ActorOutput> ballSF2( )
//{
//	return arr<ActorInput, ActorOutput>( []( const Signal<ActorInput>& input )
//	{
//		ActorState state = input.val.state;
//		//FVec3 pos = add( state.pos ) < integral( ) <= state.vel;
//		FVec3 pos = ( add( state.pos ) < integral( ) <= state.vel )
//			.f( Signal < void > { input.deltaMs } ).val;
//		//integral( ) <= state.vel;
//		return ActorOutput{ state };
//	} );
//}
SF<ActorInput_S, ActorOutput_S> ballSF_S( )
{
	return arr<ActorInput_S, ActorOutput_S>( []( const S<ActorInput_S>& input )
	{
		S<ActorState_S> state = input.val.state;
		state.val.pos = add( state.val.pos ) < integral( ) < state.val.vel;
		//FVec3 pos = add( state.pos ) < integral( ) <= state.vel;
		/*FVec3 pos = ( add( state.val.pos ) < integral( ) <= state.vel )
			.f( Signal < void > { input.deltaMs } ).val;*/
		return ActorOutput_S{ state };
	} );
}
std::function<ActorOutput( const ActorInput& )> ballSF( )
{
	return []( const ActorInput& input )
	{
		ActorState outState = input.state;
		static const float speed = 0.001f;
		static const float rotSpeed = 0.001f;
		if ( input.gameInput[Key::W] )
		{
			outState.vel.z += speed;
		}
		if ( input.gameInput[Key::S] )
		{
			outState.vel.z -= speed;
		}
		float rotZ = 0.0f;
		if ( input.gameInput[Key::A] )
		{
			rotZ -= input.gameInput.deltaMs * rotSpeed;
		}
		if ( input.gameInput[Key::D] )
		{
			rotZ += input.gameInput.deltaMs * rotSpeed;
		}
		outState.vel -= outState.vel * 0.1f;
		outState.pos = input.state.pos + outState.vel * input.gameInput.deltaMs;
		// TODO: overload (operator =) on the actorOutput values to evaluate SFs?
		/*integral( ) <= outState.vel;
		constant( outState.vel ) > integral( );*/
		outState.rot = input.state.rot * eulerRadToQuat( FVec3{ 0.0f, rotZ, 0.0f } );
		return ActorOutput{ outState };
	};
}
// TODO: move into default SFs header
std::function<ActorOutput( const ActorInput& )> staticActorSF( )
{
	return []( const ActorInput& input )
	{
		return ActorOutput{ input.state };
	};
}
SF<ActorInput_S, ActorOutput_S> staticActorSF_S( )
{
	return arr<ActorInput_S, ActorOutput_S>( []( const S<ActorInput_S>& input )
	{
		return ActorOutput_S{ input.val.state };
	} );
}
InitCamRenderSF cameraRenderSF( )
{
	return []( const ActorCameraDef& cameraDef, const WindowConfigImm& windowConfig )
		-> CamRenderSF
	{
		const Frustum frustum = init( static_cast<float>( PI ) / 4.f,
			static_cast<float>( windowConfig.width ) / windowConfig.height,
			cameraDef.nearClipDist, cameraDef.farClipDist );
		const Mat4x4 projection = matrixPerspectiveFovLH( frustum.fieldOfView,
			frustum.aspectRatio, frustum.nearClipDist, frustum.farClipDist );
		return[projection]( Renderer& renderer, const ActorOutput_S& output,
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
						cameraRenderSF( ) // render
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
					{ 250.0f, 250.0f } // textureRepeat
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


	std::vector<ActorDef_S> actors_S
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
			ballSF_S( ), // sf
			{ // children
				{
					actorCameraDef( {
						0.001f, // nearClipDist
						1000.0f, // farClipDist
						cameraRenderSF( ) // render
					} ),
					{ // startingState
						{ 0.0f, 2.0f, -7.0f }, // pos
						{ 0.0f, 0.0f, 0.0f }, // vel
						{ 1.0f, 1.0f, 1.0f }, // scl
						FQuat::identity // rot
					},
					staticActorSF_S( ), // sf
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
					{ 250.0f, 250.0f } // textureRepeat
				}
			} ),
			{ // startingState
				{ 0.0f, 0.0f, 0.0f }, // pos
				{ 0.0f, 0.0f, 0.0f }, // vel
				{ 1.0f, 1.0f, 1.0f }, // scl
				FQuat::identity // rot
			},
			staticActorSF_S( ), // sf
			{ } // children
		}
	};


	Engine engine = init( "example1", std::move( actors_S ) );
	run_IO( engine );
	return 0;
}