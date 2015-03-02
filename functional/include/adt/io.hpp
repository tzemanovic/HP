#pragma once
namespace hp_fp
{

	template<typename A>
	struct IO
	{
		template<typename B> friend const IO<B> io( B&& b );
	public:
		const A& operator() ( ) const
		{
			return val;
		}
	private:
		IO( A& a ) : val( a )
		{ }
		/*IO( A&& a ) : val( std::forward<A>(a) )
		{ }*/
	private:
		A val;
	};

	template< >
	struct IO < void >
	{ };

	template<typename A>
	const IO<A> io( A&& a )
	{
		return IO<A>( std::forward<A>(a) );
	}
}