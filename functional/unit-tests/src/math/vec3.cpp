#include <pch/pch.hpp>
#include <math/vec3.hpp>
#include <gtest/gtest.h>
using namespace hp_fp;

TEST( Vec3Test, OpPlus )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	FVec3 b{ -0.5f, 0.5f, -0.5f };
	EXPECT_EQ( a.x + b.x, ( a + b ).x );
	EXPECT_EQ( b.x + a.x, ( b + a ).x );
	EXPECT_EQ( a.y + b.y, ( a + b ).y );
	EXPECT_EQ( b.y + a.y, ( b + a ).y );
	EXPECT_EQ( a.z + b.z, ( a + b ).z );
	EXPECT_EQ( b.z + a.z, ( b + a ).z );
}

TEST( Vec3Test, OpMinus )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	FVec3 b{ -0.5f, 0.5f, -0.5f };
	EXPECT_EQ( a.x - b.x, ( a - b ).x );
	EXPECT_EQ( b.x - a.x, ( b - a ).x );
	EXPECT_EQ( a.y - b.y, ( a - b ).y );
	EXPECT_EQ( b.y - a.y, ( b - a ).y );
	EXPECT_EQ( a.z - b.z, ( a - b ).z );
	EXPECT_EQ( b.z - a.z, ( b - a ).z );
}

TEST( Vec3Test, OpEquals )
{
	FVec3 a{ 1.0f, -1.0f, 1.0f };
	FVec3 b{ 1.0f, -1.0f, 1.0f };
	FVec3 c{ -1.0f, -1.0f, 1.0f };
	FVec3 d{ 1.0f, 1.0f, -1.0f };
	FVec3 e{ -1.0f, 1.0f, -1.0f };
	EXPECT_EQ( true, a == b );
	EXPECT_EQ( false, a == c );
	EXPECT_EQ( false, a == d );
	EXPECT_EQ( false, a == e );
}

TEST( Vec3Test, OpLowerThan )
{
	FVec3 a{ 1.0f, -1.0f, 1.0f };
	FVec3 b{ 1.0f, -1.0f, 1.0f };
	FVec3 c{ -1.0f, -1.0f, 1.0f };
	FVec3 d{ 1.0f, 1.0f, -1.0f };
	FVec3 e{ -1.0f, 1.0f, -1.0f };
	EXPECT_EQ( false, a < b );
	EXPECT_EQ( false, b < a );
	EXPECT_EQ( false, a < c );
	EXPECT_EQ( true, c < a );
	EXPECT_EQ( true, a < d );
	EXPECT_EQ( false, d < a );
	EXPECT_EQ( false, a < e );
	EXPECT_EQ( true, e < a );
}

TEST( Vec3Test, OpMultiply )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	float scalar = 3.0f;
	EXPECT_EQ( a.x * scalar, ( a * scalar ).x );
	EXPECT_EQ( a.x * scalar, ( scalar * a ).x );
	EXPECT_EQ( a.y * scalar, ( a * scalar ).y );
	EXPECT_EQ( a.y * scalar, ( scalar * a ).y );
	EXPECT_EQ( a.z * scalar, ( a * scalar ).z );
	EXPECT_EQ( a.z * scalar, ( scalar * a ).z );
}

TEST( Vec3Test, OpDivide )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	float scalar = 0.5f;
	EXPECT_EQ( a.x / scalar, ( a / scalar ).x );
	EXPECT_EQ( a.y / scalar, ( a / scalar ).y );
	EXPECT_EQ( a.z / scalar, ( a / scalar ).z );
}

TEST( Vec3Test, FnCross )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	FVec3 b{ 5.0f, -0.5f, -1.5f };
	EXPECT_EQ( a.y * b.z - a.z * b.y, cross( a, b ).x );
	EXPECT_EQ( a.z * b.x - a.x * b.z, cross( a, b ).y );
	EXPECT_EQ( a.x * b.y - a.y * b.x, cross( a, b ).z );
}

TEST( Vec3Test, FnDot )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	FVec3 b{ 5.0f, -0.5f, -1.5f };
	EXPECT_EQ( a.x * b.x + a.y * b.y + a.z * b.z, dot( a, b ) );
}