#include <pch.hpp>
#include "../../include/core/timer.hpp"
namespace hp_fp
{
	// http://randomascii.wordpress.com/2012/02/13/dont-store-that-in-a-float/
	// start timer at 2^32 to gain best precision
	const double TIME_ADDITION = 4294967296;
	Timer initTimer_IO( )
	{
		return Timer{ 0.0, getTimeMs_IO( ), 0.0 };
	}
	void updateTimer_IO( Timer& timer )
	{
		double currentTimeMs = getTimeMs_IO( );
		timer.deltaMs = currentTimeMs - timer._lastTimeMs;
		timer._lastTimeMs = currentTimeMs;
		timer._timeMs += timer.deltaMs;
	}
	namespace
	{
		const double getTimeMs_IO( )
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
}