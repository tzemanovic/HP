#pragma once
#include <window/windowConfigImm.hpp>
#include <window/windowMut.hpp>
namespace hp_fp
{
	template<typename A> struct Maybe;
	struct Engine;

	Maybe<WindowMut> open_IO( Engine& engine, const WindowConfigImm& windowConfig );
	const WindowConfigImm defaultWindowConfig_IO( );
	void setWindowVisibility_IO( WindowHandle windowHandle, bool visible );
	void switchToFullscreen_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig );
	void processMessages_IO( WindowHandle windowHandle );
	void captureMouse_IO( WindowHandle windowHandle );
	void releaseMouse_IO( );
	namespace
	{
		LRESULT CALLBACK windowProc_IO( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
		bool isOnlyInstance_IO( const LPCWSTR windowName );
	}
}