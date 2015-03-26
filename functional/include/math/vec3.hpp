#pragma once
namespace hp_fp
{
	template<typename A>
	struct Vec3
	{
	public:
		A x, y, z;
		static const Vec3<A> zero;
		static const Vec3<A> right;
		static const Vec3<A> up;
		static const Vec3<A> forward;
		/*Vec3( const A x = 0, const A y = 0, const A z = 0 ) : x( x ), y( y ), z( z )
		{ }*/
		Vec3<A> operator + ( const Vec3<A>& vec ) const
		{
			return Vec3 < A > { x + vec.x, y + vec.y, z + vec.z };
		}
		Vec3<A> operator - ( const Vec3<A>& vec ) const
		{
			return Vec3 < A > { x - vec.x, y - vec.y, z - vec.z };
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
		bool operator == ( const Vec3<A>& v ) const
		{
			return x == v.x && y == v.y && z == v.z;
		}
		bool operator < ( const Vec3<A>& v ) const
		{
			if ( x == v.x )
			{
				if ( y == v.y )
				{
					return  z < v.z;
				}
				return y < v.y;
			}
			return x < v.x;
		}
		template<typename B>
		friend inline Vec3<B> operator * ( const float scalar, const Vec3<B>& vec );
		template<typename B>
		friend inline Vec3<B> operator * ( const Vec3<B>& vec, const float scalar );
		template<typename B>
		friend inline Vec3<B> operator / ( const Vec3<B>& vec, const float scalar );
	};
	typedef Vec3<UInt16> UInt16Vec3;
	typedef Vec3<Int16> Int16Vec3;
	typedef Vec3<UInt32> UInt32Vec3;
	typedef Vec3<Int32> Int32Vec3;
	typedef Vec3<float> FVec3;
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A>
	inline Vec3<A> operator * ( const float scalar, const Vec3<A>& vec )
	{
		return Vec3 < A > { vec.x * scalar, vec.y * scalar, vec.z * scalar };
	}
	template<typename A>
	inline Vec3<A> operator * ( const Vec3<A>& vec, const float scalar )
	{
		return scalar * vec;
	}
	template<typename A>
	inline Vec3<A> operator / ( const Vec3<A>& vec, const float scalar )
	{
		return Vec3 < A > { vec.x / scalar, vec.y / scalar, vec.z / scalar };
	}
	template<typename A>
	inline Vec3<A> cross( const Vec3<A>& vec1, const Vec3<A>& vec2 )
	{
		return Vec3 < A > {
			vec1.y * vec2.z - vec1.z * vec2.y,
				vec1.z * vec2.x - vec1.x * vec2.z,
				vec1.x * vec2.y - vec1.y * vec2.x };
	}
	template<typename A>
	inline A dot( const Vec3<A>& vec1, const Vec3<A>& vec2 )
	{
		return ( vec1.x ) * ( vec2.x ) + ( vec1.y ) * ( vec2.y ) + ( vec1.z ) * ( vec2.z );
	}
	template<typename A>
	Vec3<A> normalize( const Vec3<A>& vec )
	{
		Vec3<A> normVec{ };
		float len = length( vec );
		if ( len == 0.0f )
		{
			return normVec;
		}
		len = 1.0f / len;
		normVec.x = vec.x * len;
		normVec.y = vec.y * len;
		normVec.z = vec.z * len;
		return normVec;
	}
	template<typename A>
	float length( const Vec3<A>& vec )
	{
		return sqrtf( vec.x * vec.x + vec.y * vec.y + vec.z * vec.z );
	}
	template<typename A>
	float mag( const Vec3<A>& vec )
	{
		return length( vec );
	}
	template<typename A>
	Vec3<A> clampMag( const Vec3<A>& vec, const float min, const float max )
	{
		float magnitude = mag( vec );
		if ( magnitude < min )
		{
			return vec / magnitude * min;
		}
		else if ( magnitude > max )
		{
			return vec / magnitude * max;
		}
		return vec;
	}
}