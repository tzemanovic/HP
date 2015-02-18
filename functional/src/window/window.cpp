#include <pch.hpp>
#include <window/window.hpp>
#include <utils/stringUtils.hpp>
#include <window/inputMessage.hpp>

namespace hp_fp
{
	IO<Maybe<WindowMut>> open( const String&& windowName, const WindowConfigImm& windowConfig )
	{
		std::wstring windowNameW = s2ws( windowName );
		//_windowName = windowNameW.c_str( );
		WindowMut window{ nullptr, windowNameW.c_str( ) };
		if ( isOnlyInstance( window.name ) )
		{
			// window class details
			WNDCLASSEX windowClass = { 0 };
			windowClass.cbSize = sizeof( WNDCLASSEX );
			windowClass.style = CS_VREDRAW | CS_HREDRAW;
			windowClass.lpfnWndProc = &windowProc;
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
				window.handle = CreateWindowEx( 0, window.name, window.name, style, left, top, width, height, GetDesktopWindow( ), nullptr, GetModuleHandle( nullptr ), nullptr );
				if ( window.handle == nullptr )
				{
					ERR( GetLastError( ) );
					return io(nothing<WindowMut>( ) );
				}
				if ( windowConfig.windowStyle == WindowStyle::Fullscreen )
				{
					switchToFullscreen( window.handle, windowConfig );
				}
			}
		}
		return io( just( &window ) );
	}

	const WindowConfigImm defaultWindowConfig( )
	{
		DEVMODE mode;
		mode.dmSize = sizeof( mode );
		EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
		return WindowConfigImm{ mode.dmPelsWidth, mode.dmPelsHeight, WindowStyle::Default, mode.dmBitsPerPel };
	}

	std::tuple<unsigned, unsigned, unsigned> getCurrentMode( )
	{
		DEVMODE mode;
		mode.dmSize = sizeof( mode );
		EnumDisplaySettings( nullptr, ENUM_CURRENT_SETTINGS, &mode );
		return std::make_tuple( mode.dmPelsWidth, mode.dmPelsHeight, mode.dmBitsPerPel );
	}

	bool isOnlyInstance( const LPCWSTR windowName )
	{
		HANDLE handle = CreateMutex( nullptr, true, windowName );
		if ( GetLastError( ) != ERROR_SUCCESS )
		{
			WindowHandle windowHandle = FindWindow( windowName, nullptr );
			if ( windowHandle != nullptr )
			{
				setWindowVisibility( windowHandle, true );
				return false;
			}
		}
		return true;
	}

	void setWindowVisibility( WindowHandle windowHandle, bool visible )
	{
		ShowWindow( windowHandle, visible ? SW_SHOW : SW_HIDE );
		if ( visible )
		{
			SetFocus( windowHandle );
			SetForegroundWindow( windowHandle );
			SetActiveWindow( windowHandle );
		}
	}

	void switchToFullscreen( WindowHandle windowHandle, const WindowConfigImm& windowConfig )
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
		setWindowVisibility( windowHandle, true );
	}

	LRESULT CALLBACK windowProc( WindowHandle handle, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch ( message )
		{
		case WM_CLOSE:
		{
			// create close message and push it to the queue
			InputMessage msg( CloseMessage{ } );
			//_messages.push( msg );
			break;
		}
		default:
			// default window procedure
			return DefWindowProc( handle, message, wParam, lParam );
		}
		//switch ( message )
		//{
		//case WM_CLOSE:
		//{
		//	// create close message and push it to the queue
		//	InputMessage msg( InputMessage::Type::Close );
		//	_messages.push( msg );
		//	break;
		//}
		//case WM_ACTIVATEAPP:
		//{
		//	if ( wParam && _mouseCaptured )
		//	{
		//		captureMouse( );
		//	}
		//	break;
		//}
		//case WM_KEYDOWN:
		//case WM_SYSKEYDOWN:
		//{
		//	if ( !_oldKeyStates[wParam] )
		//	{
		//		InputMessage msg( InputMessage::Type::KeyDown );
		//		msg.key.type = ( Key ) wParam;
		//		msg.key.lCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		//		msg.key.rCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		//		msg.key.lAlt = HIWORD( GetKeyState( VK_LMENU ) ) != 0;
		//		msg.key.rAlt = HIWORD( GetKeyState( VK_RMENU ) ) != 0;
		//		msg.key.lShift = HIWORD( GetKeyState( VK_LSHIFT ) ) != 0;
		//		msg.key.rShift = HIWORD( GetKeyState( VK_RSHIFT ) ) != 0;
		//		_messages.push( msg );
		//		_oldKeyStates[wParam] = true;
		//	}
		//	break;
		//}
		//case WM_KEYUP:
		//case WM_SYSKEYUP:
		//{
		//	InputMessage msg( InputMessage::Type::KeyUp );
		//	msg.key.type = ( Key ) wParam;
		//	msg.key.lCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		//	msg.key.rCtrl = HIWORD( GetKeyState( VK_RCONTROL ) ) != 0;
		//	msg.key.lAlt = HIWORD( GetKeyState( VK_LMENU ) ) != 0;
		//	msg.key.rAlt = HIWORD( GetKeyState( VK_RMENU ) ) != 0;
		//	msg.key.lShift = HIWORD( GetKeyState( VK_LSHIFT ) ) != 0;
		//	msg.key.rShift = HIWORD( GetKeyState( VK_RSHIFT ) ) != 0;
		//	_messages.push( msg );
		//	_oldKeyStates[wParam] = false;
		//	break;
		//}
		//case WM_CHAR:
		//{
		//	InputMessage msg( InputMessage::Type::Text );
		//	msg.text.unicode = wParam;
		//	_messages.push( msg );
		//	break;
		//}
		//case WM_MOUSEMOVE:
		//{
		//	InputMessage msg( InputMessage::Type::MouseMove );
		//	msg.mouseMove.x = LOWORD( lParam );
		//	msg.mouseMove.y = HIWORD( lParam );
		//	msg.mouseMove.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseMove.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseMove.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseMove.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseMove.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseMove.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseMove.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	_messages.push( msg );
		//	break;
		//}
		//case WM_LBUTTONDOWN:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonDown );
		//	msg.mouseButton.type = MouseButton::LeftButton;
		//	msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	captureMouse( );
		//	break;
		//}
		//case WM_LBUTTONUP:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonUp );
		//	msg.mouseButton.type = MouseButton::LeftButton;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	releaseMouse( );
		//	break;
		//}
		//case WM_RBUTTONDOWN:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonDown );
		//	msg.mouseButton.type = MouseButton::RightButton;
		//	msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	captureMouse( );
		//	break;
		//}
		//case WM_RBUTTONUP:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonUp );
		//	msg.mouseButton.type = MouseButton::RightButton;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	releaseMouse( );
		//	break;
		//}
		//case WM_MBUTTONDOWN:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonDown );
		//	msg.mouseButton.type = MouseButton::MiddleButton;
		//	msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	captureMouse( );
		//	break;
		//}
		//case WM_MBUTTONUP:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonUp );
		//	msg.mouseButton.type = MouseButton::MiddleButton;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	releaseMouse( );
		//	break;
		//}
		//case WM_MOUSEWHEEL:
		//{
		//	POINT screenPos;
		//	screenPos.x = static_cast< LONG >( LOWORD( lParam ) );
		//	screenPos.y = static_cast< LONG >( HIWORD( lParam ) );
		//	ScreenToClient( handle, &screenPos );
		//	InputMessage msg( InputMessage::Type::MouseWheel );
		//	msg.mouseWheel.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseWheel.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseWheel.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseWheel.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseWheel.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseWheel.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseWheel.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	msg.mouseWheel.x = static_cast< uint16_t >( screenPos.x );
		//	msg.mouseWheel.y = static_cast< uint16_t >( screenPos.y );
		//	msg.mouseWheel.delta = HIWORD( wParam ) / 120;
		//	_messages.push( msg );
		//	break;
		//}
		//case WM_XBUTTONDOWN:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonDown );
		//	msg.mouseButton.type = HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
		//	msg.mouseButton.ctrl = ( wParam & MK_CONTROL ) != 0;
		//	msg.mouseButton.lButton = ( wParam & MK_LBUTTON ) != 0;
		//	msg.mouseButton.mButton = ( wParam & MK_MBUTTON ) != 0;
		//	msg.mouseButton.rButton = ( wParam & MK_RBUTTON ) != 0;
		//	msg.mouseButton.shift = ( wParam & MK_SHIFT ) != 0;
		//	msg.mouseButton.xButton1 = ( wParam & MK_XBUTTON1 ) != 0;
		//	msg.mouseButton.xButton2 = ( wParam & MK_XBUTTON2 ) != 0;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	break;
		//}
		//case WM_XBUTTONUP:
		//{
		//	InputMessage msg( InputMessage::Type::MouseButtonUp );
		//	msg.mouseButton.type = HIWORD( wParam ) == XBUTTON1 ? MouseButton::XButton1 : MouseButton::XButton2;
		//	msg.mouseButton.x = LOWORD( lParam );
		//	msg.mouseButton.y = HIWORD( lParam );
		//	_messages.push( msg );
		//	break;
		//}
		//default:
		//	// default window procedure
		//	return DefWindowProc( handle, message, wParam, lParam );
		//}
		return 0;
	}

	void processMessages( WindowHandle windowHandle )
	{
		MSG message;
		while ( PeekMessage( &message, windowHandle, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
}