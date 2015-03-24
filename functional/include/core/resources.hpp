#pragma once
#include <map>
#include <tuple>
#include "actor/actor.hpp"
#include "../graphics/model.hpp"
#include "../graphics/material.hpp"
namespace hp_fp
{
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  0  ][  0  ]
	struct Resources
	{
		std::map<LoadedModelDef, Maybe<Model>> loadedModels;
		std::map<BuiltInModelDef, Maybe<Model>> builtInModels;
		std::map<MaterialDef, Maybe<Material>> materials;
		std::map<String, ID3D11ShaderResourceView*> textures;
	};
	struct ActorResources
	{
		Model& model;
		Material& material;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/

	Maybe<Model>& getModel_IO( Renderer& renderer, Resources& resources,
		const LoadedModelDef& modelDef );
	Maybe<Model>& getModel_IO( Renderer& renderer, Resources& resources,
		const BuiltInModelDef& modelDef );
	Maybe<Material>& getMaterial_IO( Renderer& renderer, Resources& resources,
		const MaterialDef& materialDef );
	Maybe<ActorResources> getActorResources_IO( Renderer& renderer, Resources& resources,
		const ActorModelDef& actorModelDef );
	namespace
	{
		Maybe<ActorResources> getMaterialForModel_IO( Renderer& renderer, Resources& resources,
			Maybe<Model>& model, const MaterialDef& materialDef );
	}
}