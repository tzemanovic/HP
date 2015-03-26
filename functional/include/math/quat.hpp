#pragma once
#include "vec3.hpp"
namespace hp_fp
{
	template<typename A>
	struct Quat
	{
	public:
		A x, y, z, w;
		Quat( const A x, const A y, const A z, const A w ) : x( x ), y( y ), z( z ), w( w )
		{ };
		Quat( ) : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 1.0f )
		{ };
		static const Quat<A> identity;
		Quat operator * ( const Quat& q ) const
		{
			return Quat{
				x * q.w + w * q.x + z * q.y - y * q.z,
				y * q.w - z * q.x + w * q.y + x * q.z,
				z * q.w + y * q.x - x * q.y + w * q.z,
				w * q.w - x * q.x - y * q.y - z * q.z };
		}
	};
	typedef Quat<float> FQuat;
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	template<typename A>
	Quat<A> eulerDegToQuat( const Vec3<A>& vec )
	{
		return eulerRadToQuat( vec * DEG_TO_RAD_F );
	}
	template<typename A>
	Quat<A> eulerRadToQuat( const Vec3<A>& vec )
	{
		Quat<A> quat;
		quat.x = sin( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * sin( vec.z / 2.0f ) +
			cos( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * cos( vec.z / 2.0f );
		quat.y = sin( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * cos( vec.z / 2.0f ) -
			cos( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * sin( vec.z / 2.0f );
		quat.z = cos( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * sin( vec.z / 2.0f ) -
			sin( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * cos( vec.z / 2.0f );
		quat.w = cos( vec.y / 2.0f ) * cos( vec.x / 2.0f ) * cos( vec.z / 2.0f ) +
			sin( vec.y / 2.0f ) * sin( vec.x / 2.0f ) * sin( vec.z / 2.0f );
		return quat;
	}
	template<typename A>
	Quat<A> conjugate( const Quat<A>& quat )
	{
		return Quat < A > {-quat.x, -quat.y, -quat.z, quat.w};
	}
	template<typename A>
	Vec3<A> rotate( const Vec3<A>& vec, const Quat<A>& quat )
	{
		auto rotated = conjugate( quat ) * Quat < A > {vec.x, vec.y, vec.z, 1.0f} *quat;
		return Vec3 < A > {rotated.x, rotated.y, rotated.z};
	};
}