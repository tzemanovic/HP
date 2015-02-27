#include <pch.hpp>
#include <core/engine.hpp>
#include <window/window.hpp>
#include <adt/maybe.hpp>

namespace hp_fp
{
	EngineMut init( const String&& name )
	{
		return EngineMut{ name, true };

	}
	void run_IO( EngineMut& engine )
	{
		Maybe<WindowMut> window = open_IO( engine, defaultWindowConfig_IO( ) );
		ifThenElse( window, [&engine]( WindowMut& window )
		{
			while ( engine.running )
			{
				processMessages_IO( window.handle );
			}
		}, []
		{
			ERR( "Failed to open a window." );
		} );
	}
	void onClose_IO( EngineMut& engine )
	{
		engine.onClose( engine );
	}
}