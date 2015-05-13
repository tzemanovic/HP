#pragma once
#include "iComponent.hpp"
#include "../../../math/mat4x4.hpp"
#include "../../../math/quat.hpp"
namespace hp_ip
{
	class TransformComponent : public iComponent
	{
	public:
		TransformComponent( const FVec3& pos, const FVec3& vel, const FVec3& scl,
			const FQuat& rot = FQuat::identity, const FQuat& modelRot = FQuat::identity,
			const Mat4x4& parentTransform = Mat4x4::identity )
			: iComponent( ComponentType::Transform ), _pos( pos ), _vel( vel ),
			_scl( scl ), _rot( rot ), _modelRot( modelRot ),
			_parentTransform( parentTransform )
		{ }
		TransformComponent( TransformComponent&& transform ) : TransformComponent(
			std::move( transform._pos ), std::move( transform._vel ),
			std::move( transform._scl ), std::move( transform._rot ),
			std::move( transform._modelRot ), std::move( transform._parentTransform ) )
		{ }
		TransformComponent operator = ( TransformComponent&& transform )
		{
			return TransformComponent( std::forward<TransformComponent>( transform ) );
		}
		virtual void vUpdate( const float deltaMs, const GameInput& input );
		Mat4x4 transform( ) const;
		Mat4x4 modelTransform( ) const;
	protected:
		FVec3 _pos;
		FVec3 _vel;
		FVec3 _scl;
		FQuat _rot;
		FQuat _modelRot;
		Mat4x4 _parentTransform;
	public:
		FVec3 pos( ) const
		{
			return _pos;
		}
		FVec3 vel( ) const
		{
			return _vel;
		}
		FVec3 scl( ) const
		{
			return _scl;
		}
		FQuat rot( ) const
		{
			return _rot;
		}
		FQuat modelRot( ) const
		{
			return _modelRot;
		}
		void setPos( const FVec3& pos )
		{
			_pos = pos;
		}
		void setVel( const FVec3 vel )
		{
			_vel = vel;
		}
		void setScl( const FVec3 scl )
		{
			_scl = scl;
		}
		void setRot( const FQuat rot )
		{
			_rot = rot;
		}
		void setModelRot( const FQuat modelRot )
		{
			_modelRot = modelRot;
		}
		void setParentTransform( const Mat4x4& transform )
		{
			_parentTransform = transform;
		}
	};
}

