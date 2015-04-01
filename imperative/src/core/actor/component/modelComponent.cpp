#include <pch.hpp>
#include "../../../../include/core/actor/component/modelComponent.hpp"
#include "../../../../include/core/resources.hpp"
#include "../../../../include/core/actor/actor.hpp"
#include "../../../../include/graphics/renderer.hpp"
namespace hp_ip
{
	void ModelComponent::vInit( Resources& resources, Renderer* pRenderer )
	{
		switch ( _type )
		{
		case ModelType::Loaded:
		{
			_model = resources.getModel( pRenderer, _loadedModelDef );
		}
		break;
		case ModelType::BuiltIn:
		{
			_model = resources.getModel( pRenderer, _builtInModelDef );
		}
		break;
		default:
			WAR( "Invalid model type." );
		}
		_material = resources.getMaterial( pRenderer, _materialDef );
	}
	void ModelComponent::vRender( Renderer* pRenderer )
	{
		if ( _owner != nullptr )
		{
			const Camera& cam = pRenderer->getCamera( );
			_material->setProjection( cam.projection );
			_material->setView( inverse( cam.transform ) );
			_material->setWorld( _owner->transformComponent( ).modelTransform( ) );
			_material->setCameraPosition( pos( cam.transform ) );
			_material->setAbientLightColor( Color( 0.1f, 0.1f, 0.1f, 0.6f ) );
			_material->setDiffuseLightColor( Color( 1.0f, 0.95f, 0.4f, 0.4f ) );
			_material->setSpecularLightColor( Color( 1.0f, 1.0f, 1.0f, 0.3f ) );
			_material->setLightDirection( FVec3{ -0.5f, -1.0f, 0.1f } );
			_material->setTextures( );
			_material->setMaterials( );
			_material->bindInputLayout( pRenderer );
			for ( UInt32 i = 0; i < _material->getPassCount( ); ++i )
			{
				_material->applyPass( pRenderer, i );
				for ( UInt32 j = 0; j < _model->_meshes.size( ); ++j )
				{
					pRenderer->setBuffers( _model->_meshes[j] );
					pRenderer->drawIndexed( _model->_meshes[j].indicesSize( ), 0, 0 );
				}
			}
		}
	}
}