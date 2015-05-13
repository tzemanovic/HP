#include <pch.hpp>
#include "../../include/math/mat4x4.hpp"
namespace hp_fp
{
	const Mat4x4 Mat4x4::identity( Mat4x4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ) );
	FVec3 pos( const Mat4x4& mat )
	{
		return FVec3{ mat.m[3][0], mat.m[3][1], mat.m[3][2] };
	}
	float determinant( const Mat4x4& mat )
	{
		FVec4 v1, v2, v3;
		v1.x = mat.m[0][0];
		v1.y = mat.m[1][0];
		v1.z = mat.m[2][0];
		v1.w = mat.m[3][0];
		v2.x = mat.m[0][1];
		v2.y = mat.m[1][1];
		v2.z = mat.m[2][1];
		v2.w = mat.m[3][1];
		v3.x = mat.m[0][2];
		v3.y = mat.m[1][2];
		v3.z = mat.m[2][2];
		v3.w = mat.m[3][2];
		FVec4 x = cross( v1, v2, v3 );
		return -( mat.m[0][3] * x.x + mat.m[1][3] * x.y + mat.m[2][3] * x.z +
			mat.m[3][3] * x.w );
	}
	Mat4x4 inverse( const Mat4x4& mat )
	{
		Mat4x4 invMat;
		FVec4 vec[3];
		int i, j;
		float det = determinant( mat );
		for ( i = 0; i < 4; ++i )
		{
			for ( j = 0; j < 4; ++j )
			{
				if ( i != j )
				{
					int a = ( j <= i ) ? j : j - 1;
					vec[a].x = mat.m[j][0];
					vec[a].y = mat.m[j][1];
					vec[a].z = mat.m[j][2];
					vec[a].w = mat.m[j][3];
				}
			}
			FVec4 x = cross( vec[0], vec[1], vec[2] );
			float cofactor;
			for ( j = 0; j < 4; ++j )
			{
				switch ( j )
				{
				case 0: cofactor = x.x; break;
				case 1: cofactor = x.y; break;
				case 2: cofactor = x.z; break;
				case 3: cofactor = x.w; break;
				}
				invMat.m[j][i] = pow( -1.0f, i ) * cofactor / det;
			}
		}
		return invMat;
	}
	Mat4x4 matrixPerspectiveFovLH( const float fieldOfView, const float aspectRatio,
		const float nearClipDist, const float farClipDist )
	{
		Mat4x4 perspective;
		float tanFov = tan( fieldOfView / 2.0f );
		perspective.m[0][0] = 1.0f / ( aspectRatio * tanFov );
		perspective.m[1][1] = 1.0f / tanFov;
		perspective.m[2][2] = farClipDist / ( farClipDist - nearClipDist );
		perspective.m[2][3] = 1.0f;
		perspective.m[3][2] = ( farClipDist *  -nearClipDist ) / ( farClipDist - nearClipDist );
		perspective.m[3][3] = 0.0f;
		return perspective;
	}
	Mat4x4 rotToMat4x4( const FQuat& rot )
	{
		Mat4x4 mat;
		mat.m[0][0] = 1.0f - 2.0f * ( rot.y * rot.y + rot.z * rot.z );
		mat.m[0][1] = 2.0f * ( rot.x *rot.y + rot.z * rot.w );
		mat.m[0][2] = 2.0f * ( rot.x * rot.z - rot.y * rot.w );
		mat.m[1][0] = 2.0f * ( rot.x * rot.y - rot.z * rot.w );
		mat.m[1][1] = 1.0f - 2.0f * ( rot.x * rot.x + rot.z * rot.z );
		mat.m[1][2] = 2.0f * ( rot.y *rot.z + rot.x *rot.w );
		mat.m[2][0] = 2.0f * ( rot.x * rot.z + rot.y * rot.w );
		mat.m[2][1] = 2.0f * ( rot.y *rot.z - rot.x *rot.w );
		mat.m[2][2] = 1.0f - 2.0f * ( rot.x * rot.x + rot.y * rot.y );
		return mat;
	}
	Mat4x4 posToMat4x4( const FVec3& pos )
	{
		Mat4x4 mat;
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
		return mat;
	}
	Mat4x4 sclToMat4x4( const FVec3& scl )
	{
		Mat4x4 mat;
		mat.m[0][0] = scl.x;
		mat.m[1][1] = scl.y;
		mat.m[2][2] = scl.z;
		return mat;
	}
	Mat4x4 rotSclPosToMat4x4( const FQuat& rot, const FVec3& scl, const FVec3& pos )
	{
		Mat4x4 mat;
		mat.m[0][0] = 1.0f - 2.0f * ( rot.y * rot.y + rot.z * rot.z );
		mat.m[0][1] = 2.0f * ( rot.x *rot.y + rot.z * rot.w );
		mat.m[0][2] = 2.0f * ( rot.x * rot.z - rot.y * rot.w );
		mat.m[1][0] = 2.0f * ( rot.x * rot.y - rot.z * rot.w );
		mat.m[1][1] = 1.0f - 2.0f * ( rot.x * rot.x + rot.z * rot.z );
		mat.m[1][2] = 2.0f * ( rot.y *rot.z + rot.x *rot.w );
		mat.m[2][0] = 2.0f * ( rot.x * rot.z + rot.y * rot.w );
		mat.m[2][1] = 2.0f * ( rot.y *rot.z - rot.x *rot.w );
		mat.m[2][2] = 1.0f - 2.0f * ( rot.x * rot.x + rot.y * rot.y );
		mat.m[3][0] = pos.x;
		mat.m[3][1] = pos.y;
		mat.m[3][2] = pos.z;
		mat.m[0][0] *= scl.x;
		mat.m[1][1] *= scl.y;
		mat.m[2][2] *= scl.z;
		return mat;

	}
}

