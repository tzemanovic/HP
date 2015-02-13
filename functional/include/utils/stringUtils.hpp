#pragma once

namespace hp_fp
{
#if defined( HP_PLATFORM_WIN32 )
	// converts multibyte string to unicode widechar string
	std::wstring s2ws( const std::string& s );

	// returns base path from a file path
	std::string basePath( const std::string& path );
#endif
}