#pragma once

namespace hp_fp
{
	template< >
	struct IO < void >
	{ };

	template<typename A>
	struct IO
	{
	public:
		IO( const A& a ) : val( a )
		{ }
		const A& operator() ( ) const
		{
			return val;
		}
	private:
		A val;
	};
}