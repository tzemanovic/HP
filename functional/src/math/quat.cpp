#include <pch.hpp>
#include "../../include/math/quat.hpp"
namespace hp_fp
{
	const FQuat FQuat::identity( FQuat( 0.0f, 0.0f, 0.0f, 1.0f ) );
	FQuat eulerDegToQuat( const FVec3& vec )
	{
		return eulerRadToQuat( vec * DEG_TO_RAD_F );
	}
	FQuat eulerRadToQuat( const FVec3& vec )
	{
		FQuat quat;
		quat.x = sin( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * sin( vec.z / 2.0f ) +
			cos( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * cos( vec.z / 2.0f );
		quat.y = sin( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * cos( vec.z / 2.0f ) -
			cos( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * sin( vec.z / 2.0f );
		quat.z = cos( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * sin( vec.z / 2.0f ) -
			sin( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * cos( vec.z / 2.0f );
		quat.w = cos( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * cos( vec.z / 2.0f ) +
			sin( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * sin( vec.z / 2.0f );
		return quat;
	}
}