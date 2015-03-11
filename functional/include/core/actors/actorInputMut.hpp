#pragma once
#include <adt/sum.hpp>
#include <window/gameInputMut.hpp>
// [const][cop-c][cop-a][mov-c][mov-a]
// [  +  ][  0  ][  0  ][  0  ][  0  ]
namespace hp_fp
{
	struct ActorInputMut
	{
		GameInputMut gameInput;
	};

	struct ActorOutput
	{
		float x;
		float y;
	};

	//typedef Sum<ShipOutput> ActorOutput;
}