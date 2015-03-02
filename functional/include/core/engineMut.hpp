#pragma once
namespace hp_fp
{
	struct KeyMessage;
	struct MouseButtonMessage;
	struct MouseMoveMessage;
	struct MouseWheelMessage;
	struct TextMessage;
	struct EngineMut
	{
		const String name;
		bool running;
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