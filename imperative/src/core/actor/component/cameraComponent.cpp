#include <pch.hpp>
#include "../../../../include/core/actor/component/cameraComponent.hpp"
#include "../../../../include/core/actor/actor.hpp"
#include "../../../../include/graphics/renderer.hpp"
namespace hp_ip
{
	void CameraComponent::vInit( Resources& resources, Renderer* pRenderer )
	{
		_pRenderer = pRenderer;
		_frustum = init( static_cast<float>( PI ) / 4.f,
			static_cast<float>( pRenderer->windowConfig( ).width ) /
			pRenderer->windowConfig( ).height,
			_cameraDef.nearClipDist, _cameraDef.farClipDist );
		_projection = matrixPerspectiveFovLH( _frustum.fieldOfView,
			_frustum.aspectRatio, _frustum.nearClipDist, _frustum.farClipDist );
	}
	void CameraComponent::vUpdate( const float deltaMs, const GameInput& input )
	{
		_pRenderer->setCamera( { _projection, _owner->transformComponent( ).modelTransform( ) } );
	}
}

