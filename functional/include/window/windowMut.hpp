#pragma once

namespace hp_fp
{
	struct WindowMut
	{
		WindowHandle	handle;
#if defined( HP_PLATFORM_WIN32 )
		const LPCWSTR	name;
#endif
	};
}