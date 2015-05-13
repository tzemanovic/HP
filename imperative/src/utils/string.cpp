#include <pch.hpp>
#include "../../include/utils/string.hpp"
namespace hp_ip
{
	std::wstring s2ws( const std::string& s )
	{
		int len;
		int slength = (int) s.length( ) + 1;
		len = MultiByteToWideChar( CP_ACP, 0, s.c_str( ), slength, 0, 0 );
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar( CP_ACP, 0, s.c_str( ), slength, buf, len );
		std::wstring r( buf );
		delete[] buf;
		return r;
	}
	std::string basePath( const std::string& path )
	{
		size_t pos = path.find_last_of( "\\/" );
		return ( std::string::npos == pos ) ? "" : path.substr( 0, pos + 1 );
	}
}

