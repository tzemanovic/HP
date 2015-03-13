#pragma once
namespace hp_fp
{
	struct Color
	{
		float r, g, b, a;
		Color( const float r = 0, const float g = 0, const float b = 0, const float a = 1.0f ) : r( r ), g( g ), b( b ), a( a )
		{ }
		operator float*( )
		{
			return &r;
		}
	};
}