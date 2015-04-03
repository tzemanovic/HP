#include <pch.hpp>
#include "../../../../include/core/actor/component/transformComponent.hpp"
namespace hp_ip
{
	void TransformComponent::vUpdate( const float deltaMs, const GameInput& input )
	{
		_pos += _vel * deltaMs;
	}
	Mat4x4 TransformComponent::transform( ) const
	{
		return rotSclPosToMat4x4( _rot, _scl, _pos ) * _parentTransform;
	}
	Mat4x4 TransformComponent::modelTransform( ) const
	{
		return rotSclPosToMat4x4( _modelRot * _rot, _scl, _pos ) * _parentTransform;
	}
}