#pragma once

namespace hp_fp
{
	struct WindowMut
	{
		WindowMut( WindowHandle handle, const LPCWSTR name ) : handle( handle ), name( name )
		{
			LOG( "con" );
		}
		WindowMut( const WindowMut& ) = delete;               // Copy constructor
		WindowMut( WindowMut&& w ) : handle( std::move( w.handle ) ), name( std::move( w.name ) )
		{
			LOG( "movc" );
		}                    // Move constructor
		WindowMut operator=( const WindowMut& ) = delete;  // Copy assignment operator
		WindowMut operator=( WindowMut&& w )
		{
			LOG( "mova" );
			return WindowMut{ w.handle, w.name };
		}

		WindowHandle	handle;
		const LPCWSTR	name;
	};
}