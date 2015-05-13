#pragma once
namespace hp_ip
{
	template<typename A>
	struct Vec2
	{
	public:
		A x, y;
		/*Vec2( const A x = 0, const A y = 0 ) : x( x ), y( y )
		{ }*/
		Vec2<A> operator - ( const Vec2<A>& vec ) const
		{
			return Vec2 < A > { x - vec.x, y - vec.y };
		}
		bool operator == ( const Vec2<A>& v ) const
		{
			return x == v.x && y == v.y;
		}
		bool operator < ( const Vec2<A>& v ) const
		{
			if ( x == v.x )
			{
				return y < v.y;
			}
			return x < v.x;
		}
	};
	typedef Vec2<UInt16> UInt16Vec2;
	typedef Vec2<Int16> Int16Vec2;
	typedef Vec2<UInt32> UInt32Vec2;
	typedef Vec2<Int32> Int32Vec2;
	typedef Vec2<float> FVec2;
}

