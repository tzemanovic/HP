#pragma once
#include <window/windowConfigImm.hpp>
#include <window/windowMut.hpp>

namespace hp_fp
{
	template<typename A> struct IO;
	template<typename A> struct Maybe;
	struct EngineMut;

	IO<Maybe<WindowMut>> open( const String&& windowName, const WindowConfigImm& windowConfig );
	IO<Maybe<WindowMut>> open( EngineMut& engine, const String&& windowName, const WindowConfigImm& windowConfig );
	const WindowConfigImm defaultWindowConfig( );
	void setWindowVisibility( WindowHandle windowHandle, bool visible );
	void switchToFullscreen( WindowHandle windowHandle, const WindowConfigImm& windowConfig );
	LRESULT CALLBACK windowProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
	void processMessages( WindowHandle windowHandle );
	namespace
	{
		bool isOnlyInstance( const LPCWSTR windowName );
	}
}