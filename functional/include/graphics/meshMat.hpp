#pragma once
#include "directx.h"
#include "renderer.hpp"
#include "../math/mat4x4.hpp"
#include "../math/color.hpp"
#include "../math/vec2.hpp"
namespace hp_fp
{
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  0  ][  0  ]
	struct MeshMat
	{
		String filename;
		String techniqueName;
		// effect variables
		ID3DX11Effect* effect;
		ID3DX11EffectTechnique* technique;
		ID3DX11EffectPass* pass;
		// input layout
		ID3D11InputLayout* inputLayout;
		// technique desc
		D3DX11_TECHNIQUE_DESC techniqueDesc;
		// effect variables(constants)
		ID3DX11EffectMatrixVariable* viewMatrixVariable;
		ID3DX11EffectMatrixVariable* projectionMatrixVariable;
		ID3DX11EffectMatrixVariable* worldMatrixVariable;
		// Textures
		ID3DX11EffectShaderResourceVariable* diffuseTextureVariable;
		ID3DX11EffectShaderResourceVariable* specularTextureVariable;
		ID3DX11EffectShaderResourceVariable* bumpTextureVariable;
		ID3DX11EffectShaderResourceVariable* parallaxTextureVariable;
		ID3DX11EffectShaderResourceVariable* envMapVariable;
		ID3D11ShaderResourceView* diffuseTexture;
		ID3D11ShaderResourceView* specularTexture;
		ID3D11ShaderResourceView* bumpTexture;
		ID3D11ShaderResourceView* parallaxTexture;
		ID3D11ShaderResourceView* envMapTexture;
		FVec2 textureRepeat;
		// Texture switches
		ID3DX11EffectScalarVariable* useDiffuseTextureVariable;
		ID3DX11EffectScalarVariable* useSpecularTextureVariable;
		ID3DX11EffectScalarVariable* useBumpTextureVariable;
		ID3DX11EffectScalarVariable* useParallaxTextureVariable;
		// Light
		ID3DX11EffectVectorVariable* ambientLightColourVariable;
		ID3DX11EffectVectorVariable* diffuseLightColourVariable;
		ID3DX11EffectVectorVariable* specularLightColourVariable;
		// Direction
		ID3DX11EffectVectorVariable* lightDirectionVariable;
		// Material
		ID3DX11EffectVectorVariable* ambientMaterialVariable;
		ID3DX11EffectVectorVariable* diffuseMaterialVariable;
		ID3DX11EffectVectorVariable* specularMaterialVariable;
		ID3DX11EffectScalarVariable* specularPowerVariable;
		ID3DX11EffectVectorVariable* textureRepeatVariable;
		// Camera
		ID3DX11EffectVectorVariable* cameraPositionVariable;
		// Material colours
		Color ambientMaterial;
		Color diffuseMaterial;
		Color specularMaterial;
		float specularPower;
	};
	/*}   }   }   }  }  }  } } } }}}} Functions {{{{ { { {  {  {  {   {   {   {*/
	MeshMat defaultMat( );
	bool init_IO( MeshMat& meshMat, Renderer& renderer );
	bool loadTexture_IO( ID3D11ShaderResourceView** texture, Renderer& renderer,
		const String& filename );
	namespace
	{
		bool loadShader_IO( MeshMat& meshMat, Renderer& renderer );
		bool loadAndCompile_IO( MeshMat& meshMat, Renderer& renderer, const String& shaderModel,
			ID3DBlob** buffer );
		bool createVertexLayout_IO( MeshMat& meshMat, Renderer& renderer );
	}
	void setProjection( MeshMat& meshMat, const Mat4x4& mat );
	void setView( MeshMat& meshMat, const Mat4x4& mat );
	void setWorld( MeshMat& meshMat, const Mat4x4& mat );
	void setAbientLightColor( MeshMat& meshMat, const Color& color );
	void setDiffuseLightColor( MeshMat& meshMat, const Color& color );
	void setSpecularLightColor( MeshMat& meshMat, const Color& color );
	void setLightDirection( MeshMat& meshMat, const FVec3& dir );
	void setCameraPosition( MeshMat& meshMat, const FVec3& dir );
	void setTextureRepeat( MeshMat& meshMat, const FVec2& repeat );
	void setTextures( MeshMat& meshMat );
	void setMaterials( MeshMat& meshMat );
	void bindInputLayout( Renderer& renderer, MeshMat& meshMat );
	UInt32 getPassCount( MeshMat& meshMat );
	void applyPass( Renderer& renderer, MeshMat& meshMat, UInt32 i );
}