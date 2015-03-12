#pragma once
#include <graphics/directx.h>
#include <math/color.hpp>
#include <math/vec2.hpp>
#include <math/vec3.hpp>
namespace hp_fp
{
	struct Vertex
	{
		FVec3 position;
		Color color;
		FVec2 texCoord;
		FVec3 normal;
		FVec3 tangent;
		FVec3 binormal;
	};
	static const D3D11_INPUT_ELEMENT_DESC D3D11_LAYOUT[] =
	{
		{
			"POSITION", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32B32_FLOAT, // format
			0, // input slot
			0, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},

		{
			"COLOR", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32B32A32_FLOAT, // format
			0, // input slot
			12, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},

		{
			"TEXCOORD", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32_FLOAT, // format
			0, // input slot
			28, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},

		{
			"NORMAL", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32B32_FLOAT, // format
			0, // input slot
			36, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},

		{
			"TANGENT", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32B32_FLOAT, // format
			0, // input slot
			48, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},

		{
			"BINORMAL", // semantic name
			0, // semantic index
			DXGI_FORMAT_R32G32B32_FLOAT, // format
			0, // input slot
			60, // aligned byte offset
			D3D11_INPUT_PER_VERTEX_DATA, // input slot class
			0 // instance data step rate
		},
	};
}