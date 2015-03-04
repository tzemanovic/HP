#pragma once
#include <adt/list.hpp>
namespace hp_fp
{
	template<typename A>
	struct Tree
	{
	private:
		struct Item
		{
			Item( A v ) : val( v )
			{ }
			A _node;
			List<std::shared_ptr<const Item>> _leaves;
		};
		friend Item;
		List<std::shared_ptr<const Item>> _root;
	public:

	};
}