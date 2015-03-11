#pragma once
namespace hp_fp
{
	template<typename A>
	struct Vec2
	{
	public:
		A x, y;
		Vec2( const A x = 0, const A y = 0 ) : x( x ), y( y )
		{ }
		Vec2<A> operator - ( const Vec2<A>& vec ) const
		{
			return Vec2<A>( x - vec.x, y - vec.y );
		}
	};
	typedef Vec2<UInt16> UInt16Vec2;
	typedef Vec2<Int16> Int16Vec2;
	typedef Vec2<UInt32> UInt32Vec2;
	typedef Vec2<Int32> Int32Vec2;
	typedef Vec2<float> FVec2;
}