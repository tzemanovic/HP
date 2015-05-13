#include <pch/pch.hpp>
#include "ballTransformComponent.hpp"
namespace hp_ip
{
	void BallTransformComponent::vUpdate( const float deltaMs, const GameInput& input )
	{
		static const float acceleration = 0.001f;
		static const float jumpSpeed = 0.01f;
		static const float rotSpeed = 0.003f;
		static const float minPosX = 0.45f;
		static const float gravity = -0.00000981f;
		static const float cor = -0.5f; // coefficient of restitution
		FVec3 acc = FVec3::zero;
		float angVel = 0.0f;
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
			if ( input[Key::Q] )
			{
				acc.x += -acceleration;
			}
			if ( input[Key::E] )
			{
				acc.x += acceleration;
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
		_rot = _rot * eulerRadToQuat( FVec3{ 0.0f, angVel * deltaMs, 0.0f } );
		velYUp = _vel.y + velYUp + gravity * deltaMs;
		if ( _pos.y <= minPosX )
		{
			if ( velYUp < 0.0f )
			{
				velYUp *= cor;
			}
			if ( fabsf( velYUp ) < 0.001f )
			{
				velYUp = 0.0f;
			}
		}
		_vel = ( ( _vel + acc * deltaMs ) * 0.999f ).clampMag( 0.01f );
		_vel.y = velYUp;
		_pos = _pos + rotate( _vel * deltaMs, _rot );
		if ( _pos.y < minPosX )
		{
			_pos.y = minPosX;
		}
		FVec3 intVel = _vel * deltaMs;
		_modelRot = _modelRot * eulerRadToQuat( FVec3{ intVel.z, 0.0f, -intVel.x } *0.5f );
	}
}

