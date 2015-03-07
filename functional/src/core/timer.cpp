#include <pch.hpp>
#include <core/timer.hpp>
namespace hp_fp
{
	TimerMut initTimer_IO( )
	{
		return TimerMut{ 0.0, getTimeMs_IO( ), 0.0 };
	}
	void updateTimer_IO( TimerMut& timer )
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