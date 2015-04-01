#include <pch.hpp>
#include "../../include/adt/frp/sfs.hpp"
namespace hp_fp
{
	SF<FVec3, FVec3> rotate( const S<FQuat>& rot )
	{
		return SF < FVec3, FVec3 >
		{
			[rot]( const S<FVec3>& b ) -> S < FVec3 >
			{
				return S < FVec3 >
				{
					[rot, b]( const float deltaMs ) -> FVec3
					{
						return rotate( b < deltaMs, rot < deltaMs );
					}
				};
			}
		};
	}
	SF<FVec3, FVec3> rotate( const FQuat& rot )
	{
		return SF < FVec3, FVec3 >
		{
			[rot]( const S<FVec3>& b ) -> S < FVec3 >
			{
				return S < FVec3 >
				{
					[rot, b]( const float deltaMs ) -> FVec3
					{
						return rotate( b < deltaMs, rot );
					}
				};
			}
		};
	}
}