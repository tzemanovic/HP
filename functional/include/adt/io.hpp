#pragma once

namespace hp_fp
{
	template<typename T>
	struct IO
	{
		typedef std::function<T( )> type;
	};
}