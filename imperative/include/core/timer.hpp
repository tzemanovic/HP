#pragma once
namespace hp_ip
{
	extern const double TIME_ADDITION;
	class Timer
	{
	public:
		Timer( )
			: _lastTimeMs( getTimeMs( ) ), _timeMs( TIME_ADDITION )
		{ }
		double update( );
	private:
		double getTimeMs( );
		double _timeMs;
		double _lastTimeMs;
	public:
		double timeMs( ) const
		{
			return _timeMs - TIME_ADDITION;
		}
	};
}

