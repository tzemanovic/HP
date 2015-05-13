#include <pch/pch.hpp>
#include <math/quat.hpp>
#include <gtest/gtest.h>
using namespace hp_fp;

TEST( QuatTest, OpMultiply )
{
	FQuat a{ 1.0f, -1.0f, 0.5f, 1.0f };
	FQuat b{ -0.5f, 0.5f, -1.0f, 1.0f };
	EXPECT_EQ( a.x * b.w + a.w * b.x + a.z * b.y - a.y * b.z, ( a * b ).x );
	EXPECT_EQ( a.y * b.w - a.z * b.x + a.w * b.y + a.x * b.z, ( a * b ).y );
	EXPECT_EQ( a.z * b.w + a.y * b.x - a.x * b.y + a.w * b.z, ( a * b ).z );
	EXPECT_EQ( a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z, ( a * b ).w );
}

TEST( QuatTest, FnEulerDegToQuat )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	FVec3 aRad = a * DEG_TO_RAD_F;
	EXPECT_EQ( sin( aRad.y / 2.0f ) * cos( aRad.x / 2.0f ) * sin( aRad.z / 2.0f ) +
		cos( aRad.y / 2.0f ) * sin( aRad.x / 2.0f ) * cos( aRad.z / 2.0f ),
		eulerDegToQuat( a ).x );
	EXPECT_EQ( sin( aRad.y / 2.0f ) * cos( aRad.x / 2.0f ) * cos( aRad.z / 2.0f ) -
		cos( aRad.y / 2.0f ) * sin( aRad.x / 2.0f ) * sin( aRad.z / 2.0f ),
		eulerDegToQuat( a ).y );
	EXPECT_EQ( cos( aRad.y / 2.0f ) * cos( aRad.x / 2.0f ) * sin( aRad.z / 2.0f ) -
		sin( aRad.y / 2.0f ) * sin( aRad.x / 2.0f ) * cos( aRad.z / 2.0f ),
		eulerDegToQuat( a ).z );
	EXPECT_EQ( cos( aRad.y / 2.0f ) * cos( aRad.x / 2.0f ) * cos( aRad.z / 2.0f ) +
		sin( aRad.y / 2.0f ) * sin( aRad.x / 2.0f ) * sin( aRad.z / 2.0f ),
		eulerDegToQuat( a ).w );
}

TEST( QuatTest, FnEulerRadToQuat )
{
	FVec3 a{ 1.0f, -1.0f, 0.5f };
	EXPECT_EQ( sin( a.y / 2.0f ) * cos( a.x / 2.0f ) * sin( a.z / 2.0f ) +
		cos( a.y / 2.0f ) * sin( a.x / 2.0f ) * cos( a.z / 2.0f ),
		eulerRadToQuat( a ).x );
	EXPECT_EQ( sin( a.y / 2.0f ) * cos( a.x / 2.0f ) * cos( a.z / 2.0f ) -
		cos( a.y / 2.0f ) * sin( a.x / 2.0f ) * sin( a.z / 2.0f ),
		eulerRadToQuat( a ).y );
	EXPECT_EQ( cos( a.y / 2.0f ) * cos( a.x / 2.0f ) * sin( a.z / 2.0f ) -
		sin( a.y / 2.0f ) * sin( a.x / 2.0f ) * cos( a.z / 2.0f ),
		eulerRadToQuat( a ).z );
	EXPECT_EQ( cos( a.y / 2.0f ) * cos( a.x / 2.0f ) * cos( a.z / 2.0f ) +
		sin( a.y / 2.0f ) * sin( a.x / 2.0f ) * sin( a.z / 2.0f ),
		eulerRadToQuat( a ).w );
}
