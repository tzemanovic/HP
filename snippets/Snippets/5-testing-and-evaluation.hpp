// Includes


// Actor definition in FP version
auto actors = {
	actorModelDef( {
		loadedModelDef( { // model
			"basketball.fbx", // filename
			0.02f // scale
		} ),
		{ // material
			"basketball-diffuse.jpg", // diffuseTextureFilename
			"", // specularTextureFilename
			"basketball-bump.jpg", // bumpTextureFilename
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
};

struct GameInput;
// Getter of game input
GameInput getGameInput( const ActorInput& input )
{
	return input.gameInput;
}

enum Key
{
	W, S, A, D, Space, Q, E
};
template<typename A, typename B> struct SF;
template<typename A> struct S;
struct ActorInput;
struct ActorOutput;
template<typename A> struct FVec3;
struct FQuat;
// Signal function of the ball
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
			// coefficient of restitution
			static const float cor = -0.5f;
			auto state = arr( getState ) < input;
			auto gi = arr( getGameInput ) < input;
			// stop from going under minPosX
			S<bool> onTheGround =
				arrAlt<float, bool>( eq( minPosX ) )
				< arr( getY ) < arr( getPos ) < state;
			// controls
			// accelerate forward
			S<float> accZForward =
				arrAlt<bool, float>( ifElse( acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::W ) )
				< gi;
			// accelerate backward
			S<float> accZBackward =
				arrAlt<bool, float>( ifElse( -acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::S ) )
				< gi;
			// anti-clockwise angular velocity
			S<float> angVelZLeft =
				arrAlt<bool, float>( ifElse( -rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::A ) )
				< gi;
			// clockwise angular velocity
			S<float> angVelZRight =
				arrAlt<bool, float>( ifElse( rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::D ) )
				< gi;
			// jump
			S<float> velYUp =
				arrAlt<bool, float>( ifElse( jumpSpeed, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState(
				Key::Space ) ) < gi;


			/*
			...
			*/


			// rotation
			S<FQuat> rot = mul( arr( getRot ) < state )
				// Signal function of ball 2 of 2
				// rotation
				;			S<FQuat> rot = mul( arr( getRot ) < state )
				< arr( eulerRadToQuat<float> )
				< arrAlt<float, FVec3>( conFVec3fromY( ) )
				< integral<float>( ) < add( angVelZLeft )
				< angVelZRight;
			// acceleration
			S<FVec3> accZ = arrAlt<float, FVec3>( conFVec3fromZ( ) )
				< add( accZForward ) < accZBackward;
			// new actor state
			auto newState = setRot( rot ) < state;
			// model rotation
			auto modelRot = mul( arr( getModelRot ) < newState )
				< arr( rollBall ) < integral<FVec3>( ) < vel;
			// construct new actor output
			return arr( conActorOutput ) < setModelRot( modelRot )
				< newState;
		}
	};
}

// Higher-order function ifElse
template<typename A>
std::function<A( const bool )> ifElse( const A& a, const A&b )
{
	return [a, b]( const bool cond )
	{
		return cond ? a : b;
	};
}

struct A;
struct Actor;
struct ModelComponent;
struct BallTransformComponent;
struct TransformComponent;
struct CameraComponent;
// Actor definition in IP version
BallTransformComponent ballTransform{ // transform
	{ 0.0f, 0.45f, 0.0f }, // pos
	{ 0.0f, 0.0f, 0.0f }, // vel
	{ 1.0f, 1.0f, 1.0f }, // scl
	FQuat::identity, // rot
	FQuat::identity // modelRot
};
Actor ball( &ballTransform );
auto ballModel = ModelComponent( // model
	"basketball.fbx", // filename
	0.02f, // scale
	MaterialDef{ // material
	"basketball-diffuse.jpg", // diffuseTextureFilename
	"", // specularTextureFilename
	"basketball-bump.jpg", // bumpTextureFilename
	"", // parallaxTextureFilename
	"" // evnMapTextureFilename
} );
ball.addComponent( &ballModel );
TransformComponent cameraTransform{ // transform
	{ 0.0f, 2.0f, -7.0f }, // pos
	{ 0.0f, 0.0f, 0.0f }, // vel
	{ 1.0f, 1.0f, 1.0f }, // scl
	FQuat::identity, // rot
	FQuat::identity // modelRot
};
Actor camera( &cameraTransform );
auto cameraComp = CameraComponent( { // camera
	0.001f, // nearClipDist
	1000.0f, // farClipDist
} );
camera.addComponent( &cameraComp );
ball.addChild( std::move( camera ) );

// Custom derived class in IP version
struct BallTransformComponent : public TransformComponent
{
	virtual void vUpdate( const float deltaMs,
		const GameInput& input ) override
	{
		static const float acceleration = 0.001f;
		static const float jumpSpeed = 0.01f;
		static const float rotSpeed = 0.003f;
		static const float minPosX = 0.45f;
		static const float gravity = -0.00000981f;
		// coefficient of restitution
		static const float cor = -0.5f;
		FVec3 acc = FVec3::zero;
		float velYUp = 0.0f;
		if ( _pos.y == minPosX )
		{
			if ( input[Key::W] )
			{
				acc.z += acceleration;
			}
			if ( input[Key::S] )
			{
				acc.z += -acceleration;
			}
			if ( input[Key::Space] )
			{
				velYUp = jumpSpeed;
			}
		}
		if ( input[Key::A] )
		{
			angVel += -rotSpeed;
		}
		if ( input[Key::D] )
		{
			angVel += rotSpeed;
		}
		_rot = _rot *
			eulerRadToQuat( FVec3{ 0.0f, angVel * deltaMs, 0.0f } );
		_modelRot = _modelRot *
			eulerRadToQuat( FVec3{ intVel.z, 0.0f, -intVel.x } *
			0.5f );
	}
};

// One of the bottleneck signal functions in FP version
template<typename A, typename B, typename C>
SF<A, C> compose( const SF<A, B>& fst, const SF<B, C>& snd )
{
	return SF < A, C > {
		[fst, snd]( const S<A>& a ) -> S < C >
		{
			return snd.f( fst.f( a ) );
		}
	};
}


// presentation
SF<ActorInput, ActorOutput> ball( )
{
	return SF < ActorInput, ActorOutput >
	{
		[]( const S<ActorInput>& input ) -> S < ActorOutput >
		{
			static const float acceleration = 0.001f;
			static const float minPosX = 0.45f;
			auto state = arr( getState ) < input;
			auto gi = arr( getGameInput ) < input;
			S<bool> onTheGround = arr( eq( minPosX ) )
				< arr( getY ) < arr( getPos ) < state;
			S<float> accZForward = arr( ifElse( acceleration, 0.0f ) )
				< and( onTheGround ) 
				< arr( getInputState( Key::W ) ) < gi;

			static const float acceleration = 0.001f;
			static const float jumpSpeed = 0.01f;
			static const float rotSpeed = 0.003f;
			static const float minPosX = 0.45f;
			static const float gravity = -0.00000981f;
			// coefficient of restitution
			static const float cor = -0.5f;
			auto state = arr( getState ) < input;
			auto gi = arr( getGameInput ) < input;
			// stop from going under minPosX
			S<bool> onTheGround =
				arrAlt<float, bool>( eq( minPosX ) )
				< arr( getY ) < arr( getPos ) < state;
			// controls
			// accelerate forward
			S<float> accZForward =
				arrAlt<bool, float>( ifElse( acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::W ) )
				< gi;
			// accelerate backward
			S<float> accZBackward =
				arrAlt<bool, float>( ifElse( -acceleration, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState( Key::S ) )
				< gi;
			// anti-clockwise angular velocity
			S<float> angVelZLeft =
				arrAlt<bool, float>( ifElse( -rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::A ) )
				< gi;
			// clockwise angular velocity
			S<float> angVelZRight =
				arrAlt<bool, float>( ifElse( rotSpeed, 0.0f ) )
				< arrAlt<GameInput, bool>( getInputState( Key::D ) )
				< gi;
			// jump
			S<float> velYUp =
				arrAlt<bool, float>( ifElse( jumpSpeed, 0.0f ) )
				< and( onTheGround )
				< arrAlt<GameInput, bool>( getInputState(
				Key::Space ) ) < gi;


			/*
			...
			*/


			// rotation
			S<FQuat> rot = mul( arr( getRot ) < state )
				// Signal function of ball 2 of 2
				// rotation
				;			S<FQuat> rot = mul( arr( getRot ) < state )
				< arr( eulerRadToQuat<float> )
				< arrAlt<float, FVec3>( conFVec3fromY( ) )
				< integral<float>( ) < add( angVelZLeft )
				< angVelZRight;
			// acceleration
			S<FVec3> accZ = arrAlt<float, FVec3>( conFVec3fromZ( ) )
				< add( accZForward ) < accZBackward;
			// new actor state
			auto newState = setRot( rot ) < state;
			// model rotation
			auto modelRot = mul( arr( getModelRot ) < newState )
				< arr( rollBall ) < integral<FVec3>( ) < vel;
			// construct new actor output
			return arr( conActorOutput ) < setModelRot( modelRot )
				< newState;
		}
	};
}


struct BallTransformComponent : public TransformComponent
{
	virtual void vUpdate( const float deltaMs,
		const GameInput& input ) override
	{
		static const float acceleration = 0.001f;
		static const float jumpSpeed = 0.01f;
		static const float rotSpeed = 0.003f;
		static const float minPosX = 0.45f;
		static const float gravity = -0.00000981f;
		// coefficient of restitution
		static const float cor = -0.5f;
		FVec3 acc = FVec3::zero;
		float velYUp = 0.0f;

		static const float acceleration = 0.001f;
		static const float minPosX = 0.45f;
		if ( _pos.y == minPosX )
		{
			if ( input[Key::W] )
			{
				acc.z += acceleration;
			}
		}

		if ( input[Key::S] )
		{
			acc.z += -acceleration;
		}
		if ( input[Key::Space] )
		{
			velYUp = jumpSpeed;
		}
	}
	if ( input[Key::A] )
	{
		angVel += -rotSpeed;
	}
	if ( input[Key::D] )
	{
		angVel += rotSpeed;
	}
	_rot = _rot *
		eulerRadToQuat( FVec3{ 0.0f, angVel * deltaMs, 0.0f } );
	_modelRot = _modelRot *
		eulerRadToQuat( FVec3{ intVel.z, 0.0f, -intVel.x } *
		0.5f );
}
};