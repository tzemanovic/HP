#pragma once
#include "vec3.hpp"
#include "vec4.hpp"
#include "quat.hpp"
namespace hp_ip
{
	struct Mat4x4
	{
	public:
		union
		{
			struct
			{
				float _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44;
			};
			float m[4][4];
		};
		Mat4x4( ) : Mat4x4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 )
		{ }
		Mat4x4( float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44 )
			: _11( m11 ), _12( m12 ), _13( m13 ), _14( m14 ), _21( m21 ), _22( m22 ), _23( m23 ), _24( m24 ),
			_31( m31 ), _32( m32 ), _33( m33 ), _34( m34 ), _41( m41 ), _42( m42 ), _43( m43 ), _44( m44 )
		{ }
		static const Mat4x4 identity;
		Mat4x4 operator * ( const Mat4x4& mat ) const
		{
			Mat4x4 result;
			for ( int i = 0; i < 4; i++ )
			{
				for ( int j = 0; j < 4; j++ )
				{
					result.m[i][j] =
						m[i][0] * mat.m[0][j] +
						m[i][1] * mat.m[1][j] +
						m[i][2] * mat.m[2][j] +
						m[i][3] * mat.m[3][j];
				}
			}
			return result;
		}
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	FVec3 pos( const Mat4x4& mat );
	float determinant( const Mat4x4& mat );
	Mat4x4 inverse( const Mat4x4& mat );
	Mat4x4 matrixPerspectiveFovLH( const float fieldOfView, const float aspectRatio,
		const float nearClipDist, const float farClipDist );
	Mat4x4 rotToMat4x4( const FQuat& rot );
	Mat4x4 sclToMat4x4( const FVec3& scl );
	Mat4x4 posToMat4x4( const FVec3& pos );
	Mat4x4 rotSclPosToMat4x4( const FQuat& rot, const FVec3& scl, const FVec3& pos );
}

