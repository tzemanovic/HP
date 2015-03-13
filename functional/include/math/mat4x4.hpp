#pragma once
#include <math/vec3.hpp>
#include <math/vec4.hpp>
namespace hp_fp
{
	class Mat4x4
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
		Mat4x4( )
		{ }
		Mat4x4( float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44 )
			: _11( m11 ), _12( m12 ), _13( m13 ), _14( m14 ), _21( m21 ), _22( m22 ), _23( m23 ), _24( m24 ),
			_31( m31 ), _32( m32 ), _33( m33 ), _34( m34 ), _41( m41 ), _42( m42 ), _43( m43 ), _44( m44 )
		{ }
		static const Mat4x4 identity;
		// not implemented yet
		static inline Mat4x4* Identity( Mat4x4* pMat )
		{
			return &Mat4x4( );
		}
		// not implemented yet
		inline Mat4x4 Inverse( ) const
		{
			return Mat4x4( );
		}
		inline const FVec3 GetPosition( ) const
		{
			return FVec3( m[3][0], m[3][1], m[3][2] );
		}
		inline void SetPosition( const FVec3 pos )
		{
			m[3][0] = pos.x, m[3][1] = pos.y; m[3][2] = pos.z; m[3][3] = 1.0f;
		}
	};
}