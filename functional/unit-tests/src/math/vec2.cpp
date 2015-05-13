#include <pch/pch.hpp>
#include <math/vec2.hpp>
#include <gtest/gtest.h>
using namespace hp_fp;

TEST( Vec2Test, OpMinus )
{
	FVec2 a{ 1.0f, -1.0f };
	FVec2 b{ -0.5f, 0.5f };
	EXPECT_EQ( a.x - b.x, (a - b).x );
	EXPECT_EQ( b.x - a.x, (b - a).x );
	EXPECT_EQ( a.y - b.y, (a - b).y );
	EXPECT_EQ( b.y - a.y, (b - a).y );
}

TEST( Vec2Test, OpEquals )
{
	FVec2 a{ 1.0f, -1.0f };
	FVec2 b{ 1.0f, -1.0f };
	FVec2 c{ -1.0f, -1.0f };
	FVec2 d{ 1.0f, 1.0f };
	FVec2 e{ -1.0f, 1.0f };
	EXPECT_EQ( true, a == b );
	EXPECT_EQ( false, a == c );
	EXPECT_EQ( false, a == d );
	EXPECT_EQ( false, a == e );
}

TEST( Vec2Test, OpLowerThan )
{
	FVec2 a{ 1.0f, -1.0f };
	FVec2 b{ 1.0f, -1.0f };
	FVec2 c{ -1.0f, -1.0f };
	FVec2 d{ 1.0f, 1.0f };
	FVec2 e{ -1.0f, 1.0f };
	EXPECT_EQ( false, a < b );
	EXPECT_EQ( false, b < a );
	EXPECT_EQ( false, a < c );
	EXPECT_EQ( true, c < a );
	EXPECT_EQ( true, a < d );
	EXPECT_EQ( false, d < a );
	EXPECT_EQ( false, a < e );
	EXPECT_EQ( true, e < a );
}