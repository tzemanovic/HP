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
}