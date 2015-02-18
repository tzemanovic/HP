#pragma once

namespace hp_fp
{

	template<typename A>
	struct IO
	{
		template<typename B> friend const IO<B> io( const B& b );
	public:
		const A& operator() ( ) const
		{
			return val;
		}
	private:
		IO( const A& a ) : val( a )
		{ }
		IO( const A&& a ) : val( a )
		{ }
	private:
		A val;
	};

	template< >
	struct IO < void >
	{ };

	template<typename A>
	const IO<A> io( const A& a )
	{
		return IO<A>( a );
	}
}