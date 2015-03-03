#pragma once
// [const][cop-c][cop-a][mov-c][mov-a]
// [  -  ][  0  ][  0  ][  +  ][  +  ]
namespace hp_fp
{
	extern const double CURRENT_TIME_ADDITION;
	struct TimerMut
	{
		friend TimerMut initTimer_IO( );
		friend void updateTimer_IO( TimerMut& timer );
		TimerMut( const TimerMut& ) = delete;
		TimerMut operator=( const TimerMut& ) = delete;
		TimerMut( TimerMut&& r ) : deltaMs( std::move( r.deltaMs ) ), _currentTimeMs( std::move( r._currentTimeMs ) ), _lastTimeMs( std::move( r._lastTimeMs ) )
		{ }
		TimerMut operator=( TimerMut&& r )
		{
			return TimerMut{ r.deltaMs, r._currentTimeMs, r._lastTimeMs };
		}
		double deltaMs;
		double currentTimeMs( )
		{
			return _currentTimeMs - CURRENT_TIME_ADDITION;
		}
	private:
		TimerMut( const double deltaMs, const double lastTimeMs, const double currentTimeMs ) : deltaMs( deltaMs ), _lastTimeMs( lastTimeMs ), _currentTimeMs( currentTimeMs + CURRENT_TIME_ADDITION )
		{ }
		double _currentTimeMs;
		double _lastTimeMs;
	};
}
