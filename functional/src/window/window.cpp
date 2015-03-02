#include <pch.hpp>
#include <core/engine.hpp>
#include <utils/stringUtils.hpp>
#include <window/inputMessage.hpp>
#include <window/window.hpp>
#include <adt/io.hpp>
#include <adt/maybe.hpp>
namespace hp_fp
{
	Maybe<WindowMut> open_IO( EngineMut& engine, const WindowConfigImm& windowConfig )
	{
		std::wstring windowNameW = s2ws( "HP_FP::" + engine.name );
		WindowMut window{ nullptr, windowNameW.c_str( ) };
		if ( isOnlyInstance_IO( window.name ) )
		{
			// window class details
			WNDCLASSEX windowClass = { 0 };
			windowClass.cbSize = sizeof( WNDCLASSEX );
			windowClass.style = CS_VREDRAW | CS_HREDRAW;
			windowClass.lpfnWndProc = &windowProc_IO;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle( nullptr );
			windowClass.hIcon = 0;
			windowClass.hIconSm = 0;
			windowClass.hCursor = 0;
			windowClass.hbrBackground = 0;
			windowClass.lpszMenuName = 0;
			windowClass.lpszClassName = window.name;
			// register the window
			if ( RegisterClassEx( &windowClass ) )
			{
				// find position and size
				HDC screenDC = GetDC( nullptr );
				unsigned left = ( GetDeviceCaps( screenDC, HORZRES ) - windowConfig.width ) / 2;
				unsigned top = ( GetDeviceCaps( screenDC, VERTRES ) - windowConfig.height ) / 2;
				unsigned width = windowConfig.width;
				unsigned height = windowConfig.height;
				ReleaseDC( nullptr, screenDC );
				// set the style of the window
				DWORD style = WS_VISIBLE;
				if ( windowConfig.windowStyle == WindowStyle::Window )
				{
					style |= WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU;
					// adjust the window size with the borders etc.
					RECT rectangle = { 0, 0, windowConfig.width, windowConfig.height };
					AdjustWindowRect( &rectangle, style, false );
					width = rectangle.right - rectangle.left;
					height = rectangle.bottom - rectangle.top;
				}
				// create the window
				window.handle = CreateWindowEx( 0, window.name, window.name, style, left, top, width, height, GetDesktopWindow( ), nullptr, GetModuleHandle( nullptr ), &engine );
				if ( window.handle == nullptr )
				{
					ERR( GetLastError( ) );
					return nothing<WindowMut>( );
				}
				if ( windowConfig.windowStyle == WindowStyle::Fullscreen )
				{
					switchToFullscreen_IO( window.handle, windowConfig );
				}
			}
		}
		return just( std::move( window ) );
	}
	const WindowConfigImm defaultWindowConfig_IO( )
	{
		DEVMODE mode;
		mode.dmSize = sizeof( mode );
		EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
		return WindowConfigImm{ mode.dmPelsWidth, mode.dmPelsHeight, WindowStyle::Default, mode.dmBitsPerPel };
	}
	void setWindowVisibility_IO( WindowHandle windowHandle, bool visible )
	{
		ShowWindow( windowHandle, visible ? SW_SHOW : SW_HIDE );
		if ( visible )
		{
			SetFocus( windowHandle );
			SetForegroundWindow( windowHandle );
			SetActiveWindow( windowHandle );
		}
	}
	void switchToFullscreen_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig )
	{
		// set display settings
		DEVMODE devMode;
		devMode.dmSize = sizeof( devMode );
		devMode.dmPelsWidth = windowConfig.width;
		devMode.dmPelsHeight = windowConfig.height;
		devMode.dmBitsPerPel = windowConfig.bitsPerPx;
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		// change default display device settings
		if ( ChangeDisplaySettings( &devMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			return;
		}
		// set window style
		SetWindowLong( windowHandle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS );
		// set extended window style
		SetWindowLong( windowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW );
		// set window size, position and z-order
		SetWindowPos( windowHandle, HWND_TOP, 0, 0, windowConfig.width, windowConfig.height, SWP_FRAMECHANGED );
		// show the window
		setWindowVisibility_IO( windowHandle, true );
	}
	void processMessages_IO( WindowHandle windowHandle )
	{
		MSG message;
		while ( PeekMessage( &message, windowHandle, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
	void captureMouse_IO( WindowHandle windowHandle )
	{
		SetCapture( windowHandle );
	}
	void releaseMouse_IO( )
	{
		ReleaseCapture( );
	}
	namespace
	{
		LRESULT CALLBACK windowProc_IO( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam )
		{
			EngineMut* engine = reinterpret_cast<EngineMut*>( GetWindowLongPtr( handle, GWL_USERDATA ) );
			switch ( message )
			{
			case WM_CREATE:
			{
				CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT*>( lParam );
				engine = reinterpret_cast<EngineMut*>( cs->lpCreateParams );
				SetLastError( 0 );
				if ( SetWindowLongPtr( handle, GWL_USERDATA, reinterpret_cast<LONG_PTR>( engine ) ) == 0 )
				{
					if ( GetLastError( ) != 0 )
					{
						ERR( "Unable to set window user data." );
						return FALSE;
					}
				}
				break;
			}
			case WM_CLOSE:
			{
				engine->onClose( *engine );
				break;
			}
			case WM_ACTIVATEAPP:
			{
				break;
			}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				// if not repeated
				if ( !( lParam & ( 1 << 30 ) ) )
				{
					KeyMessage msg = KeyMessage{ static_cast<Key>( wParam ), KeyMods(
						( HIWORD( GetKeyState( VK_LCONTROL ) ) != 0 ) ? KeyMods::L_CTRL : 0 |
						( HIWORD( GetKeyState( VK_RCONTROL ) ) != 0 ) ? KeyMods::R_CTRL : 0 |
						( HIWORD( GetKeyState( VK_LMENU ) ) != 0 ) ? KeyMods::L_ALT : 0 |
						( HIWORD( GetKeyState( VK_RMENU ) ) != 0 ) ? KeyMods::R_ALT : 0 |
						( HIWORD( GetKeyState( VK_LSHIFT ) ) != 0 ) ? KeyMods::L_SHIFT : 0 |
						( HIWORD( GetKeyState( VK_RSHIFT ) ) != 0 ) ? KeyMods::R_SHIFT : 0
						) };
					engine->onKeyDown( *engine, std::move( msg ) );
				}
				break;
			}
			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				KeyMessage msg = KeyMessage{ static_cast<Key>( wParam ), KeyMods(
					( HIWORD( GetKeyState( VK_LCONTROL ) ) != 0 ) ? KeyMods::L_CTRL : 0 |
					( HIWORD( GetKeyState( VK_RCONTROL ) ) != 0 ) ? KeyMods::R_CTRL : 0 |
					( HIWORD( GetKeyState( VK_LMENU ) ) != 0 ) ? KeyMods::L_ALT : 0 |
					( HIWORD( GetKeyState( VK_RMENU ) ) != 0 ) ? KeyMods::R_ALT : 0 |
					( HIWORD( GetKeyState( VK_LSHIFT ) ) != 0 ) ? KeyMods::L_SHIFT : 0 |
					( HIWORD( GetKeyState( VK_RSHIFT ) ) != 0 ) ? KeyMods::R_SHIFT : 0
					) };
				engine->onKeyUp( *engine, std::move( msg ) );
				break;
			}
			case WM_CHAR:
			{
				engine->onText( *engine, TextMessage{ wParam } );
				break;
			}
			case WM_MOUSEMOVE:
			{
				MouseMoveMessage msg = MouseMoveMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseMove( *engine, std::move( msg ) );
				break;
			}
			case WM_LBUTTONDOWN:
			{
				captureMouse_IO( handle );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::LeftButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonDown( *engine, std::move( msg ) );
				break;
			}
			case WM_LBUTTONUP:
			{
				releaseMouse_IO( );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::LeftButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonUp( *engine, std::move( msg ) );
				break;
			}
			case WM_RBUTTONDOWN:
			{
				captureMouse_IO( handle );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::RightButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonDown( *engine, std::move( msg ) );
				break;
			}
			case WM_RBUTTONUP:
			{
				releaseMouse_IO( );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::RightButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonUp( *engine, std::move( msg ) );
				break;
			}
			case WM_MBUTTONDOWN:
			{
				captureMouse_IO( handle );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::MiddleButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonDown( *engine, std::move( msg ) );
				break;
			}
			case WM_MBUTTONUP:
			{
				releaseMouse_IO( );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), MouseButton::MiddleButton, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonUp( *engine, std::move( msg ) );
				break;
			}
			case WM_MOUSEWHEEL:
			{
				POINT screenPos;
				screenPos.x = static_cast<LONG>( LOWORD( lParam ) );
				screenPos.y = static_cast<LONG>( HIWORD( lParam ) );
				ScreenToClient( handle, &screenPos );
				MouseWheelMessage msg = MouseWheelMessage{ static_cast<UInt16>( screenPos.x ), static_cast<UInt16>( screenPos.y ), HIWORD( wParam ) / 120, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseWheel( *engine, std::move( msg ) );
				break;
			}
			case WM_XBUTTONDOWN:
			{
				captureMouse_IO( handle );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonDown( *engine, std::move( msg ) );
				break;
			}
			case WM_XBUTTONUP:
			{
				releaseMouse_IO( );
				MouseButtonMessage msg = MouseButtonMessage{ LOWORD( lParam ), HIWORD( lParam ), HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2, MouseMods(
					( ( wParam & MK_CONTROL ) != 0 ) ? MouseMods::CTRL : 0 |
					( ( wParam & MK_LBUTTON ) != 0 ) ? MouseMods::L_BUTTON : 0 |
					( ( wParam & MK_MBUTTON ) != 0 ) ? MouseMods::M_BUTTON : 0 |
					( ( wParam & MK_RBUTTON ) != 0 ) ? MouseMods::R_BUTTON : 0 |
					( ( wParam & MK_SHIFT ) != 0 ) ? MouseMods::SHIFT : 0 |
					( ( wParam & MK_XBUTTON1 ) != 0 ) ? MouseMods::X_BUTTON1 : 0 |
					( ( wParam & MK_XBUTTON2 ) != 0 ) ? MouseMods::X_BUTTON2 : 0
					) };
				engine->onMouseButtonUp( *engine, std::move( msg ) );
				break;
			}
			default:
				return DefWindowProc( handle, message, wParam, lParam );
			}
			return FALSE;
		}
		bool isOnlyInstance_IO( const LPCWSTR windowName )
		{
			HANDLE handle = CreateMutex( nullptr, true, windowName );
			if ( GetLastError( ) != ERROR_SUCCESS )
			{
				WindowHandle windowHandle = FindWindow( windowName, nullptr );
				if ( windowHandle != nullptr )
				{
					setWindowVisibility_IO( windowHandle, true );
					return false;
				}
			}
			return true;
		}
	}
}