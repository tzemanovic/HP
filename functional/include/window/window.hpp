#pragma once
#include <window/windowConfigImm.hpp>
#include <window/windowMut.hpp>
#include <adt/maybe.hpp>

namespace hp_fp
{
	Maybe<WindowMut> open( const String&& windowName, const WindowConfigImm& windowConfig );

	const WindowConfigImm defaultWindowConfig( );
	//std::tuple<unsigned, unsigned, unsigned> getCurrentMode( );

	bool isOnlyInstance( const LPCWSTR windowName );

	void setWindowVisibility( WindowHandle windowHandle, bool visible );

	void switchToFullscreen( WindowHandle windowHandle, const WindowConfigImm& windowConfig );

	LRESULT CALLBACK windowProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
}