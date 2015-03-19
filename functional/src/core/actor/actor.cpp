#include <pch.hpp>
#include "../../include/core/actor/actor.hpp"
namespace hp_fp
{
	// Should be function that takes renderer and returns rendering function.
	// Have to specify lambda's return type to std::function because of the issue
	// with return type deduction (http://stackoverflow.com/questions/12639578/c11-lambda-returning-lambda)
	std::function<void( Renderer&, ActorOutput& )>  renderFunction_IO( Meshes& meshes,
		MeshMat& meshMat )
	{
		// TODO: fix the copy construction deallocation
		return [=]( Renderer& renderer, ActorOutput& output ) mutable
		{
			//std::cout << output.x - 96.0f << std::endl;
			//TODO: these are all IO
			setProjection( meshMat, output.cameraProjection );
			setView( meshMat, output.cameraView );
			setWorld( meshMat, output.localTransTo );
			setCameraPosition( meshMat, output.cameraWorldPos );
			setAbientLightColor( meshMat, Color( 0.1f, 0.1f, 0.1f, 0.6f ) );
			setDiffuseLightColor( meshMat, Color( 1.0f, 0.95f, 0.4f, 0.4f ) );
			setSpecularLightColor( meshMat, Color( 1.0f, 1.0f, 1.0f, 0.3f ) );
			setLightDirection( meshMat, FVec3( -0.5f, -1.0f, 0.1f ) );
			setTextures( meshMat );
			setMaterials( meshMat );
			bindInputLayout( renderer, meshMat );
			for ( UInt32 i = 0; i < getPassCount( meshMat ); ++i )
			{
				applyPass( renderer, meshMat, i );
				for ( UInt32 j = 0; j < meshes.meshes.size( ); ++j )
				{
					setBuffers_IO( renderer, meshes.meshes[j] );
					drawIndexed_IO( renderer, meshes.meshes[j].indices.size( ), 0, 0 );
				}
			}
		};
	};
	std::function<void( Renderer&, ActorOutput& )> initActorRenderFunction_IO( Renderer& renderer, const ActorsDef& actorDef )
	{
		static std::function<void( Renderer&, ActorOutput& )> doNothing = []( Renderer& renderer,
			ActorOutput& output )
		{ };
		Maybe<Meshes> meshes = loadModelFromFile_IO( actorDef.modelFilename,
			actorDef.modelScale );
		return ifThenElse( meshes, [&renderer, &actorDef]( Meshes& meshes )
		{
			bool buffersCreated = true;
			for ( auto& mesh : meshes.meshes )
			{
				buffersCreated &= createBuffers_IO( renderer, mesh );
			}
			if ( buffersCreated )
			{
				MeshMat meshMat = defaultMat( );
				if ( init_IO( meshMat, renderer ) )
				{
					bool texturesLoaded = true;
					if ( actorDef.diffuseTextureFilename != "" )
					{
						if ( !loadTexture_IO( &meshMat.diffuseTexture, renderer,
							actorDef.diffuseTextureFilename ) )
						{
							texturesLoaded = false;
							ERR( "Failed to load \"" + actorDef.diffuseTextureFilename + "\" texture." );
						}
					}
					if ( actorDef.specularTextureFilename != "" )
					{
						if ( !loadTexture_IO( &meshMat.specularTexture, renderer,
							actorDef.specularTextureFilename ) )
						{
							texturesLoaded = false;
							ERR( "Failed to load \"" + actorDef.specularTextureFilename + "\" texture." );
						}
					}
					if ( actorDef.bumpTextureFilename != "" )
					{
						if ( !loadTexture_IO( &meshMat.bumpTexture, renderer,
							actorDef.bumpTextureFilename ) )
						{
							texturesLoaded = false;
							ERR( "Failed to load \"" + actorDef.bumpTextureFilename + "\" texture." );
						}
					}
					if ( actorDef.parallaxTextureFilename != "" )
					{
						if ( !loadTexture_IO( &meshMat.parallaxTexture, renderer,
							actorDef.parallaxTextureFilename ) )
						{
							texturesLoaded = false;
							ERR( "Failed to load \"" + actorDef.parallaxTextureFilename + "\" texture." );
						}
					}
					if ( texturesLoaded )
					{
						return renderFunction_IO( meshes, meshMat );
					}
				}
				else
				{
					ERR( "Failed to initalize \"" + actorDef.modelFilename + "\"'s material." );
				}
			}
			else
			{
				ERR( "Failed to initalize \"" + actorDef.modelFilename + "\"'s buffers." );
			}
			return doNothing;
		}, [&actorDef] // nothing<Meshes>
		{
			ERR( "Failed to load \"" + actorDef.modelFilename + "\" model." );
			return doNothing;
		} );
	}
}