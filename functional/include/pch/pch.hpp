///
/// precompiled header
///
#pragma once

// disable STL expections
#define _HAS_EXCEPTIONS 0


#if defined( _WIN32 ) || defined( __WIN32__ )
// Windows
#   define HP_PLATFORM_WIN32
// exclude rarely-used services from Windows headers
#   ifndef WIN32_LEAN_AND_MEAN
#       define WIN32_LEAN_AND_MEAN
#   endif
#   include <Windows.h>
#else
#   error This operation system is not supported
#endif


// redefine new for debugging purposes
#if defined( _DEBUG ) || defined( DEBUG )
#	define HP_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#   define HP_DEBUG
#	include <iostream>
#else
#	define HP_NEW new
#endif


// safe delete pointer
#ifndef HP_DELETE
#   define HP_DELETE( x ) delete x; x = nullptr;
#endif
// safe delete array
#ifndef HP_DELETE_ARRAY
#   define HP_DELETE_ARRAY( x ) delete [] x; x = nullptr;
#endif
// safe release
#ifndef HP_RELEASE
#   define HP_RELEASE( x ) if( x ) x->Release( ); x = nullptr;
#endif


#include <string>


#if defined( HP_PLATFORM_WIN32 )
typedef HWND		 WindowHandle;
typedef std::string	 String;
#endif


namespace hp_fp
{
	extern const double PI;
	extern const double TWO_PI;
}