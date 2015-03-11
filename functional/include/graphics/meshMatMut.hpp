#pragma once
#include <graphics/directx.h>
#include <math/color.hpp>
#include <math/vec2.hpp>
// [const][cop-c][cop-a][mov-c][mov-a]
// [  +  ][  0  ][  0  ][  0  ][  0  ]
namespace hp_fp
{
	struct MeshMatMut
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
}