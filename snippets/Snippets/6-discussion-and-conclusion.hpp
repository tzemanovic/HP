

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
			S<bool> onTheGround = arr( eq( minPosX ) )
				< arr( getY ) < arr( getPos ) < state;
			// controls
			// accelerate forward
			S<float> accZForward =
				arr( ifElse( acceleration, 0.0f ) )
				< and( onTheGround )
				< arr( getInputState( Key::W ) ) < gi;
			// accelerate backward
			S<float> accZBackward =
				arr( ifElse( -acceleration, 0.0f ) )
				< and( onTheGround )
				< arr( getInputState( Key::S ) ) < gi;
			// anti-clockwise angular velocity
			S<float> angVelZLeft = arr( ifElse( -rotSpeed, 0.0f ) )
				< arr( getInputState( Key::A ) ) < gi;
			// clockwise angular velocity
			S<float> angVelZRight = arr( ifElse( rotSpeed, 0.0f ) )
				< arr( getInputState( Key::D ) ) < gi;

			/*
			...
			*/

			// jump
			S<float> velYUp = arr( ifElse( jumpSpeed, 0.0f ) )
				< and( onTheGround )
				< arr( getInputState( Key::Space ) ) < gi;
			// rotation
			S<FQuat> rot = mul( arr( getRot ) < state )
// Signal function of ball 2 of 2
				// rotation
;			S<FQuat> rot = mul( arr( getRot ) < state )
				< arr( eulerRadToQuat )
				< arr( conFVec3fromY( ) ) < integral( )
				< add( angVelZLeft ) < angVelZRight;
			// acceleration
			S<FVec3> accZ = arr( conFVec3fromZ( ) )
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

// C++ member variable access
struct Foo
{
	int bar;
}; 
Foo foo{ 1337 }; 
int foobar = foo.bar;