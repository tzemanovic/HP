#include <pch.hpp>
#include <core/engine.hpp>
#include <window/window.hpp>

namespace hp_fp
{
	EngineMut init( const String&& name )
	{
		EngineMut engine{ true };
		// TODO: pass engine into window for windowProc
		IO<Maybe<WindowMut>> window = open( std::move( name ), defaultWindowConfig( ) );
		return engine;
	}
}