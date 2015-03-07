#pragma once
#include <core/engineMut.hpp>
#include <core/worldImm.hpp>
#include <window/window.hpp>
namespace hp_fp
{
	EngineMut init( String&& name );
	void setWorld_IO( EngineMut& engine, WorldImm&& world );
	void run_IO( EngineMut& engine, const WindowConfigImm& windowConfig = defaultWindowConfig_IO( ) );
}