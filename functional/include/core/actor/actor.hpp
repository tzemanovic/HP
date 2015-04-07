#pragma once
#include <functional>
#include <vector>
#include "../../adt/frp/sf.hpp"
#include "../../graphics/material.hpp"
#include "../../graphics/model.hpp"
#include "../../graphics/renderer.hpp"
#include "../../math/mat4x4.hpp"
#include "../../math/quat.hpp"
#include "../../window/gameInput.hpp"
namespace hp_fp
{
	struct Resources;
	struct ActorResources;
	struct ActorStartingState
	{
		FVec3 pos;
		FVec3 vel;
		FVec3 scl;
		FQuat rot;
		FQuat modelRot;
	};
	struct ActorState
	{
		FVec3 pos;
		FVec3 vel;
		FVec3 scl;
		FQuat rot;
		FQuat modelRot;
	};
	struct ActorInput
	{
		GameInput gameInput;
		ActorState state;
	};
	struct ActorOutput
	{
		ActorState state;
	};
	struct ActorModelDef
	{
		ModelDef model;
		MaterialDef material;
	};
	typedef std::function<void( Renderer&, const ActorState&, const Mat4x4& )> CamRenderFn;
	struct ActorCameraDef;
	typedef CamRenderFn( *InitCamRenderFn )( const ActorCameraDef&, const WindowConfig& );
	struct ActorCameraDef
	{
		float nearClipDist;
		float farClipDist;
		InitCamRenderFn render;
	};
	struct ActorTypeDef
	{
		template<typename A>
		bool is( ) const
		{
			return ( _typeId == typeId<A>( ) );
		}
	private:
		TypeId _typeId;
	public:
		union
		{
			ActorModelDef model;
			ActorCameraDef camera;
		};
		friend ActorTypeDef actorModelDef( ActorModelDef&& m );
		friend ActorTypeDef actorCameraDef( ActorCameraDef&& c );
	};
	struct ActorDef
	{
		ActorTypeDef type;
		ActorStartingState startingState;
		SF<ActorInput, ActorOutput> sf;
		std::vector<ActorDef> children;
	};
	struct Actor
	{
		ActorState state;
		SF<ActorInput, ActorOutput> sf;
		std::function<void( Renderer&, const ActorState&, const Mat4x4& )> render_IO;
		std::vector<Actor> children;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	ActorTypeDef actorModelDef( ActorModelDef&& m );
	ActorTypeDef actorCameraDef( ActorCameraDef&& c );
	std::function<void( Renderer&, const ActorState&, const Mat4x4& )>
		initActorRenderFunction_IO( Renderer& renderer, Resources& resources,
		const ActorDef& actorDef );
	Mat4x4 trasformMatFromActorState( const ActorState& actorState );
	Mat4x4 modelTrasformMatFromActorState( const ActorState& actorState );
	namespace
	{
		std::function<void( Renderer&, const ActorState&, const Mat4x4& )>  renderActor_IO(
			ActorResources& res );
	}
}