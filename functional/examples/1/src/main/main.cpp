#include <pch/pch.hpp>
#ifdef HP_DEBUG
#include <vld.h>
#endif
#include <hpFp.hpp>
using namespace hp_fp;
SF<FVec3, FVec3> clampMag( const float max )
{
	return arrAlt<FVec3, FVec3>( [max]( const FVec3& vel ) -> FVec3
	{
		return clampMag( vel, max );
	} );
}
FQuat rollBall( const FVec3& vel )
{
	return eulerRadToQuat( FVec3{ vel.z, 0.0f, -vel.x } *0.5f );
}
ActorState getState( const ActorInput& input )
{
	return input.state;
}
GameInput getGameInput( const ActorInput& input )
{
	return input.gameInput;
}
ActorOutput conActorOutput( const ActorState& state )
{
	return ActorOutput{ state };
}
typedef bool( *GetI )( const GameInput& );
std::function<bool( const GameInput& )> getInputState( const Key key )
{
	return [key]( const GameInput& gi )
	{
		return gi[key];
	};
}
template<typename A>
std::function<A( const bool )> ifElse( const A& a, const A&b )
{
	return [a, b]( const bool cond )
	{
		return cond ? a : b;
	};
}
std::function<FVec3( const FVec3& )> minY( const float minY )
{
	return [minY]( const FVec3& vec )
	{
		return FVec3{ vec.x, vec.y < minY ? minY : vec.y, vec.z };
	};
}
std::function<float( const float )> clamp( const float min, const float max )
{
	return [min, max]( const float x )
	{
		return x < min ? min : ( x > max ? max : x );
	};
}
std::function<float( const float )> clampToZero( const float min )
{
	return [min]( const float x )
	{
		return abs( x ) < min ? 0.0f : x;
	};
}
float getX( const FVec3& vec )
{
	return vec.x;
}
float getY( const FVec3& vec )
{
	return vec.y;
}
float getZ( const FVec3& vec )
{
	return vec.z;
}
std::function<FVec3( const float )> conFVec3fromX( )
{
	return []( const float x )
	{
		return FVec3{ x, 0.0f, 0.0f };
	};
}
std::function<FVec3( const float )> conFVec3fromY( )
{
	return []( const float y )
	{
		return FVec3{ 0.0f, y, 0.0f };
	};
}
std::function<FVec3( const float )> conFVec3fromZ( )
{
	return []( const float z )
	{
		return FVec3{ 0.0f, 0.0f, z };
	};
}
FVec3 getVel( const ActorState& state )
{
	return state.vel;
}
FVec3 getPos( const ActorState& state )
{
	return state.pos;
}
FQuat getRot( const ActorState& state )
{
	return state.rot;
}
FQuat getModelRot( const ActorState& state )
{
	return state.modelRot;
}
SF<ActorState, ActorState> setPos( const S<FVec3>& posS )
{
	return SF < ActorState, ActorState > {
		[posS]( const S<ActorState>& stateS )
		{
			return S < ActorState >
			{
				[posS, stateS]( const float deltaMs ) -> ActorState
				{
					auto state = stateS < deltaMs;
					state.pos = posS < deltaMs;
					return state;
				}
			};
		}
	};
}
SF<ActorState, ActorState> setVel( const S<FVec3>& velS )
{
	return SF < ActorState, ActorState > {
		[velS]( const S<ActorState>& stateS )
		{
			return S < ActorState >
			{
				[velS, stateS]( const float deltaMs ) -> ActorState
				{
					auto state = stateS < deltaMs;
					state.vel = velS < deltaMs;
					return state;
				}
			};
		}
	};
}
SF<ActorState, ActorState> setRot( const S<FQuat>& rotS )
{
	return SF < ActorState, ActorState > {
		[rotS]( const S<ActorState>& stateS )
		{
			return S < ActorState >
			{
				[rotS, stateS]( const float deltaMs ) -> ActorState
				{
					auto state = stateS < deltaMs;
					state.rot = rotS < deltaMs;
					return state;
				}
			};
		}
	};
}
SF<ActorState, ActorState> setModelRot( const S<FQuat>& modelRotS )
{
	return SF < ActorState, ActorState > {
		[modelRotS]( const S<ActorState>& stateS )
		{
			return S < ActorState >
			{
				[modelRotS, stateS]( const float deltaMs ) -> ActorState
				{
					auto state = stateS < deltaMs;
					state.modelRot = modelRotS < deltaMs;
					return state;
				}
			};
		}
	};
}
SF<FVec3, FVec3> setY( const S<float>& yS )
{
	return SF < FVec3, FVec3 > {
		[yS]( const S<FVec3>& vecS )
		{
			return S < FVec3 >
			{
				[yS, vecS]( const float deltaMs ) -> FVec3
				{
					auto vec = vecS < deltaMs;
					vec.y = yS < deltaMs;
					return vec;
				}
			};
		}
	};
}
template<typename A>
std::function<bool( const A& )> lt( const A& a )
{
	return [a]( const A& b )
	{
		return b < a;
	};
}
template<typename A>
std::function<bool( const A& )> eq( const A& a )
{
	return [a]( const A& b )
	{
		return b == a;
	};
}
template<typename A>
std::function<bool( const A& )> lte( const A& a )
{
	return [a]( const A& b )
	{
		return b <= a;
	};
}
template<typename A>
std::function<bool( const A& )> ht( const A& a )
{
	return [a]( const A& b )
	{
		return b > a;
	};
}
template<typename A>
std::function<bool( const A& )> hte( const A& a )
{
	return [a]( const A& b )
	{
		return b >= a;
	};
}
SF<bool, bool> and( const S<bool>& aS )
{
	return SF < bool, bool > {
		[aS]( const S<bool>& bS )
		{
			return S < bool >
			{
				[aS, bS]( const float deltaMs ) -> bool
				{
					return aS( deltaMs ) && bS( deltaMs );
				}
			};
		}
	};
}
SF<bool, bool> or( const S<bool>& aS )
{
	return SF < bool, bool > {
		[aS]( const S<bool>& bS )
		{
			return S < bool >
			{
				[aS, bS]( const float deltaMs ) -> bool
				{
					return aS( deltaMs ) || bS( deltaMs );
				}
			};
		}
	};
}
float ab( const float& a )
{
	return abs( a );
}
SF<ActorInput, ActorOutput> bouncingBall( )
{
	return SF < ActorInput, ActorOutput >
	{
		[]( const S<ActorInput>& input ) -> S < ActorOutput >
		{
			static const float rotSpeed = 0.003f;
			auto state = arr( getState ) < input;
			auto gi = arr( getGameInput ) < input;
			S<FQuat> rot = mul( arr( getRot ) < state ) < arr( eulerRadToQuat<float> )
				< arrAlt<float, FVec3>( conFVec3fromY( ) )
				< integral<float>( ) < rotSpeed;
			//S<float> velY = arr( getY ) < arr( getVel ) < state;
			S<float> velY =
				// add last velY
				add( arr( getY ) < arr( getVel ) < state )
				// add gravity
				< integral<float>( ) < -0.00000981f;
			S<float> velY2 =
				// set velY to zero if posY is lower than 0.45f
				mul( arrAlt<bool, float>( ifElse( -1.0f, 1.0f ) )
				< and( arrAlt<float, bool>( lte( 0.45f ) )
				< arr( getY ) < arr( getPos ) < state )
				< arrAlt<float, bool>( lt( 0.0f ) ) < velY ) < velY;
			S<FVec3> vel = arrAlt<float, FVec3>( conFVec3fromY( ) ) < velY2;
			S<FVec3> pos = arrAlt<FVec3, FVec3>( minY( 0.45f ) )
				< add( integral<FVec3>( ) < vel )
				< arr( getPos ) < state;
			return arr( conActorOutput ) < setRot( rot ) < setVel( vel )
				< setPos( pos ) < state;
		}
	};
}
SF<ActorInput, ActorOutput> ball( )
{
	return SF < ActorInput, ActorOutput >
	{
		[]( const S<ActorInput>& input ) -> S < ActorOutput >
		{
			static const float acceleration = 0.001f;
			static const float jumpSpeed = 0.01f;
			static const float rotSpeed = 0.003f;
			static const float minPosX = 0.45f;
			static const float gravity = -0.00000981f;
			static const float cor = -0.5f; // coefficient of restitution
			auto state = arr( getState ) < input;
			auto gi = arr( getGameInput ) < input;
			S<bool> onTheGround = arrAlt<float, bool>( eq( minPosX ) )
				< arr( getY ) < arr( getPos ) < state;
			S<float> accZForward = arrAlt<bool, float>( ifElse( acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::W ) ) < gi;
			S<float> accZBackward = arrAlt<bool, float>( ifElse( -acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::S ) ) < gi;
			S<float> accXLeft = arrAlt<bool, float>( ifElse( -acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::Q ) ) < gi;
			S<float> accXRight = arrAlt<bool, float>( ifElse( acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::E ) ) < gi;
			S<float> angVelZLeft = arrAlt<bool, float>( ifElse( -rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::A ) ) < gi;
			S<float> angVelZRight = arrAlt<bool, float>( ifElse( rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::D ) ) < gi;
			S<float> velYUp = arrAlt<bool, float>( ifElse( jumpSpeed, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::Space ) ) < gi;
			// rotation
			S<FQuat> rot = mul( arr( getRot ) < state ) < arr( eulerRadToQuat<float> )
				< arrAlt<float, FVec3>( conFVec3fromY( ) )
				< integral<float>( ) < add( angVelZLeft ) < angVelZRight;
			// acceleration
			S<FVec3> accZ = arrAlt<float, FVec3>( conFVec3fromZ( ) )
				< add( accZForward ) < accZBackward;
			S<FVec3> accX = arrAlt<float, FVec3>( conFVec3fromX( ) )
				< add( accXLeft ) < accXRight;
			S<FVec3> acc = add( accZ ) < accX;


			/*
			switch on (edge < pos <= 0) event (tagged with pos and vel)
			that either calls bouncingBall with lower vel to rebounce
			or stops the ball if vel is too low

			*/

			/*auto addEvent = []( const float y )
			{
			if ( y < minPosX )
			{
			return std::make_tuple( minPosX, just( 0 ) );
			}
			return std::make_tuple( y, nothing<int>( ) );
			};

			std::function<SF<float, std::tuple<float, E<int>>>( const S<float>& )> fallingBall =
			[addEvent, state]( const S<float>& velY )
			{
			return arrAlt<float, std::tuple<float, E<int>>>( addEvent )
			< add( integral<float>( ) < add( velY )
			< integral<float>( ) < -0.000000981f );
			};

			std::function < std::function<SF<float, float>( const int )>( const S<float>& ) >
			bouncingBall;
			bouncingBall =
			[&fallingBall, &bouncingBall]( const S<float>& velY )
			{
			return std::function<SF<float, float>( const int )>(
			[velY, &fallingBall, &bouncingBall]( const int i )
			{
			return sw( fallingBall( velY ),
			bouncingBall( mul( -0.5f ) < velY ) );
			} );
			};
			auto ball = bouncingBall( ( arr( getY ) < arr( getVel ) < state ) )( 0 );

			//auto ball = sw(fallingBall, bouncingBall); */

			S<float> velY =
				// add last velY
				add( arr( getY ) < arr( getVel ) < state )
				// add gravity and jump
				< add( velYUp ) < integral<float>( ) < gravity;
			S<float> velY2 =
				// set velY to zero if posY is lower than minPosX
				mul( arrAlt<bool, float>( ifElse( cor, 1.0f ) )
				< and( arrAlt<float, bool>( lte( minPosX ) )
				< arr( getY ) < arr( getPos ) < state )
				< arrAlt<float, bool>( lt( 0.0f ) ) < velY ) < velY;
			// stop ball from jumping when velocity is low
			S<float> velY3 = mul( arrAlt<bool, float>( ifElse( 0.0f, 1.0f ) )
				< and( arrAlt<float, bool>( lte( minPosX ) )
				< arr( getY ) < arr( getPos ) < state )
				< arrAlt<float, bool>( lt( 0.001f ) ) < arr( ab ) < velY2 ) < velY2;

			// add integral of acceleration to velocity, then damp and clamp it
			S<FVec3> vel = setY( velY3 ) < clampMag( 0.01f ) < mul<FVec3>( 0.999f )
				< add( integral<FVec3>( ) < acc ) < arr( getVel ) < state;
			// add oriented integral of velocity to position
			S<FVec3> pos = arrAlt<FVec3, FVec3>( minY( minPosX ) )
				< add( rotate( rot ) < integral<FVec3>( ) < vel )
				< arr( getPos ) < state;

			auto newState = setRot( rot ) < setVel( vel )
				< setPos( pos ) < state;
			// model rotation
			auto modelRot = mul( arr( getModelRot ) < newState ) < arr( rollBall )
				< integral<FVec3>( ) < vel;
			return arr( conActorOutput ) < setModelRot( modelRot ) < newState;

			//return S < ActorOutput >
			//{
			//	[input]( const float deltaMs )
			//	{
			//		ActorInput ai = input < deltaMs;
			//		ActorState state = ai.state;
			//		static const float acceleration = 0.001f;
			//		static const float rotSpeed = 0.005f;
			//		FVec3 acc = FVec3::zero;
			//		if ( ai.gameInput[Key::W] )
			//		{
			//			acc.z = acceleration;
			//		}
			//		if ( ai.gameInput[Key::S] )
			//		{
			//			acc.z = -acceleration;
			//		}
			//		if ( ai.gameInput[Key::Q] )
			//		{
			//			acc.x = -acceleration;
			//		}
			//		if ( ai.gameInput[Key::E] )
			//		{
			//			acc.x = acceleration;
			//		}
			//		float rotZ = 0.0f;
			//		if ( ai.gameInput[Key::A] )
			//		{
			//			rotZ -= deltaMs * rotSpeed;
			//		}
			//		if ( ai.gameInput[Key::D] )
			//		{
			//			rotZ += deltaMs * rotSpeed;
			//		}
			//		// rotation
			//		state.rot = mul( state.rot ) < eulerRadToQuat( FVec3{ 0.0f, rotZ, 0.0f } )
			//			< deltaMs;
			//		// add integral of acceleration to velocity, then damp and clamp it
			//		state.vel = clampMag( 0.01f ) < mul<FVec3>( 0.9f )
			//			< add( state.vel ) < integral<FVec3>( ) < acc < deltaMs;
			//		// add integral of velocity to position
			//		state.pos = add( state.pos ) < rotate( state.rot )
			//			< integral<FVec3>( ) < state.vel < deltaMs;
			//		// model rotation
			//		state.modelRot = mul( state.modelRot ) < arr( rollBall ) < state.vel
			//			< deltaMs;
			//		return ActorOutput{ state };
			//	}
			//};
		}
	};
}
//SF<ActorInput, ActorOutput> ball( )
//{
//	return arr<ActorInput, ActorOutput>( []( const S<ActorInput>& input )
//	{
//		S<ActorState> state = input.val.state;
//		static const float acceleration = 0.001f;
//		static const float rotSpeed = 0.005f;
//		FVec3 acc;
//		if ( input.val.gameInput.val[Key::W] )
//		{
//			acc.z = acceleration;
//		}
//		if ( input.val.gameInput.val[Key::S] )
//		{
//			acc.z = -acceleration;
//		}
//		if ( input.val.gameInput.val[Key::Q] )
//		{
//			acc.x = -acceleration;
//		}
//		if ( input.val.gameInput.val[Key::E] )
//		{
//			acc.x = acceleration;
//		}
//		float rotZ = 0.0f;
//		if ( input.val.gameInput.val[Key::A] )
//		{
//			rotZ -= input.deltaMs * rotSpeed;
//		}
//		if ( input.val.gameInput.val[Key::D] )
//		{
//			rotZ += input.deltaMs * rotSpeed;
//		}
//		// rotation
//		state.val.rot = mul( state.val.rot )
//			< signal( eulerRadToQuat( FVec3{ 0.0f, rotZ, 0.0f } ), input.deltaMs );
//		// add integral of acceleration to velocity, then damp and clamp it
//		state.val.vel = clampMag( -0.01f, 0.01f ) < mul<FVec3>( 0.9f )
//			< add( state.val.vel ) < integral( ) < signal( acc, input.deltaMs );
//		// add integral of velocity to position
//		state.val.pos = add( state.val.pos ) < rotate( state.val.rot )
//			< integral( ) < state.val.vel;
//		// model rotation
//		state.val.modelRot = mul( state.val.modelRot ) < rollBall( ) < state.val.vel;
//		return ActorOutput{ state };
//	} );
//}
SF<ActorInput, ActorOutput> staticActor( )
{
	return arr<ActorInput, ActorOutput>( []( const ActorInput& input )
	{
		return ActorOutput{ input.state };
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
					BuiltInModelType::Cube, // type
					{ 500.0f, 0.1f, 500.0f } // dimensions
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
	//const int I = 64;
	//const int I_HALF = 32;
	//for ( int i = 0; i < I; ++i )
	//{
	//	float y = ( abs( i - I_HALF ) / static_cast<float>( I_HALF ) );
	//	actors.push_back( {
	//		actorModelDef( {
	//			loadedModelDef( { // model
	//				"assets/models/basketball/basketball.fbx", // filename
	//				0.02f // scale
	//			} ),
	//			{ // material
	//				"assets/textures/basketball/basketball-diffuse.jpg", // diffuseTextureFilename
	//				"", // specularTextureFilename
	//				"assets/textures/basketball/basketball-bump.jpg", // bumpTextureFilename
	//				"", // parallaxTextureFilename
	//				"" // evnMapTextureFilename
	//			}
	//		} ),
	//		{ // startingState
	//			{ sinf( i * TWO_PI_F / I ) * 10.0f, 5.0f * y + 0.45f, cosf( i * TWO_PI_F / I ) * 10.0f }, // pos
	//			{ 0.0f, 0.0f, 0.0f }, // vel
	//			{ 1.0f, 1.0f, 1.0f }, // scl
	//			FQuat::identity, // rot
	//			FQuat::identity // modelRot
	//		},
	//		bouncingBall( ), // sf
	//		{ } // children
	//	} );
	//}


	Engine engine = init( "example1" );
	run_IO( engine, std::move( actors ) );
	return 0;
}

