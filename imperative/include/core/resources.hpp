#pragma once
#include <map>
#include <tuple>
#include "../graphics/model.hpp"
#include "../graphics/material.hpp"
namespace hp_ip
{
	class Renderer;
	class Resources
	{
	public:
		Resources( )
		{ }
		~Resources( );
		Resources( const Resources& ) = delete;
		void operator = ( const Resources& ) = delete;
		Model* getModel( Renderer* pRenderer, const LoadedModelDef& loadedModelDef );
		Model* getModel( Renderer* pRenderer, const BuiltInModelDef& builtInModelDef );
		Material* getMaterial( Renderer* pRenderer, const MaterialDef& materialDef );
	private:
		std::map<LoadedModelDef, Model*> _loadedModels;
		std::map<BuiltInModelDef, Model*> _builtInModels;
		std::map<MaterialDef, Material*> _materials;
		std::map<String, ID3D11ShaderResourceView*> _textures;
	};
}

