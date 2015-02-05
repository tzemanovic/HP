#pragma once
#include "windowStyle.hpp"

// Generated using tools/immutableStruct.hs:
// gen' "WindowConfig" [("unsigned", "width"), ("unsigned", "height"), ("WindowStyle", "windowStyle"), ("unsigned", "bitsPerPx")] ["windowStyle"]
namespace hp_fp
{
	struct WindowConfig
	{
	public:
		const unsigned _width;
		const unsigned _height;
		const WindowStyle _windowStyle;
		const unsigned _bitsPerPx;
	public:
		const WindowConfig width( const unsigned width ) const
		{
			return WindowConfig{ width, _height, _windowStyle, _bitsPerPx };
		}
		const WindowConfig height( const unsigned height ) const
		{
			return WindowConfig{ _width, height, _windowStyle, _bitsPerPx };
		}
		const WindowConfig windowStyle( const WindowStyle windowStyle ) const
		{
			return WindowConfig{ _width, _height, windowStyle, _bitsPerPx };
		}
		const WindowConfig bitsPerPx( const unsigned bitsPerPx ) const
		{
			return WindowConfig{ _width, _height, _windowStyle, bitsPerPx };
		}
	};
}
