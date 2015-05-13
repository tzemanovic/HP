#include <pch.hpp>
#include "../../include/core/timer.hpp"
namespace hp_ip
{
	// http://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
	// start timer at 2^32 to gain best precision
	const double TIME_ADDITION = 4294967296;
	double Timer::update( )
	{
		double currentTimeMs = getTimeMs( );
		double deltaMs = currentTimeMs - _lastTimeMs;
		_lastTimeMs = currentTimeMs;
		_timeMs += deltaMs;
		return deltaMs;
	}
	double Timer::getTimeMs( )
	{
		// use only single thread to calculate time
		HANDLE currentThread = GetCurrentThread( );
		DWORD_PTR previousMask = SetThreadAffinityMask( currentThread, 1 );
		static LARGE_INTEGER frequency;
		LARGE_INTEGER time;
		QueryPerformanceFrequency( &frequency );
		QueryPerformanceCounter( &time );
		// use previously used thread
		SetThreadAffinityMask( currentThread, previousMask );
		return double( 1000 * time.QuadPart / frequency.QuadPart );
	}
}

