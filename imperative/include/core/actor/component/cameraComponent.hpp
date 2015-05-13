#pragma once
#include "iComponent.hpp"
#include "../../../graphics/camera.hpp"
#include "../../../math/frustum.hpp"
namespace hp_ip
{
	class CameraComponent : public iComponent
	{
	public:
		CameraComponent( CameraDef&& cameraDef )
			: _cameraDef( std::forward<CameraDef>( cameraDef ) )
		{ }
		virtual void vInit( Resources& resources, Renderer* pRenderer ) override;
		virtual void vUpdate( const float deltaMs, const GameInput& input ) override;
	protected:
		CameraDef _cameraDef;
		Frustum _frustum;
		Mat4x4 _projection;
		Renderer* _pRenderer;
	};
}

