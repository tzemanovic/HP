#include <pch.hpp>
#include <core/engine.hpp>
#include <core/timer.hpp>
#include <adt/maybe.hpp>
#include <graphics/renderer.hpp>
#include <window/inputMessage.hpp>
namespace hp_fp
{
	EngineMut init( const String&& name )
	{
		return EngineMut{ name, EngineState::Initialized, nullptr,
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
	void setWorld_IO( EngineMut& engine, WorldImm&& world )
	{
		engine.world = HP_NEW WorldImm( world );
	}
	void run_IO( EngineMut& engine, const WindowConfigImm& windowConfig )
	{
		Maybe<WindowMut> window = open_IO( engine, windowConfig );
		ifThenElse( window, [&engine, &windowConfig]( WindowMut& window )
		{
			Maybe<RendererMut> renderer = init_IO( window.handle, windowConfig );
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