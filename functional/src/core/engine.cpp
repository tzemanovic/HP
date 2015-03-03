#include <pch.hpp>
#include <core/engine.hpp>
#include <core/timer.hpp>
#include <adt/maybe.hpp>
#include <graphics/renderer.hpp>
#include <window/inputMessage.hpp>
#include <window/window.hpp>
namespace hp_fp
{
	EngineMut init( const String&& name )
	{
		return EngineMut{ name, EngineState::Initialized,
			[]( EngineMut& engine ) // onClose
		{
			engine.state = EngineState::Terminated;
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
			Maybe<RendererMut> renderer = init_IO( window.handle, defaultWindowConfig_IO( ) );
			ifThenElse( renderer, [&engine, &window]( RendererMut& renderer )
			{
				engine.state = EngineState::Running;
				TimerMut timer = initTimer_IO( );
				while ( engine.state == EngineState::Running )
				{
					processMessages_IO( window.handle );
					updateTimer_IO( timer );
					//render_IO( timer );
					//update_IO( timer );
				}
			}, []
			{
				ERR( "Failed to initialize renderer." );
			} );
		}, []
		{
			ERR( "Failed to open a window." );
		} );
	}
}