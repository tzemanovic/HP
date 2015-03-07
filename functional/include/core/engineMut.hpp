#pragma once
#include <core/worldImm.hpp>
#include <window/gameInputMut.hpp>
// [const][cop-c][cop-a][mov-c][mov-a]
// [  +  ][  0  ][  0  ][  0  ][  0  ]
namespace hp_fp
{
	struct KeyMessage;
	struct MouseButtonMessage;
	struct MouseMoveMessage;
	struct MouseWheelMessage;
	struct TextMessage;
	enum class EngineState : UInt8
	{
		Initialized,
		Running,
		Terminated
	};
	struct EngineMut
	{
		EngineMut( String&& name, EngineState&& state, GameInputMut&& gameInput, WorldImm* world )
			: name( std::move( name ) ), state( std::move( state ) ),
			gameInput( std::move( gameInput ) ), world( world )
		{ }
		EngineMut( const EngineMut& ) = delete;
		EngineMut( EngineMut&& e ) : name( std::move( e.name ) ), state( std::move( e.state ) ),
			gameInput( std::move( e.gameInput ) ), world( std::move( e.world ) )
		{ }
		EngineMut operator=( const EngineMut& ) = delete;
		EngineMut operator=( EngineMut&& e )
		{
			return EngineMut{ std::move( e ) };
		}
		const String name;
		EngineState state;
		GameInputMut gameInput;
		WorldImm* world;
	};
}