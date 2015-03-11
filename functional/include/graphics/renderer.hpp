#pragma once
#include <adt/maybe.hpp>
#include <graphics/rendererMut.hpp>
#include <window/windowConfigImm.hpp>
namespace hp_fp
{
	Maybe<RendererMut> init_IO( WindowHandle windowHandle, const WindowConfigImm& windowConfig );
	void preRender_IO( RendererMut& renderer );
	void present_IO( RendererMut& renderer );
}