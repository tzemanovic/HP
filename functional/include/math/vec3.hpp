#pragma once
namespace hp_fp
{
	template<typename A>
	struct Vec3
	{
	public:
		A x, y, z;
		static const Vec3<A> right;
		static const Vec3<A> up;
		static const Vec3<A> forward;
		Vec3( const A x = 0, const A y = 0, const A z = 0 ) : x( x ), y( y ), z( z )
		{ }
		static inline Vec3<A> normalize( const Vec3<A>& vec )
		{
			Vec3<A> normVec;
			float length = Vec3<A>::length( vec );
			if ( length == 0.0f )
			{
				return normVec;
			}
			length = 1.0f / length;
			normVec.x = vec.x * length;
			normVec.y = vec.y * length;
			normVec.z = vec.z * length;
			return normVec;
		}
		static float length( const Vec3<A>& vec )
		{
			return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
		}
		Vec3<A> operator + ( const Vec3<A>& vec ) const
		{
			return Vec3<A>( x + vec.x, y + vec.y, z + vec.z );
		}
		Vec3<A> operator - ( const Vec3<A>& vec ) const
		{
			return Vec3<A>( x - vec.x, y - vec.y, z - vec.z );
		}
		Vec3<A>& operator += ( const Vec3<A>& vec )
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}
		Vec3<A>& operator -= ( const Vec3<A>& vec )
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}
		template<typename B>
		friend inline Vec3<B> operator * ( const float scalar, const Vec3<B>& vec );
		template<typename B>
		friend inline Vec3<B> operator * ( const Vec3<B>& vec, const float scalar );
	};
	template<typename A>
	inline Vec3<A> operator * ( const float scalar, const Vec3<A>& vec )
	{
		return Vec3<A>( vec.x * scalar, vec.y * scalar, vec.z * scalar );
	}
	template<typename A>
	inline Vec3<A> operator * ( const Vec3<A>& vec, const float scalar )
	{
		return scalar * vec;
	}
	template<typename A>
	inline Vec3<A> cross( const Vec3<A>& vec1, const Vec3<A>& vec2 )
	{
		return Vec3<A>(
			vec1.y * vec2.z - vec1.z * vec2.y,
			vec1.z * vec2.x - vec1.x * vec2.z,
			vec1.x * vec2.y - vec1.y * vec2.x );
	}
	template<typename A>
	inline A dot( const Vec3<A>& vec1, const Vec3<A>& vec2 )
	{
		return ( vec1.x ) * ( vec2.x ) + ( vec1.y ) * ( vec2.y ) + ( vec1.z ) * ( vec2.z );
	}
	typedef Vec3<UInt16> UInt16Vec3;
	typedef Vec3<Int16> Int16Vec3;
	typedef Vec3<UInt32> UInt32Vec3;
	typedef Vec3<Int32> Int32Vec3;
	typedef Vec3<float> FVec3;
}