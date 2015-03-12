#pragma once
#include <graphics/meshMatMut.hpp>
namespace hp_fp
{
	struct RendererMut;
	MeshMatMut defaultMat( );
	bool init_IO( MeshMatMut& meshMat, RendererMut& renderer );
	bool loadTexture_IO( ID3D11ShaderResourceView* texture, RendererMut& renderer,
		const String& filename );
	namespace
	{
		bool loadShader_IO( MeshMatMut& meshMat, RendererMut& renderer );
		bool loadAndCompile_IO( MeshMatMut& meshMat, RendererMut& renderer, const String& shaderModel,
			ID3DBlob** buffer );
		bool createVertexLayout_IO( MeshMatMut& meshMat, RendererMut& renderer );
	}
}