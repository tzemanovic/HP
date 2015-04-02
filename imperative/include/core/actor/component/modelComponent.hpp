#pragma once
#include "iComponent.hpp"
#include "../../../graphics/material.hpp"
#include "../../../graphics/model.hpp"
namespace hp_ip
{
	class ModelComponent : public iComponent
	{
	public:
		ModelComponent( String&& filename, const float scale, MaterialDef& materialDef )
			: _model( nullptr ), _type( ModelType::Loaded ),
			_loadedModelDef( std::make_tuple( std::forward<String>( filename ), scale ) ),
			_materialDef( std::forward<MaterialDef>( materialDef ) )
		{ }
		ModelComponent( const BuiltInModelType modelType, const FVec3& dimensions,
			MaterialDef&& materialDef ) : _model( nullptr ), _type( ModelType::BuiltIn ),
			_builtInModelDef( std::make_tuple( modelType, dimensions ) ),
			_materialDef( std::forward<MaterialDef>( materialDef ) )
		{ }
		~ModelComponent()
		{
			_material = nullptr;
			_model = nullptr;
		}
		virtual void vInit( Resources& resources, Renderer* pRenderer ) override;
		virtual void vUpdate( const float deltaMs )
		{ }
		virtual void vRender( Renderer* pRenderer ) override;
	private:
		Model* _model;
		Material* _material;
		ModelType _type;
		LoadedModelDef _loadedModelDef;
		BuiltInModelDef _builtInModelDef;
		MaterialDef _materialDef;
	};
}