#include <pch.hpp>
#include <core/engine.hpp>
#include <window/window.hpp>
#include <adt/maybe.hpp>
#include <window/inputMessage.hpp>
namespace hp_fp
{
	EngineMut init( const String&& name )
	{
		return EngineMut{ name, true,
			[]( EngineMut& engine ) // onClose
		{
			engine.running = false;
		}, []( EngineMut& engine, KeyMessage&& msg ) // onKeyDown
		{

		}, []( EngineMut& engine, KeyMessage&& msg ) // onKeyUp
		{

		}, []( EngineMut& engine, MouseButtonMessage&& msg ) // onMouseButtonDown
		{

		}, []( EngineMut& engine, MouseButtonMessage&& msg ) // onMouseButtonUp
		{

		}, []( EngineMut& engine, MouseMoveMessage&& msg ) // onMouseMoveDown
		{

		}, []( EngineMut& engine, MouseWheelMessage&& msg ) // onMouseWheelDown
		{

		}, []( EngineMut& engine, TextMessage&& msg ) // onText
		{

		} };
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
}