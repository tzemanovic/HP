#pragma once
// [const][cop-c][cop-a][mov-c][mov-a]
// [  +  ][  0  ][  0  ][  +  ][  +  ]
namespace hp_fp
{
	struct WindowMut
	{
		WindowMut( WindowHandle handle, const LPCWSTR name ) : handle( handle ), name( name )
		{ }
		WindowMut( const WindowMut& ) = delete;
		WindowMut( WindowMut&& w ) : handle( std::move( w.handle ) ), name( std::move( w.name ) )
		{ }
		WindowMut operator=( const WindowMut& ) = delete;
		WindowMut operator=( WindowMut&& w )
		{
			return WindowMut{ std::move( w ) };
		}
		WindowHandle	handle;
		const LPCWSTR	name;
	};
}