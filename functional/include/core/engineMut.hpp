#pragma once
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
		const String name;
		EngineState state;
		void( *onClose )( EngineMut& engine );
		void( *onKeyDown )( EngineMut& engine, KeyMessage&& msg );
		void( *onKeyUp )( EngineMut& engine, KeyMessage&& msg );
		void( *onMouseButtonDown )( EngineMut& engine, MouseButtonMessage&& msg );
		void( *onMouseButtonUp )( EngineMut& engine, MouseButtonMessage&& msg );
		void( *onMouseMove )( EngineMut& engine, MouseMoveMessage&& msg );
		void( *onMouseWheel )( EngineMut& engine, MouseWheelMessage&& msg );
		void( *onText )( EngineMut& engine, TextMessage&& msg );
	};
}