#include <pch.hpp>
#include "../../include/window/window.hpp"
namespace hp_ip
{
	bool Window::open( )
	{
		if ( isOnlyInstance( ) )
		{
			// window class details
			WNDCLASSEX windowClass = { 0 };
			windowClass.cbSize = sizeof( WNDCLASSEX );
			windowClass.style = CS_VREDRAW | CS_HREDRAW;
			windowClass.lpfnWndProc = &Window::staticWindowProc;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle( nullptr );
			windowClass.hIcon = 0;
			windowClass.hIconSm = 0;
			windowClass.hCursor = 0;
			windowClass.hbrBackground = 0;
			windowClass.lpszMenuName = 0;
			windowClass.lpszClassName = _name.c_str( );
			// register the window
			if ( RegisterClassEx( &windowClass ) )
			{
				// find position and size
				HDC screenDC = GetDC( nullptr );
				unsigned left = ( GetDeviceCaps( screenDC, HORZRES ) - _windowConfig.width ) / 2;
				unsigned top = ( GetDeviceCaps( screenDC, VERTRES ) - _windowConfig.height ) / 2;
				unsigned width = _windowConfig.width;
				unsigned height = _windowConfig.height;
				ReleaseDC( nullptr, screenDC );
				// set the style of the window
				DWORD style = WS_VISIBLE;
				if ( _windowConfig.windowStyle == WindowStyle::Window )
				{
					style |= WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU;
					// adjust the window size with the borders etc.
					RECT rectangle = { 0, 0, _windowConfig.width, _windowConfig.height };
					AdjustWindowRect( &rectangle, style, false );
					width = rectangle.right - rectangle.left;
					height = rectangle.bottom - rectangle.top;
				}
				// create the window
				_handle = CreateWindowEx( 0, _name.c_str( ), _name.c_str( ), style, left, top, width, height,
					GetDesktopWindow( ), nullptr, GetModuleHandle( nullptr ), this );
				if ( _handle == nullptr )
				{
					ERR( GetLastError( ) );
					return false;
				}
				else if ( _windowConfig.windowStyle == WindowStyle::Fullscreen )
				{
					switchToFullscreen( );
				}
				_open = true;
			}
			else
			{
				ERR( GetLastError( ) );
			}
		}
		return _open;
	}
	void Window::switchToFullscreen( )
	{
		// set display settings
		DEVMODE devMode;
		devMode.dmSize = sizeof( devMode );
		devMode.dmPelsWidth = _windowConfig.width;
		devMode.dmPelsHeight = _windowConfig.height;
		devMode.dmBitsPerPel = _windowConfig.bitsPerPx;
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		// change default display device settings
		if ( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			return;
		}
		// set window style
		SetWindowLong( _handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
		// set extended window style
		SetWindowLong( _handle, GWL_EXSTYLE, WS_EX_APPWINDOW );
		// set window size, position and z-order
		SetWindowPos( _handle, HWND_TOP, 0, 0, _windowConfig.width, _windowConfig.height,
			SWP_FRAMECHANGED );
		// show the window
		setWindowVisibility( _handle, true );
	}
	void Window::processMessages( )
	{
		MSG message;
		while ( PeekMessage( &message, _handle, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
	bool Window::isOpen( )
	{
		return _open;
	}
	void Window::captureMouse( )
	{
		SetCapture( _handle );
	}
	void Window::releaseMouse( )
	{
		ReleaseCapture( );
	}
	LRESULT CALLBACK Window::windowProc( HWND handle, UINT message, WPARAM wParam,
		LPARAM lParam )
	{
		switch ( message )
		{
		case WM_CLOSE:
		{
			_open = false;
			break;
		}
		case WM_ACTIVATEAPP:
		{
			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			_gameInput[static_cast<Key>( wParam )] = true;
			break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			_gameInput[static_cast<Key>( wParam )] = false;
			break;
		}
		case WM_CHAR:
		{
			_gameInput.text = wParam;
			break;
		}
		case WM_MOUSEMOVE:
		{
			_gameInput.mouse.x = LOWORD( lParam );
			_gameInput.mouse.y = HIWORD( lParam );
			break;
		}
		case WM_LBUTTONDOWN:
		{
			captureMouse( );
			_gameInput[MouseButton::LeftButton] = true;
			break;
		}
		case WM_LBUTTONUP:
		{
			releaseMouse( );
			_gameInput[MouseButton::LeftButton] = false;
			break;
		}
		case WM_RBUTTONDOWN:
		{
			captureMouse( );
			_gameInput[MouseButton::RightButton] = true;
			break;
		}
		case WM_RBUTTONUP:
		{
			releaseMouse( );
			_gameInput[MouseButton::RightButton] = false;
			break;
		}
		case WM_MBUTTONDOWN:
		{
			captureMouse( );
			_gameInput[MouseButton::MiddleButton] = true;
			break;
		}
		case WM_MBUTTONUP:
		{
			releaseMouse( );
			_gameInput[MouseButton::MiddleButton] = false;
			break;
		}
		case WM_MOUSEWHEEL:
		{
			_gameInput.mouse.delta = HIWORD( wParam ) / 120;
			break;
		}
		case WM_XBUTTONDOWN:
		{
			captureMouse( );
			_gameInput[HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2] = true;
			break;
		}
		case WM_XBUTTONUP:
		{
			releaseMouse( );
			_gameInput[HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2] = false;
			break;
		}
		default:
			return DefWindowProc( handle, message, wParam, lParam );
		}
		return FALSE;
	}
	LRESULT CALLBACK Window::staticWindowProc( HWND handle, UINT message, WPARAM wParam,
		LPARAM lParam )
	{
		Window* window = reinterpret_cast<Window*>( GetWindowLongPtr( handle, GWL_USERDATA ) );
		switch ( message )
		{
		case WM_CREATE:
		{
			CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT*>( lParam );
			window = reinterpret_cast<Window*>( cs->lpCreateParams );
			SetLastError( 0 );
			if ( SetWindowLongPtr( handle, GWL_USERDATA, reinterpret_cast<LONG_PTR>( window ) )
				== 0 )
			{
				if ( GetLastError( ) != 0 )
				{
					ERR( "Unable to set window user data." );
					return FALSE;
				}
			}
			break;
		}
		default:
			return window->windowProc( handle, message, wParam, lParam );
		}
		return FALSE;
	}
	WindowConfig Window::defaultWindowConfig( )
	{
		DEVMODE mode;
		mode.dmSize = sizeof( mode );
		EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
		return WindowConfig{ mode.dmPelsWidth, mode.dmPelsHeight, WindowStyle::Default, mode.dmBitsPerPel };
	}
	void Window::setWindowVisibility( WindowHandle handle, const bool visible )
	{
		ShowWindow( handle, visible ? SW_SHOW : SW_HIDE );
		if ( visible )
		{
			SetFocus( handle );
			SetForegroundWindow( handle );
			SetActiveWindow( handle );
		}
	}
	bool Window::isOnlyInstance( )
	{
		HANDLE handle = CreateMutex( nullptr, true, _name.c_str( ) );
		if ( GetLastError( ) != ERROR_SUCCESS )
		{
			WindowHandle windowHandle = FindWindow( _name.c_str( ), nullptr );
			if ( windowHandle != nullptr )
			{
				Window::setWindowVisibility( windowHandle, true );
				return false;
			}
		}
		return true;
	}
}