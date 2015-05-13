#pragma once
#include <core/actor/component/transformComponent.hpp>
namespace hp_ip
{
	class BallTransformComponent : public TransformComponent
	{
	public:
		BallTransformComponent( const FVec3& pos, const FVec3& vel, const FVec3& scl,
			const FQuat& rot = FQuat::identity, const FQuat& modelRot = FQuat::identity,
			const Mat4x4& parentTransform = Mat4x4::identity )
			: TransformComponent( pos, vel, scl, rot, modelRot, parentTransform )
		{ }
		virtual void vUpdate( const float deltaMs, const GameInput& input ) override;
	};
}

