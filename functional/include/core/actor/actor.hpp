#pragma once
#include <functional>
#include <vector>
#include "../adt/sum.hpp"
#include "../../graphics/mesh.hpp"
#include "../../graphics/meshMat.hpp"
#include "../../graphics/renderer.hpp"
#include "../../math/mat4x4.hpp"
#include "../../window/gameInputMut.hpp"
namespace hp_fp
{
	struct ActorInput
	{
		GameInputMut gameInput;
		Mat4x4 cameraProjection;
		Mat4x4 cameraView;
		FVec3 cameraWorldPos;
	};
	struct ActorOutput
	{
		Mat4x4 localTransTo;
		Mat4x4 cameraProjection;
		Mat4x4 cameraView;
		FVec3 cameraWorldPos;
	};
	struct ActorsDef
	{
		String modelFilename;
		float modelScale;
		String diffuseTextureFilename;
		String specularTextureFilename;
		String bumpTextureFilename;
		String parallaxTextureFilename;
		String evnMapTextureFilename;
		std::function<ActorOutput( ActorInput& )> sf;
		std::vector<ActorsDef> children;
	};
	struct Actors
	{
		std::function<ActorOutput( ActorInput& )> sf;
		std::function<void( Renderer&, ActorOutput& )> render_IO;
		std::vector<Actors> children;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/
	std::function<void( Renderer&, ActorOutput& )>  renderFunction_IO( Meshes& meshes,
		MeshMat& meshMat );
	std::function<void( Renderer&, ActorOutput& )> initActorRenderFunction_IO(
		Renderer& renderer, const ActorsDef& actorDef );
}