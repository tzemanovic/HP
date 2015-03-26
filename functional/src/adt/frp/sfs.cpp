#include <pch.hpp>
#include "../../include/adt/frp/sfs.hpp"
namespace hp_fp
{
	SF<FVec3, FVec3> integral( )
	{
		return SF < FVec3, FVec3 > {
			[]( const S<FVec3>& a ) -> S < FVec3 >
			{
				return signal( a.val * static_cast<float>( a.deltaMs ), a.deltaMs );
			}
		};
	}
	SF<FVec3, FVec3> rotate( const S<FQuat>& rot )
	{
		return SF < FVec3, FVec3 > {
			[&rot]( const S<FVec3>& b )
			{
				return signal( std::forward<FVec3>( rotate( b.val, rot.val ) ), b.deltaMs );
			}
		};
	}
}