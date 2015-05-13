#include <pch.hpp>
#include "../../include/math/frustum.hpp"
namespace hp_ip
{
	Frustum init( const float fieldOfView, const float aspectRatio, const float nearClipDist,
		const float farClipDist )
	{
		Frustum frustum;
		frustum.fieldOfView = fieldOfView;
		frustum.aspectRatio = aspectRatio;
		frustum.nearClipDist = nearClipDist;
		frustum.farClipDist = farClipDist;
		float tanHalfFov = tan( frustum.fieldOfView / 2.f );
		FVec3 nearRight = ( frustum.nearClipDist * tanHalfFov ) * frustum.aspectRatio * FVec3::right;
		FVec3 farRight = ( frustum.farClipDist * tanHalfFov ) * frustum.aspectRatio * FVec3::right;
		FVec3 nearUp = ( frustum.nearClipDist * tanHalfFov ) * frustum.aspectRatio * FVec3::up;
		FVec3 farUp = ( frustum.farClipDist * tanHalfFov ) * frustum.aspectRatio * FVec3::up;
		frustum.nearClipVerts[0] = ( frustum.nearClipDist * FVec3::forward ) - nearRight + nearUp;
		frustum.nearClipVerts[1] = ( frustum.nearClipDist * FVec3::forward ) + nearRight + nearUp;
		frustum.nearClipVerts[2] = ( frustum.nearClipDist * FVec3::forward ) + nearRight - nearUp;
		frustum.nearClipVerts[3] = ( frustum.nearClipDist * FVec3::forward ) - nearRight - nearUp;
		frustum.farClipVerts[0] = ( frustum.farClipDist * FVec3::forward ) - farRight + farUp;
		frustum.farClipVerts[1] = ( frustum.farClipDist * FVec3::forward ) + farRight + farUp;
		frustum.farClipVerts[2] = ( frustum.farClipDist * FVec3::forward ) + farRight - farUp;
		frustum.farClipVerts[3] = ( frustum.farClipDist * FVec3::forward ) - farRight - farUp;
		FVec3 origin;
		frustum.planes[static_cast<UInt8>( FrustumSides::Near )] = init( frustum.nearClipVerts[2], frustum.nearClipVerts[1], frustum.nearClipVerts[0] );
		frustum.planes[static_cast<UInt8>( FrustumSides::Far )] = init( frustum.farClipVerts[0], frustum.farClipVerts[1], frustum.farClipVerts[2] );
		frustum.planes[static_cast<UInt8>( FrustumSides::Right )] = init( frustum.farClipVerts[2], frustum.farClipVerts[1], origin );
		frustum.planes[static_cast<UInt8>( FrustumSides::Top )] = init( frustum.farClipVerts[1], frustum.farClipVerts[0], origin );
		frustum.planes[static_cast<UInt8>( FrustumSides::Left )] = init( frustum.farClipVerts[0], frustum.farClipVerts[3], origin );
		frustum.planes[static_cast<UInt8>( FrustumSides::Bottom )] = init( frustum.farClipVerts[3], frustum.farClipVerts[2], origin );
		return frustum;
	}
	bool isInside( const Frustum& frustum, const FVec3& point, const float radius )
	{
		for ( UInt8 i = 0; i < static_cast<UInt8>( FrustumSides::Count ); ++i )
		{
			if ( !isInside( frustum.planes[i], point, radius ) )
			{
				return false;
			}
		}
		return true;
	}
}

