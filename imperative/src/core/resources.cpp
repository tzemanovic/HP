#include <pch.hpp>
#include "../../include/core/resources.hpp"
namespace hp_ip
{
	Resources::~Resources( )
	{
		for ( auto model : _loadedModels )
		{
			HP_DELETE( model.second );
		}
		for ( auto model : _builtInModels )
		{
			HP_DELETE( model.second );
		}
		for ( auto material : _materials )
		{
			HP_DELETE( material.second );
		}
	};
	Model* Resources::getModel( Renderer* pRenderer, const LoadedModelDef& loadedModelDef )
	{
		if ( _loadedModels.count( loadedModelDef ) == 0 )
		{
			_loadedModels.emplace( loadedModelDef,
				Model::loadModelFromFile( pRenderer, loadedModelDef ) );
		}
		return _loadedModels.at( loadedModelDef );
	}
	Model* Resources::getModel( Renderer* pRenderer, const BuiltInModelDef& builtInModelDef )
	{
		if ( _builtInModels.count( builtInModelDef ) == 0 )
		{
			switch ( std::get<0>( builtInModelDef ) )
			{
			case BuiltInModelType::Box:
			{
				_builtInModels.emplace( builtInModelDef,
					Model::cubeMesh( pRenderer, std::get<1>( builtInModelDef ) ) );
			}
			break;
			default:
				WAR( "Missing built-In model for type " +
					std::to_string( static_cast<UInt8>( std::get<0>( builtInModelDef ) ) ) + "." );
			}
		}
		return _builtInModels.at( builtInModelDef );
	}
	Material* Resources::getMaterial( Renderer* pRenderer, const MaterialDef& materialDef )
	{
		if ( _materials.count( materialDef ) == 0 )
		{
			_materials.emplace( materialDef, Material::loadMaterial( pRenderer, materialDef ) );
		}
		return _materials.at( materialDef );
	}
}