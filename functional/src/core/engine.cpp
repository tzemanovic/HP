#include <pch.hpp>
#include <core/engine.hpp>
#include <window/window.hpp>
#include <adt/io.hpp>
#include <adt/maybe.hpp>

namespace hp_fp
{
	void init( const String&& name )
	{
		EngineMut engine{ true };
		// TODO: pass engine into window for windowProc
		IO<Maybe<WindowMut>> window = open( engine, std::move( name ), defaultWindowConfig( ) );
		ifThenElse<WindowMut, void>( window( ),
			[&engine]( WindowMut& window )
		{
			while ( engine.running )
			{
				processMessages( window.handle );
			}
		},
			[]
		{
			ERR( "Failed to open the window." << std::endl );
		} );
	}
}