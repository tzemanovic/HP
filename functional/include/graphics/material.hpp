#pragma once
#include "directx.hpp"
#include "renderer.hpp"
#include "../math/mat4x4.hpp"
#include "../math/color.hpp"
#include "../math/vec2.hpp"
namespace hp_fp
{
	struct MaterialDef
	{
		const char* diffuseTextureFilename;
		const char* specularTextureFilename;
		const char* bumpTextureFilename;
		const char* parallaxTextureFilename;
		const char* evnMapTextureFilename;
		FVec2 textureRepeat;
		bool operator == ( const MaterialDef& m ) const
		{
			return diffuseTextureFilename == m.diffuseTextureFilename &&
				specularTextureFilename == m.specularTextureFilename &&
				bumpTextureFilename == m.bumpTextureFilename &&
				parallaxTextureFilename == m.parallaxTextureFilename &&
				evnMapTextureFilename == m.evnMapTextureFilename &&
				textureRepeat == m.textureRepeat;
		}
		bool operator < ( const MaterialDef& m ) const
		{
			if ( diffuseTextureFilename == m.diffuseTextureFilename )
			{
				if ( specularTextureFilename == m.specularTextureFilename )
				{
					if ( bumpTextureFilename == m.bumpTextureFilename )
					{
						if ( parallaxTextureFilename == m.parallaxTextureFilename )
						{
							if ( evnMapTextureFilename == m.evnMapTextureFilename )
							{
								return  textureRepeat < m.textureRepeat;
							}
							return evnMapTextureFilename < m.evnMapTextureFilename;
						}
						return parallaxTextureFilename < m.parallaxTextureFilename;
					}
					return bumpTextureFilename < m.bumpTextureFilename;
				}
				return specularTextureFilename < m.specularTextureFilename;
			}
			return diffuseTextureFilename < m.diffuseTextureFilename;
		}
	};
	// [const][cop-c][cop-a][mov-c][mov-a]
	// [  +  ][  0  ][  0  ][  +  ][  +  ]
	struct Material
	{
		Material( const String& filename, const String& techniqueName,
			const FVec2& textureRepeat, const Color& ambientMaterial,
			const Color& diffuseMaterial, const Color& specularMaterial,
			const float specularPower ) : filename( filename ), techniqueName( techniqueName ),
			effect( nullptr ), technique( nullptr ), pass( nullptr ), inputLayout( nullptr ),
			techniqueDesc( ), viewMatrixVariable( nullptr ), projectionMatrixVariable( nullptr ),
			worldMatrixVariable( nullptr ), diffuseTextureVariable( nullptr ),
			specularTextureVariable( nullptr ), bumpTextureVariable( nullptr ),
			parallaxTextureVariable( nullptr ), envMapVariable( nullptr ),
			diffuseTexture( nullptr ), specularTexture( nullptr ), bumpTexture( nullptr ),
			parallaxTexture( nullptr ), envMapTexture( nullptr ), textureRepeat( textureRepeat ),
			useDiffuseTextureVariable( nullptr ), useSpecularTextureVariable( nullptr ),
			useBumpTextureVariable( nullptr ), useParallaxTextureVariable( nullptr ),
			ambientLightColourVariable( nullptr ), diffuseLightColourVariable( nullptr ),
			specularLightColourVariable( nullptr ), lightDirectionVariable( nullptr ),
			ambientMaterialVariable( nullptr ), diffuseMaterialVariable( nullptr ),
			specularMaterialVariable( nullptr ), specularPowerVariable( nullptr ),
			textureRepeatVariable( nullptr ), cameraPositionVariable( nullptr ),
			ambientMaterial( ambientMaterial ), diffuseMaterial( diffuseMaterial ),
			specularMaterial( specularMaterial ), specularPower( specularPower )
		{
			ZeroMemory( &techniqueDesc, sizeof( D3D10_TECHNIQUE_DESC ) );
		}
		Material( const Material& ) = delete;
		Material( Material&&  m ) : filename( std::move( m.filename ) ),
			techniqueName( std::move( m.techniqueName ) ), effect( std::move( m.effect ) ),
			technique( std::move( m.technique ) ), pass( std::move( m.pass ) ),
			inputLayout( std::move( m.inputLayout ) ),
			techniqueDesc( std::move( m.techniqueDesc ) ),
			viewMatrixVariable( std::move( m.viewMatrixVariable ) ),
			projectionMatrixVariable( std::move( m.projectionMatrixVariable ) ),
			worldMatrixVariable( std::move( m.worldMatrixVariable ) ),
			diffuseTextureVariable( std::move( m.diffuseTextureVariable ) ),
			specularTextureVariable( std::move( m.specularTextureVariable ) ),
			bumpTextureVariable( std::move( m.bumpTextureVariable ) ),
			parallaxTextureVariable( std::move( m.parallaxTextureVariable ) ),
			envMapVariable( std::move( m.envMapVariable ) ),
			diffuseTexture( std::move( m.diffuseTexture ) ),
			specularTexture( std::move( m.specularTexture ) ),
			bumpTexture( std::move( m.bumpTexture ) ),
			parallaxTexture( std::move( m.parallaxTexture ) ),
			envMapTexture( std::move( m.envMapTexture ) ),
			textureRepeat( std::move( m.textureRepeat ) ),
			useDiffuseTextureVariable( std::move( m.useDiffuseTextureVariable ) ),
			useSpecularTextureVariable( std::move( m.useSpecularTextureVariable ) ),
			useBumpTextureVariable( std::move( m.useBumpTextureVariable ) ),
			useParallaxTextureVariable( std::move( m.useParallaxTextureVariable ) ),
			ambientLightColourVariable( std::move( m.ambientLightColourVariable ) ),
			diffuseLightColourVariable( std::move( m.diffuseLightColourVariable ) ),
			specularLightColourVariable( std::move( m.specularLightColourVariable ) ),
			lightDirectionVariable( std::move( m.lightDirectionVariable ) ),
			ambientMaterialVariable( std::move( m.ambientMaterialVariable ) ),
			diffuseMaterialVariable( std::move( m.diffuseMaterialVariable ) ),
			specularMaterialVariable( std::move( m.specularMaterialVariable ) ),
			specularPowerVariable( std::move( m.specularPowerVariable ) ),
			textureRepeatVariable( std::move( m.textureRepeatVariable ) ),
			cameraPositionVariable( std::move( m.cameraPositionVariable ) ),
			ambientMaterial( std::move( m.ambientMaterial ) ),
			diffuseMaterial( std::move( m.diffuseMaterial ) ),
			specularMaterial( std::move( m.specularMaterial ) ),
			specularPower( std::move( m.specularPower ) )
		{
			m.effect = nullptr;
			m.technique = nullptr;
			m.pass = nullptr;
			m.inputLayout = nullptr;
			m.viewMatrixVariable = nullptr;
			m.projectionMatrixVariable = nullptr;
			m.worldMatrixVariable = nullptr;
			m.diffuseTextureVariable = nullptr;
			m.specularTextureVariable = nullptr;
			m.bumpTextureVariable = nullptr;
			m.parallaxTextureVariable = nullptr;
			m.envMapVariable = nullptr;
			m.diffuseTexture = nullptr;
			m.specularTexture = nullptr;
			m.bumpTexture = nullptr;
			m.parallaxTexture = nullptr;
			m.envMapTexture = nullptr;
			m.useDiffuseTextureVariable = nullptr;
			m.useSpecularTextureVariable = nullptr;
			m.useBumpTextureVariable = nullptr;
			m.useParallaxTextureVariable = nullptr;
			m.ambientLightColourVariable = nullptr;
			m.diffuseLightColourVariable = nullptr;
			m.specularLightColourVariable = nullptr;
			m.lightDirectionVariable = nullptr;
			m.ambientMaterialVariable = nullptr;
			m.diffuseMaterialVariable = nullptr;
			m.specularMaterialVariable = nullptr;
			m.specularPowerVariable = nullptr;
			m.textureRepeatVariable = nullptr;
			m.cameraPositionVariable = nullptr;
		}
		Material operator = ( const Material& ) = delete;
		Material operator = ( Material&& m )
		{
			return Material( std::move( m ) );
		}
		~Material( )
		{
			HP_RELEASE( envMapTexture );
			HP_RELEASE( parallaxTexture );
			HP_RELEASE( bumpTexture );
			HP_RELEASE( specularTexture );
			HP_RELEASE( diffuseTexture );
			HP_RELEASE( inputLayout );
			HP_RELEASE( effect );
		}
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

