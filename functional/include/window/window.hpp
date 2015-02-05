#pragma once
#include "windowConfig.hpp"

namespace hp_fp
{
	struct Window
	{
	private:
		WindowHandle	_handle;
		LPCWSTR			_name;
	};

	namespace Window
	{
		Window open( const String&& windowName, const WindowConfig& windowConfig );
		std::tuple<unsigned, unsigned, unsigned> getCurrentMode( );
	}
}