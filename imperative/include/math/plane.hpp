#pragma once
#include "vec3.hpp"
namespace hp_ip
{
	struct Plane
	{
	public:
		float a, b, c, d;
		Plane( ) : a( 0.f ), b( 0.f ), c( 0.f ), d( 0.f )
		{ }
		Plane( float a, float b, float c, float d ) : a( a ), b( b ), c( c ), d( d )
		{ }
	};
	Plane init( const FVec3& p0, const FVec3& p1, const FVec3& p2 );
	Plane normalize( const Plane& plane );
	Plane planeFromPointNormal( const FVec3& point, const FVec3& normal );
	bool isInside( const Plane& plane, const FVec3& point, const float radius );
	float planeDotCoord( const Plane& plane, const FVec3& point );
}

