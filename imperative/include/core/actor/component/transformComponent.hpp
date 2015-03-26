#pragma once
#include "iComponent.hpp"
#include "../../../math/quat.hpp"
namespace hp_ip
{
	class transformComponent : public iComponent
	{
	private:
		FVec3 _pos;
		FVec3 _vel;
		FVec3 _scl;
		FQuat _rot;
	public:
		FVec3 pos( ) const
		{
			return _pos;
		}
		FVec3 vel( ) const
		{
			return _vel;
		}
		FVec3 scl( ) const
		{
			return _scl;
		}
		FQuat rot( ) const
		{
			return _rot;
		}
		void setPos( const FVec3& pos )
		{
			_pos = pos;
		}
		void setVel( const FVec3 vel )
		{
			_vel = vel;
		}
		void setScl( const FVec3 scl )
		{
			_scl = scl;
		}
		void setRot( const FQuat rot )
		{
			_rot = rot;
		}
	};
}