#pragma once
#include <graphics/meshMatMut.hpp>
#include <graphics/rendererMut.hpp>
#include <math/mat4x4.hpp>
namespace hp_fp
{
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
	void setProjection( MeshMatMut& meshMat, const Mat4x4& mat );
	void setView( MeshMatMut& meshMat, const Mat4x4& mat );
	void setWorld( MeshMatMut& meshMat, const Mat4x4& mat );
	void setAbientLightColor( MeshMatMut& meshMat, const Color& color );
	void setDiffuseLightColor( MeshMatMut& meshMat, const Color& color );
	void setSpecularLightColor( MeshMatMut& meshMat, const Color& color );
	void setLightDirection( MeshMatMut& meshMat, const FVec3& dir );
	void setCameraPosition( MeshMatMut& meshMat, const FVec3& dir );
	void setTextureRepeat( MeshMatMut& meshMat, const FVec2& repeat );
	void setTextures( MeshMatMut& meshMat );
	void setMaterials( MeshMatMut& meshMat );
	void bindInputLayout( RendererMut& renderer, MeshMatMut& meshMat );
	UInt32 getPassCount( MeshMatMut& meshMat );
	void applyPass( RendererMut& renderer, MeshMatMut& meshMat, UInt32 i );
}