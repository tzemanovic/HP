#pragma once
#include "directx.hpp"
#include "../math/color.hpp"
#include "../math/mat4x4.hpp"
#include "../math/vec2.hpp"
namespace hp_ip
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
	class Renderer;
	class Material
	{
	public:
		static Material* loadMaterial( Renderer* pRenderer, const MaterialDef& materialDef );
		Material( const String& filename, const String& techniqueName,
			const FVec2& textureRepeat, const Color& ambientMaterial,
			const Color& diffuseMaterial, const Color& specularMaterial,
			const float specularPower );
		~Material( );
	private:
		bool init( Renderer* pRenderer );
		bool createVertexLayout( Renderer* pRenderer );
		bool loadDiffuseTexture( Renderer* pRenderer, const String& filename );
		bool loadSpecularTexture( Renderer* pRenderer, const String& filename );
		bool loadBumpTexture( Renderer* pRenderer, const String& filename );
		bool loadParallaxTexture( Renderer* pRenderer, const String& filename );
		bool loadEnvMapTexture( Renderer* pRenderer, const String& filename );
	public:
		void setProjection( const Mat4x4& mat )
		{
			_pProjectionMatrixVariable->SetMatrix( (float*) ( &mat ) );
		}
		void setView( const Mat4x4& mat )
		{
			_pViewMatrixVariable->SetMatrix( (float*) ( &mat ) );
		}
		void setWorld( const Mat4x4& mat )
		{
			_pWorldMatrixVariable->SetMatrix( (float*) ( &mat ) );
		}
		void setAbientLightColor( const Color& color )
		{
			_pAmbientLightColourVariable->SetFloatVector( (float*) ( &color ) );
		}
		void setDiffuseLightColor( const Color& color )
		{
			_pDiffuseLightColourVariable->SetFloatVector( (float*) ( &color ) );
		}
		void setSpecularLightColor( const Color& color )
		{
			_pSpecularLightColourVariable->SetFloatVector( (float*) ( &color ) );
		}
		void setLightDirection( const FVec3& dir )
		{
			_pLightDirectionVariable->SetFloatVector( (float*) ( &dir ) );
		}
		void setCameraPosition( const FVec3& dir )
		{
			_pCameraPositionVariable->SetFloatVector( (float*) ( &dir ) );
		}
		void setTextureRepeat( const FVec2& repeat )
		{
			_textureRepeat = repeat;
		}
		void setTextures( )
		{
			_pDiffuseTextureVariable->SetResource( _pDiffuseTexture );
			_pSpecularTextureVariable->SetResource( _pSpecularTexture );
			_pBumpTextureVariable->SetResource( _pBumpTexture );
			_pParallaxTextureVariable->SetResource( _pParallaxTexture );
			_pEnvMapVariable->SetResource( _pEnvMapTexture );
			if ( _pDiffuseTexture )
				_pUseDiffuseTextureVariable->SetBool( true );
			if ( _pSpecularTexture )
				_pUseSpecularTextureVariable->SetBool( true );
			if ( _pBumpTexture )
				_pUseBumpTextureVariable->SetBool( true );
			if ( _pParallaxTexture )
				_pUseParallaxTextureVariable->SetBool( true );
			_pTextureRepeatVariable->SetFloatVector( (float*) ( &_textureRepeat ) );
		}
		void setMaterials( )
		{
			_pAmbientMaterialVariable->SetFloatVector( (float*) ( _ambientMaterial ) );
			_pDiffuseMaterialVariable->SetFloatVector( (float*) ( _diffuseMaterial ) );
			_pSpecularMaterialVariable->SetFloatVector( (float*) ( _specularMaterial ) );
			_pSpecularPowerVariable->SetFloat( _specularPower );
		}
		void bindInputLayout( Renderer* pRenderer );
		UInt32 getPassCount( )
		{
			return _techniqueDesc.Passes;
		}
		void applyPass( Renderer* pRenderer, UInt32 i );
	private:
		bool loadEffectFromFile( Renderer* pRenderer );
		bool compileD3DShader( const String& filePath, const String& shaderModel,
			ID3DBlob** ppBuffer );
		bool loadTexture( ID3D11ShaderResourceView** texture, Renderer* pRenderer,
			const String& filename );
		String _filename;
		String _techniqueName;
		// effect variables
		ID3DX11Effect* _pEffect;
		ID3DX11EffectTechnique* _pTechnique;
		ID3DX11EffectPass* _pPass;
		// input layout
		ID3D11InputLayout* _pInputLayout;
		// technique desc
		D3DX11_TECHNIQUE_DESC _techniqueDesc;
		// effect variables(constants)
		ID3DX11EffectMatrixVariable* _pViewMatrixVariable;
		ID3DX11EffectMatrixVariable* _pProjectionMatrixVariable;
		ID3DX11EffectMatrixVariable* _pWorldMatrixVariable;
		// Textures
		ID3DX11EffectShaderResourceVariable* _pDiffuseTextureVariable;
		ID3DX11EffectShaderResourceVariable* _pSpecularTextureVariable;
		ID3DX11EffectShaderResourceVariable* _pBumpTextureVariable;
		ID3DX11EffectShaderResourceVariable* _pParallaxTextureVariable;
		ID3DX11EffectShaderResourceVariable* _pEnvMapVariable;
		ID3D11ShaderResourceView* _pDiffuseTexture;
		ID3D11ShaderResourceView* _pSpecularTexture;
		ID3D11ShaderResourceView* _pBumpTexture;
		ID3D11ShaderResourceView* _pParallaxTexture;
		ID3D11ShaderResourceView* _pEnvMapTexture;
		FVec2 _textureRepeat;
		// Texture switches
		ID3DX11EffectScalarVariable* _pUseDiffuseTextureVariable;
		ID3DX11EffectScalarVariable* _pUseSpecularTextureVariable;
		ID3DX11EffectScalarVariable* _pUseBumpTextureVariable;
		ID3DX11EffectScalarVariable* _pUseParallaxTextureVariable;
		// Light
		ID3DX11EffectVectorVariable* _pAmbientLightColourVariable;
		ID3DX11EffectVectorVariable* _pDiffuseLightColourVariable;
		ID3DX11EffectVectorVariable* _pSpecularLightColourVariable;
		// Direction
		ID3DX11EffectVectorVariable* _pLightDirectionVariable;
		// Material
		ID3DX11EffectVectorVariable* _pAmbientMaterialVariable;
		ID3DX11EffectVectorVariable* _pDiffuseMaterialVariable;
		ID3DX11EffectVectorVariable* _pSpecularMaterialVariable;
		ID3DX11EffectScalarVariable* _pSpecularPowerVariable;
		ID3DX11EffectVectorVariable* _pTextureRepeatVariable;
		// Camera
		ID3DX11EffectVectorVariable* _pCameraPositionVariable;
		// Material colours
		Color _ambientMaterial;
		Color _diffuseMaterial;
		Color _specularMaterial;
		float _specularPower;
	};
}