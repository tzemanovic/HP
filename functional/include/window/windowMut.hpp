#pragma once

namespace hp_fp
{
	struct Window
	{
	public:
		WindowHandle	handle;
#if defined( HP_PLATFORM_WIN32 )
		LPCWSTR			name;
#endif
	};
}