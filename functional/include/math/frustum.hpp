#pragma once
#include "plane.hpp"
namespace hp_fp
{
	enum class FrustumSides : UInt8
	{
		Near, Far, Top, Right, Bottom, Left, Count
	};
	struct Frustum
	{
		Plane planes[static_cast<UInt8>( FrustumSides::Count )];
		FVec3 nearClipVerts[4];
		FVec3 farClipVerts[4];
		float fieldOfView; // radians
		float aspectRatio; // width / height
		float nearClipDist;
		float farClipDist;
	};
	Frustum init( const float fieldOfView, const float aspectRatio, const float nearClipDist,
		const float farClipDist );
	bool isInside( const Frustum& frustum, const FVec3& point, const float radius );
}