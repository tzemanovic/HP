#include <pch.hpp>
#include "../../include/math/plane.hpp"
namespace hp_ip
{
	Plane init( const FVec3& p0, const FVec3& p1, const FVec3& p2 )
	{
		FVec3 edge1, edge2, normal;
		edge1 = p1 - p0;
		edge2 = p2 - p0;
		normal = FVec3::normalize( cross( edge1, edge2 ) );
		return normalize( planeFromPointNormal( p0, normal ) );
	}
	Plane normalize( const Plane& plane )
	{
		Plane normPlane;
		float mag;
		mag = 1.0f / sqrt( plane.a * plane.a + plane.b * plane.b + plane.c * plane.c );
		normPlane.a = plane.a * mag;
		normPlane.b = plane.b * mag;
		normPlane.c = plane.c * mag;
		normPlane.d = plane.d * mag;
		return normPlane;
	}
	Plane planeFromPointNormal( const FVec3& point, const FVec3& normal )
	{
		Plane plane;
		plane.a = normal.x;
		plane.b = normal.y;
		plane.c = normal.z;
		plane.d = dot( point, normal );
		return plane;
	}
	bool isInside( const Plane& plane, const FVec3& point, const float radius )
	{
		float distance;
		distance = planeDotCoord( plane, point );
		return distance >= -radius;
	}
	float planeDotCoord( const Plane& plane, const FVec3& point )
	{
		return plane.a * point.x + plane.b * point.y + plane.c * point.z + plane.d;
	}
}