	Material defaultMat( );
	Maybe<Material> loadMaterial_IO( Renderer& renderer, const MaterialDef& materialDef );
	bool loadTexture_IO( ID3D11ShaderResourceView** texture, Renderer& renderer,
		const String& filename );
	namespace
	{
		bool initMaterial_IO( Material& material, Renderer& renderer );
		bool loadShader_IO( Material& material, Renderer& renderer );
		bool loadAndCompile_IO( Material& material, Renderer& renderer, const String& shaderModel,
			ID3DBlob** buffer );
		bool createVertexLayout_IO( Material& material, Renderer& renderer );
	}
	void setProjection_IO( Material& material, const Mat4x4& mat );
	void setView_IO( Material& material, const Mat4x4& mat );
	void setWorld_IO( Material& material, const Mat4x4& mat );
	void setAbientLightColor_IO( Material& material, const Color& color );
	void setDiffuseLightColor_IO( Material& material, const Color& color );
	void setSpecularLightColor_IO( Material& material, const Color& color );
	void setLightDirection_IO( Material& material, const FVec3& dir );
	void setCameraPosition_IO( Material& material, const FVec3& dir );
	void setTextureRepeat_IO( Material& material, const FVec2& repeat );
	void setTextures_IO( Material& material );
	void setMaterials_IO( Material& material );
	void bindInputLayout_IO( Renderer& renderer, Material& material );
	UInt32 getPassCount( Material& material );
	void applyPass_IO( Renderer& renderer, Material& material, UInt32 i );
}