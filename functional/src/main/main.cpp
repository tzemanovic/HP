///
/// replace WinMain function with main on Windows
///
#include <pch.hpp>
#ifdef HP_PLATFORM_WIN32
extern int main( int argc, char ** argv );

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, INT )
{
	return main( __argc, __argv );
}
#endif

