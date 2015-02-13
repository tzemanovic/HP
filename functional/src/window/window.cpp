#include "pch.hpp"
#include "../../include/window/window.hpp"

namespace hp_fp
{
	namespace Window
	{
		WindowHandle open( const String&& windowName, const WindowConfigImm& windowConfig )
		{
			return nullptr;
			std::wstring windowNameW = s2ws( windowName );
			_windowName = windowNameW.c_str( );
			if ( isOnlyInstance( ) )
			{
				// window class details
				WNDCLASSEX windowClass = { 0 };
				windowClass.cbSize = sizeof( WNDCLASSEX );
				windowClass.style = CS_VREDRAW | CS_HREDRAW;
				windowClass.lpfnWndProc = &staticWndProc;
				windowClass.cbClsExtra = 0;
				windowClass.cbWndExtra = 0;
				windowClass.hInstance = GetModuleHandle( nullptr );
				windowClass.hIcon = 0;
				windowClass.hIconSm = 0;
				windowClass.hCursor = 0;
				windowClass.hbrBackground = 0;
				windowClass.lpszMenuName = 0;
				windowClass.lpszClassName = _windowName;
				// register the window
				if ( RegisterClassEx( &windowClass ) )
				{
					// find position and size
					HDC screenDC = GetDC( nullptr );
					unsigned left = ( GetDeviceCaps( screenDC, HORZRES ) - _width ) / 2;
					unsigned top = ( GetDeviceCaps( screenDC, VERTRES ) - _height ) / 2;
					unsigned width = _width;
					unsigned height = _height;
					ReleaseDC( nullptr, screenDC );
					// set the style of the window
					DWORD style = WS_VISIBLE;
					if ( _windowStyle == WindowStyle::Windowed )
					{
						style |= WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU;
						// adjust the window size with the borders etc.
						RECT rectangle = { 0, 0, _width, _height };
						AdjustWindowRect( &rectangle, style, false );
						width = rectangle.right - rectangle.left;
						height = rectangle.bottom - rectangle.top;
					}
					// create the window
					_windowHandle = CreateWindowEx( 0, _windowName, _windowName, style, left, top, width, height, GetDesktopWindow( ), nullptr, GetModuleHandle( nullptr ), this );
					_open = true;
					if ( _windowStyle == WindowStyle::Fullscreen )
					{
						switchToFullscreen( );
					}
					_openGl = new OpenGl( openGlConfig, _windowHandle, windowConfig.getBitsPerPx( ) );
				}
			}
		}

		std::tuple<unsigned, unsigned, unsigned> Window::getCurrentMode( )
		{
			DEVMODE mode;
			mode.dmSize = sizeof( mode );
			EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
			return std::make_tuple( mode.dmPelsWidth, mode.dmPelsHeight, mode.dmBitsPerPel );
		}
	}
}