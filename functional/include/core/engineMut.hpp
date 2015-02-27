#pragma once

namespace hp_fp
{
	struct EngineMut
	{
		const String name;
		bool running;
		void( *onClose )( EngineMut& engine );
	};
}