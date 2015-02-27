#pragma once

namespace hp_fp
{
	template<typename A>
	struct functionTraits;

	template<typename A, typename... Bs>
	struct functionTraits<A( *)( Bs... )> : public functionTraits < A( Bs... ) >
	{ };

	template<typename A, typename... Bs>
	struct functionTraits<A(Bs...)>
	{
		using return_type = A;
		//static constexpr std::size_t arity = sizeof...(Bs);
		template<std::size_t C>
		struct arg
		{
			using type = typename std::tuple_element<C, std::tuple<Bs...>>::type;
		};
	};
}