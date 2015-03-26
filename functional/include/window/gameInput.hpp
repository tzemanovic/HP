#pragma once
#include <array>
namespace hp_fp
{
	enum struct Key : UInt8
	{
		Backspace = VK_BACK,
		Tab = VK_TAB,
		Return = VK_RETURN,
		Shift = VK_SHIFT,
		Ctrl = VK_CONTROL,
		Alt = VK_MENU,
		Pause = VK_PAUSE,
		CapsLock = VK_CAPITAL,
		Esc = VK_ESCAPE,
		Space = VK_SPACE,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		End = VK_END,
		Home = VK_HOME,
		ArrowLeft = VK_LEFT,
		ArrowUp = VK_UP,
		ArrowRight = VK_RIGHT,
		ArrowDown = VK_DOWN,
		Print = VK_PRINT,
		PrintScreen = VK_SNAPSHOT,
		Insert = VK_INSERT,
		Delete = VK_DELETE,
		Help = VK_HELP,
		No0 = 0x30,
		No1 = 0x31,
		No2 = 0x32,
		No3 = 0x33,
		No4 = 0x34,
		No5 = 0x35,
		No6 = 0x36,
		No7 = 0x37,
		No8 = 0x38,
		No9 = 0x39,
		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
		LWin = VK_LWIN,
		RWin = VK_RWIN,
		Num0 = VK_NUMPAD0,
		Num1 = VK_NUMPAD1,
		Num2 = VK_NUMPAD2,
		Num3 = VK_NUMPAD3,
		Num4 = VK_NUMPAD4,
		Num5 = VK_NUMPAD5,
		Num6 = VK_NUMPAD6,
		Num7 = VK_NUMPAD7,
		Num8 = VK_NUMPAD8,
		Num9 = VK_NUMPAD9,
		NumMultiply = VK_MULTIPLY,
		NumAdd = VK_ADD,
		NumSeparator = VK_SEPARATOR,
		NumSubstract = VK_SUBTRACT,
		NumDecimal = VK_DECIMAL,
		NumDivide = VK_DIVIDE,
		F1 = VK_F1,
		F2 = VK_F2,
		F3 = VK_F3,
		F4 = VK_F4,
		F5 = VK_F5,
		F6 = VK_F6,
		F7 = VK_F7,
		F8 = VK_F8,
		F9 = VK_F9,
		F10 = VK_F10,
		F11 = VK_F11,
		F12 = VK_F12,
		F13 = VK_F13,
		F14 = VK_F14,
		F15 = VK_F15,
		F16 = VK_F16,
		F17 = VK_F17,
		F18 = VK_F18,
		F19 = VK_F19,
		F20 = VK_F20,
		F21 = VK_F21,
		F22 = VK_F22,
		F23 = VK_F23,
		F24 = VK_F24,
		NumLock = VK_NUMLOCK,
		ScrollLock = VK_SCROLL,
		LShift = VK_LSHIFT,
		RShift = VK_RSHIFT,
		LCtrl = VK_LCONTROL,
		RCtrl = VK_RCONTROL,
		LAlt = VK_LMENU,
		RAlt = VK_RMENU
	};
	enum struct MouseButton : UInt8
	{
		LeftButton = MK_LBUTTON,
		RightButton = MK_RBUTTON,
		Shift = MK_SHIFT,
		Control = MK_CONTROL,
		MiddleButton = MK_MBUTTON,
		XButton1 = MK_XBUTTON1,
		XButton2 = MK_XBUTTON2
	};
	// 255 keys, 7 mouseButtons
	const size_t STATES_SIZE = 255 + 7;
	struct Mouse
	{
		UInt16 x;
		UInt16 y;
		UInt16 delta; // wheel delta
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  +  ][  +  ][  +  ][  +  ]
	struct GameInput
	{
		GameInput( )
		{
			std::fill_n( states.begin( ), STATES_SIZE, false );
		}
		GameInput( const GameInput& gi ) : states( gi.states ), mouse( gi.mouse ),
			text( gi.text )
		{ }
		GameInput( GameInput&& gi ) : states( std::move( gi.states ) ),
			mouse( std::move( gi.mouse ) ), text( std::move( gi.text ) )
		{ }
		GameInput operator = ( const GameInput& gi )
		{
			return GameInput{ gi };
		}
		GameInput operator = ( GameInput&& gi )
		{
			return GameInput{ std::move( gi ) };
		}
		bool& operator[]( size_t i )
		{
			return states[i];
		}
		bool& operator[]( Key k )
		{
			return states[static_cast<size_t>( k )];
		}
		bool& operator[]( MouseButton k )
		{
			return states[255 + static_cast<size_t>( k )];
		}
		bool operator[]( Key k ) const
		{
			return states[static_cast<size_t>( k )];
		}
		bool operator[]( MouseButton k ) const
		{
			return states[255 + static_cast<size_t>( k )];
		}
	private:
		GameInput( std::array<bool, STATES_SIZE> states, Mouse mouse, UInt32 text )
			: states( states ), mouse( mouse ), text( text )
		{ }
		std::array<bool, STATES_SIZE> states;
	public:
		Mouse mouse;
		UInt32 text;
	};
}