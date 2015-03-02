#pragma once
#include <core/engineMut.hpp>
namespace hp_fp
{
	EngineMut init( const String&& name );
	void run_IO( EngineMut& engine );
}