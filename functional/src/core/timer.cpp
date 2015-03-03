#include <pch.hpp>
#include <core/timer.hpp>
namespace hp_fp
{
	TimerMut initTimer_IO( )
	{
		return TimerMut{ 0.0, getCurrentTimeMs_IO( ), 0.0 };
	}
	void updateTimer_IO( TimerMut& timer )
	{
		double currentTimeMs = getCurrentTimeMs_IO( );
		timer.deltaMs = currentTimeMs - timer._lastTimeMs;
		timer._lastTimeMs = currentTimeMs;
		timer._currentTimeMs += timer.deltaMs;
	}
	namespace
	{
		const double getCurrentTimeMs_IO( )
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