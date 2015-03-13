#include <pch.hpp>
#include <math/vec4.hpp>
namespace hp_fp
{
	template<typename A>
	Vec4<A>::Vec4( const FVec3& vec ) : x( vec.x ), y( vec.y ), z( vec.z ), w( 1.f )
	{ }
	const FVec4 FVec4::right( 1.f, 0.f, 0.f, 0.f );
	const FVec4 FVec4::up( 0.f, 1.f, 0.f, 0.f );
	const FVec4 FVec4::forward( 0.f, 0.f, 1.f, 0.f );
}