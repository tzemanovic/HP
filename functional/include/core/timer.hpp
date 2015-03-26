#pragma once
namespace hp_fp
{
	extern const double TIME_ADDITION;
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  -  ][  0  ][  0  ][  +  ][  +  ]
	struct Timer
	{
		friend Timer initTimer_IO( );
		friend void updateTimer_IO( Timer& timer );
		Timer( const Timer& ) = delete;
		Timer operator = ( const Timer& ) = delete;
		Timer( Timer&& r ) : deltaMs( std::move( r.deltaMs ) ),
			_timeMs( std::move( r._timeMs ) ), _lastTimeMs( std::move( r._lastTimeMs ) )
		{ }
		Timer operator = ( Timer&& r )
		{
			return Timer{ r.deltaMs, r._timeMs, r._lastTimeMs };
		}
		double deltaMs;
		double timeMs( ) const
		{
			return _timeMs - TIME_ADDITION;
		}
	private:
		Timer( const double deltaMs, const double lastTimeMs, const double timeMs )
			: deltaMs( deltaMs ), _lastTimeMs( lastTimeMs ), _timeMs( timeMs + TIME_ADDITION )
		{ }
		double _timeMs;
		double _lastTimeMs;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	Timer initTimer_IO( );
	void updateTimer_IO( Timer& timer );
	namespace
	{
		double getTimeMs_IO( );
	}
}