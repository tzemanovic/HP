#pragma once
#include "../adt/maybe.hpp"
namespace hp_fp
{
	enum struct WindowStyle : unsigned
	{
		Window,
		Fullscreen,
		Default = Window
	};
	struct WindowConfig
	{
		UInt width;
		UInt height;
		WindowStyle windowStyle;
		UInt bitsPerPx;
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  +  ][  +  ]
	struct Window
	{
		Window( WindowHandle handle, const LPCWSTR name ) : handle( handle ), name( name )
		{ }
		Window( const Window& ) = delete;
		Window( Window&& w ) : handle( std::move( w.handle ) ), name( std::move( w.name ) )
		{ }
		Window operator = ( const Window& ) = delete;
		Window operator = ( Window&& w )
		{
			return Window{ std::move( w ) };
		}
		WindowHandle	handle;
		const LPCWSTR	name;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	struct Engine;
	Maybe<Window> open_IO( Engine& engine, const WindowConfig& windowConfig );
	WindowConfig defaultWindowConfig_IO( );
	void setWindowVisibility_IO( WindowHandle windowHandle, const bool visible );
	void switchToFullscreen_IO( WindowHandle windowHandle, const WindowConfig& windowConfig );
	void processMessages_IO( WindowHandle windowHandle );
	void captureMouse_IO( WindowHandle windowHandle );
	void releaseMouse_IO( );
	namespace
	{
		LRESULT CALLBACK windowProc_IO( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam );
		bool isOnlyInstance_IO( const LPCWSTR windowName );
	}
}