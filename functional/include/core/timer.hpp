#pragma once
#include <core/timerMut.hpp>
namespace hp_fp
{
	TimerMut initTimer_IO( );
	void updateTimer_IO( TimerMut& timer );
	namespace
	{
		const double getTimeMs_IO( );
	}
}