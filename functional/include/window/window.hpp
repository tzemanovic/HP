#pragma once
#include "windowConfigImm.hpp"

namespace hp_fp
{
	namespace Window
	{
		Window open( const String&& windowName, const WindowConfigImm& windowConfig );
		std::tuple<unsigned, unsigned, unsigned> getCurrentMode( );
	}
}