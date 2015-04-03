#pragma once
#include "gameInput.hpp"
#include "../utils/string.hpp"
namespace hp_ip
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
	class Window
	{
	public:
		Window( const String& name, const WindowConfig& windowConfig ) : _handle( nullptr ),
			_name( s2ws( name ) ), _windowConfig( windowConfig ), _open( false )
		{ }
		bool open( );
		void switchToFullscreen( );
		void processMessages( );
		bool isOpen( );
		void captureMouse( );
		void releaseMouse( );
		LRESULT CALLBACK windowProc( HWND handle, UINT message, WPARAM wParam, LPARAM lParam );
		static LRESULT CALLBACK staticWindowProc( HWND handle, UINT message, WPARAM wParam,
			LPARAM lParam );
		static WindowConfig defaultWindowConfig( );
		static void setWindowVisibility( WindowHandle handle, const bool visible );
	private:
		bool isOnlyInstance( );
	private:
		WindowHandle _handle;
		std::wstring _name;
		WindowConfig _windowConfig;
		GameInput _gameInput;
		bool _open;
	public:
		WindowHandle handle( ) const
		{
			return _handle;
		}
		GameInput gameInput( ) const
		{
			return _gameInput;
		}
	};
}