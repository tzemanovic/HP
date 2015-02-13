#pragma once
#include "windowStyle.hpp"

// Generated using tools/immutableStruct.hs:
// gen "WindowConfig" [("UInt", "width"), ("UInt", "height"), ("WindowStyle", "windowStyle"), ("UInt", "bitsPerPx")] ["windowStyle"]
namespace hp_fp
{
	struct WindowConfigImm
	{
	public:
		const UInt width;
		const UInt height;
		const WindowStyle windowStyle;
		const UInt bitsPerPx;
	public:
		const WindowConfigImm setWidth( const UInt w ) const
		{
			return WindowConfigImm{ w, height, windowStyle, bitsPerPx };
		}
		const WindowConfigImm setHeight( const UInt h ) const
		{
			return WindowConfigImm{ width, h, windowStyle, bitsPerPx };
		}
		const WindowConfigImm setWindowStyle( const WindowStyle w ) const
		{
			return WindowConfigImm{ width, height, w, bitsPerPx };
		}
		const WindowConfigImm setBitsPerPx( const UInt b ) const
		{
			return WindowConfigImm{ width, height, windowStyle, b };
		}
	};
}
