#pragma once
#include <functional>
#include <vector>
#include "../../adt/sum.hpp"
#include "../../adt/frp/sf2.hpp"
#include "../../graphics/material.hpp"
#include "../../graphics/model.hpp"
#include "../../graphics/renderer.hpp"
#include "../../math/mat4x4.hpp"
#include "../../math/quat.hpp"
#include "../../window/gameInputMut.hpp"
namespace hp_fp
{
	struct Resources;
	struct ActorResources;
	struct ActorState
	{
		FVec3 pos;
		FVec3 vel;
		FVec3 scl;
		FQuat rot;
	};
	struct ActorState_S
	{
		S<FVec3> pos;
		S<FVec3> vel;
		S<FVec3> scl;
		S<FQuat> rot;
	};
	struct ActorInput
	{
		GameInputMut gameInput;
		ActorState state;
	};
	struct ActorInput_S
	{
		S<GameInputMut> gameInput;
		S<ActorState_S> state;
	};
	struct ActorOutput
	{
		ActorState state;
	};
	struct ActorOutput_S
	{
		S<ActorState_S> state;
	};
	struct ActorModelDef
	{
		ModelDef model;
		MaterialDef material;
	};
	typedef std::function<void( Renderer&, const ActorOutput_S&, const Mat4x4& )> CamRenderSF;
	struct ActorCameraDef;
	typedef CamRenderSF( *InitCamRenderSF )( const ActorCameraDef&, const WindowConfigImm& );
	struct ActorCameraDef
	{
		float nearClipDist;
		float farClipDist;
		InitCamRenderSF render;
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
			ActorModelDef actor;
			ActorCameraDef camera;
		};
		friend ActorTypeDef actorModelDef( ActorModelDef&& m );
		friend ActorTypeDef actorCameraDef( ActorCameraDef&& c );
	};
	struct ActorDef
	{
		ActorTypeDef type;
		ActorState startingState;
		std::function<ActorOutput( const ActorInput& )> sf;
		std::vector<ActorDef> children;
	};
	struct ActorDef_S
	{
		ActorTypeDef type;
		ActorState startingState;
		SF<ActorInput_S, ActorOutput_S> sf;
		std::vector<ActorDef_S> children;
	};
	struct Actor
	{
		S<ActorState_S> state;
		//std::function<ActorOutput( const ActorInput& )> sf;
		SF<ActorInput_S, ActorOutput_S> sf;
		std::function<void( Renderer&, const ActorOutput_S&, const Mat4x4& )> render_IO;
		std::vector<Actor> children;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	ActorTypeDef actorModelDef( ActorModelDef&& m );
	ActorTypeDef actorCameraDef( ActorCameraDef&& c );
	std::function<void( Renderer&, const ActorOutput_S&, const Mat4x4& )>
		initActorRenderFunction_IO( Renderer& renderer, Resources& resources,
		const ActorDef_S& actorDef );
	Mat4x4 trasformMatFromActorState( const ActorState_S& actorState );
	namespace
	{
		std::function<void( Renderer&, const ActorOutput_S&, const Mat4x4& )>  renderActor_IO(
			ActorResources& res );
	}
}