#include <pch/pch.hpp>
#include <math/vec4.hpp>
#include <gtest/gtest.h>
using namespace hp_fp;

TEST( Vec4Test, OpPlus )
{
	FVec4 a{ 1.0f, -1.0f, 0.5f, 1.5f };
	FVec4 b{ -0.5f, 0.5f, -0.5f, 0.0f };
	EXPECT_EQ( a.x + b.x, ( a + b ).x );
	EXPECT_EQ( b.x + a.x, ( b + a ).x );
	EXPECT_EQ( a.y + b.y, ( a + b ).y );
	EXPECT_EQ( b.y + a.y, ( b + a ).y );
	EXPECT_EQ( a.z + b.z, ( a + b ).z );
	EXPECT_EQ( b.z + a.z, ( b + a ).z );
	EXPECT_EQ( 1.0f, ( a + b ).w );
	EXPECT_EQ( 1.0f, ( b + a ).w );
}

TEST( Vec4Test, OpMinus )
{
	FVec4 a{ 1.0f, -1.0f, 0.5f, 1.5f };
	FVec4 b{ -0.5f, 0.5f, -0.5f, 0.0f };
	EXPECT_EQ( a.x - b.x, ( a - b ).x );
	EXPECT_EQ( b.x - a.x, ( b - a ).x );
	EXPECT_EQ( a.y - b.y, ( a - b ).y );
	EXPECT_EQ( b.y - a.y, ( b - a ).y );
	EXPECT_EQ( a.z - b.z, ( a - b ).z );
	EXPECT_EQ( b.z - a.z, ( b - a ).z );
	EXPECT_EQ( 1.0f, ( a - b ).w );
	EXPECT_EQ( 1.0f, ( b - a ).w );
}

TEST( Vec4Test, OpMultiply )
{
	FVec4 a{ 1.0f, -1.0f, 0.5f, 1.5f };
	float scalar = 4.0f;
	EXPECT_EQ( a.x * scalar, ( a * scalar ).x );
	EXPECT_EQ( a.x * scalar, ( scalar * a ).x );
	EXPECT_EQ( a.y * scalar, ( a * scalar ).y );
	EXPECT_EQ( a.y * scalar, ( scalar * a ).y );
	EXPECT_EQ( a.z * scalar, ( a * scalar ).z );
	EXPECT_EQ( a.z * scalar, ( scalar * a ).z );
	EXPECT_EQ( a.w, ( a * scalar ).w );
	EXPECT_EQ( a.w, ( scalar * a ).w );
}

TEST( Vec4Test, FnCross )
{
	FVec4 a{ 1.0f, -1.0f, 0.5f, 1.5f };
	FVec4 b{ 5.0f, -0.5f, -1.5f, 0.0f };
	FVec4 c{ -0.5f, 1.5f, 5.5f, -1.0f };
	EXPECT_EQ( a.y * ( b.z * c.w - b.w * c.z )
		- a.z * ( b.y * c.w - b.w * c.y )
		+ a.w * ( b.y * c.z - b.z * c.y ), cross( a, b, c ).x );
	EXPECT_EQ( -a.x * ( b.z * c.w - b.w * c.z )
		+ a.z * ( b.x * c.w - b.w * c.x )
		- a.w * ( b.x * c.z - b.z * c.x ), cross( a, b, c ).y );
	EXPECT_EQ( a.x * ( b.y * c.w - b.w * c.y )
		- a.y * ( b.x * c.w - b.w * c.x )
		+ a.w * ( b.x * c.y - b.y * c.x ), cross( a, b, c ).z );
	EXPECT_EQ( -a.x * ( b.y * c.z - b.z * c.y )
		+ a.y * ( b.x * c.z - b.z * c.x )
		- a.z * ( b.x * c.y - b.y * c.x ), cross( a, b, c ).w );
}