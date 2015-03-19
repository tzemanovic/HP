#pragma once
#include <math/vec3.hpp>
namespace hp_fp
{
	template<typename A>
	struct Vec4
	{
	public:
		A x, y, z, w;
		static const Vec4<A> right;
		static const Vec4<A> up;
		static const Vec4<A> forward;
		Vec4( const A x = 0, const A y = 0, const A z = 0, const A w = 0 ) : x( x ), y( y ), z( z ), w( w )
		{ }
		Vec4( const FVec3& vec );
		// not implemented yet
		static inline float Vec4Dot( const Vec4<A>* vec1, const Vec4<A>* vec2 )
		{
			return 0.f;
		}
		// not implemented yet
		static inline Vec3<A>* Vec4Normalize( Vec4<A>* vecOut, const Vec4<A>* vecIn )
		{
			return vecOut;
		}
		Vec4<A> operator + ( const Vec4<A>& vec ) const
		{
			return Vec4<A>( x + vec.x, y + vec.y, z + vec.z, 1.f );
		}
		Vec4<A> operator - ( const Vec4<A>& vec ) const
		{
			return Vec4<A>( x - vec.x, y - vec.y, z - vec.z, 1.f );
		}
		Vec4<A>& operator += ( const Vec4<A>& vec )
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vec4<A>& operator -= ( const Vec4<A>& vec )
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		template<typename B >
		friend inline Vec4< B > operator * ( const float scalar, const Vec4< B >& vec );
		template<typename B >
		friend inline Vec4< B > operator * ( const Vec4< B >& vec, const float scalar );
	};
	template<typename A>
	inline Vec4<A> operator * ( const float scalar, const Vec4<A>& vec )
	{
		return Vec4<A>( vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w );
	}
	template<typename A>
	inline Vec4<A> operator * ( const Vec4<A>& vec, const float scalar )
	{
		return scalar * vec;
	}
	typedef Vec4<float> FVec4;
}