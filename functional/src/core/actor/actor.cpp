#include <pch.hpp>
#include "../../include/core/actor/actor.hpp"
#include "../../include/core/resources.hpp"
namespace hp_fp
{
	ActorTypeDef actorModelDef( ActorModelDef&& m )
	{
		ActorTypeDef def;
		def.model = m;
		def._typeId = typeId<ActorModelDef>( );
		return def;
	}
	ActorTypeDef actorCameraDef( ActorCameraDef&& c )
	{
		ActorTypeDef def;
		def.camera = c;
		def._typeId = typeId<ActorCameraDef>( );
		return def;
	}
	std::function<void( Renderer&, const ActorState&, const Mat4x4& )>
		initActorRenderFunction_IO( Renderer& renderer, Resources& resources,
		const ActorDef& actorDef )
	{
		static std::function<void( Renderer&, const ActorState&, const Mat4x4& )> doNothing =
			[]( Renderer&, const ActorState&, const Mat4x4& )
		{ };
		if ( actorDef.type.is<ActorModelDef>( ) )
		{
			Maybe<ActorResources> res = getActorResources_IO( renderer, resources,
				actorDef.type.model );
			return ifThenElse( res, []( ActorResources& res )
			{
				return renderActor_IO( res );
			}, []
			{
				return doNothing;
			} );
		}
		else if ( actorDef.type.is<ActorCameraDef>( ) )
		{
			return actorDef.type.camera.render( actorDef.type.camera, renderer.windowConfig );
		}
		return doNothing;
	}
	Mat4x4 trasformMatFromActorState( const ActorState& actorState )
	{
		return rotSclPosToMat4x4( actorState.rot, actorState.scl, actorState.pos );
	}
	Mat4x4 modelTrasformMatFromActorState( const ActorState& actorState )
	{
		return rotSclPosToMat4x4( actorState.modelRot * actorState.rot,
			actorState.scl, actorState.pos );
	}
	namespace
	{
		// Have to specify lambda's return type to std::function because of the issue
		// with return type deduction (http://stackoverflow.com/questions/12639578/c11-lambda-returning-lambda)
		std::function<void( Renderer&, const ActorState&, const Mat4x4& )>  renderActor_IO(
			ActorResources& res )
		{
			return [res]( Renderer& renderer, const ActorState& actorState,
				const Mat4x4& transform ) mutable
			{
				const Camera& cam = getCamera( renderer.cameraBuffer );
				setProjection_IO( res.material, cam.projection );
				setView_IO( res.material, inverse( cam.transform ) );
				setWorld_IO( res.material,
					modelTrasformMatFromActorState( actorState ) * transform );
				setCameraPosition_IO( res.material, pos( cam.transform ) );
				setAbientLightColor_IO( res.material, Color( 0.1f, 0.1f, 0.1f, 0.6f ) );
				setDiffuseLightColor_IO( res.material, Color( 1.0f, 0.95f, 0.4f, 0.4f ) );
				setSpecularLightColor_IO( res.material, Color( 1.0f, 1.0f, 1.0f, 0.3f ) );
				setLightDirection_IO( res.material, FVec3{ -0.5f, -1.0f, 0.1f } );
				setTextures_IO( res.material );
				setMaterials_IO( res.material );
				bindInputLayout_IO( renderer, res.material );
				for ( UInt32 i = 0; i < getPassCount( res.material ); ++i )
				{
					applyPass_IO( renderer, res.material, i );
					for ( UInt32 j = 0; j < res.model.meshes.size( ); ++j )
					{
						setBuffers_IO( renderer, res.model.meshes[j] );
						drawIndexed_IO( renderer, res.model.meshes[j].indices.size( ), 0, 0 );
					}
				}
			};
		};
	}
}

