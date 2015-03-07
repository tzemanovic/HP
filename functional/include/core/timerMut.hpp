#pragma once
// [const][cop-c][cop-a][mov-c][mov-a]
// [  -  ][  0  ][  0  ][  +  ][  +  ]
namespace hp_fp
{
	extern const double TIME_ADDITION;
	struct TimerMut
	{
		friend TimerMut initTimer_IO( );
		friend void updateTimer_IO( TimerMut& timer );
		TimerMut( const TimerMut& ) = delete;
		TimerMut operator=( const TimerMut& ) = delete;
		TimerMut( TimerMut&& r ) : deltaMs( std::move( r.deltaMs ) ),
			_timeMs( std::move( r._timeMs ) ), _lastTimeMs( std::move( r._lastTimeMs ) )
		{ }
		TimerMut operator=( TimerMut&& r )
		{
			return TimerMut{ r.deltaMs, r._timeMs, r._lastTimeMs };
		}
		double deltaMs;
		double timeMs( )
		{
			return _timeMs - TIME_ADDITION;
		}
	private:
		TimerMut( const double deltaMs, const double lastTimeMs, const double timeMs )
			: deltaMs( deltaMs ), _lastTimeMs( lastTimeMs ), _timeMs( timeMs + TIME_ADDITION )
		{ }
		double _timeMs;
		double _lastTimeMs;
	};
}
