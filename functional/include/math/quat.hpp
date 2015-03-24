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

	FQuat eulerDegToQuat( const FVec3& vec );
	FQuat eulerRadToQuat( const FVec3& vec );
}