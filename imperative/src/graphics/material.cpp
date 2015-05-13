#include <pch.hpp>
#include "../../include/graphics/material.hpp"
#include "../../include/graphics/renderer.hpp"
#include "../../include/graphics/vertex.hpp"
namespace hp_ip
{
	Material* Material::loadMaterial( Renderer* pRenderer, const MaterialDef& materialDef )
	{
		Material* material = HP_NEW Material
		{
			"assets/shaders/parallax.fx", // filename
			"Render", // techniqueName
			{ 1.0f, 1.0f }, // textureRepeat
			{ 0.5f, 0.5f, 0.5f }, // ambientMaterial
			{ 0.8f, 0.8f, 0.8f }, // diffuseMaterial
			{ 1.0f, 1.0f, 1.0f }, // specularMaterial
			25.0f // specularPower
		};
		bool materialLoaded;
		if ( materialLoaded = material->init( pRenderer ) )
		{
			// TODO: fix these in /Ox
			if ( materialDef.diffuseTextureFilename != "" )
			{
				materialLoaded &= material->loadDiffuseTexture( pRenderer,
					materialDef.diffuseTextureFilename );
			}
			if ( materialDef.specularTextureFilename != "" )
			{
				materialLoaded &= material->loadSpecularTexture( pRenderer,
					materialDef.specularTextureFilename );
			}
			if ( materialDef.bumpTextureFilename != "" )
			{
				materialLoaded &= material->loadBumpTexture( pRenderer,
					materialDef.bumpTextureFilename );
			}
			if ( materialDef.parallaxTextureFilename != "" )
			{
				materialLoaded &= material->loadParallaxTexture( pRenderer,
					materialDef.parallaxTextureFilename );
			}
			if ( materialDef.textureRepeat.x != 0.0f && materialDef.textureRepeat.y != 0.0f )
			{
				material->_textureRepeat = materialDef.textureRepeat;
			}
		}
		if ( materialLoaded )
		{
			return material;
		}
		HP_DELETE( material );
		return nullptr;
	}
	Material::Material( const String& filename, const String& techniqueName,
		const FVec2& textureRepeat, const Color& ambientMaterial,
		const Color& diffuseMaterial, const Color& specularMaterial,
		const float specularPower ) : _filename( filename ), _techniqueName( techniqueName ),
		_pEffect( nullptr ), _pTechnique( nullptr ), _pPass( nullptr ), _pInputLayout( nullptr ),
		_techniqueDesc( ), _pViewMatrixVariable( nullptr ), _pProjectionMatrixVariable( nullptr ),
		_pWorldMatrixVariable( nullptr ), _pDiffuseTextureVariable( nullptr ),
		_pSpecularTextureVariable( nullptr ), _pBumpTextureVariable( nullptr ),
		_pParallaxTextureVariable( nullptr ), _pEnvMapVariable( nullptr ),
		_pDiffuseTexture( nullptr ), _pSpecularTexture( nullptr ), _pBumpTexture( nullptr ),
		_pParallaxTexture( nullptr ), _pEnvMapTexture( nullptr ), _textureRepeat( textureRepeat ),
		_pUseDiffuseTextureVariable( nullptr ), _pUseSpecularTextureVariable( nullptr ),
		_pUseBumpTextureVariable( nullptr ), _pUseParallaxTextureVariable( nullptr ),
		_pAmbientLightColourVariable( nullptr ), _pDiffuseLightColourVariable( nullptr ),
		_pSpecularLightColourVariable( nullptr ), _pLightDirectionVariable( nullptr ),
		_pAmbientMaterialVariable( nullptr ), _pDiffuseMaterialVariable( nullptr ),
		_pSpecularMaterialVariable( nullptr ), _pSpecularPowerVariable( nullptr ),
		_pTextureRepeatVariable( nullptr ), _pCameraPositionVariable( nullptr ),
		_ambientMaterial( ambientMaterial ), _diffuseMaterial( diffuseMaterial ),
		_specularMaterial( specularMaterial ), _specularPower( specularPower )
	{
		ZeroMemory( &_techniqueDesc, sizeof( D3D10_TECHNIQUE_DESC ) );
	}
	Material::~Material( )
	{
		HP_RELEASE( _pEnvMapTexture );
		HP_RELEASE( _pParallaxTexture );
		HP_RELEASE( _pBumpTexture );
		HP_RELEASE( _pSpecularTexture );
		HP_RELEASE( _pDiffuseTexture );
		HP_RELEASE( _pInputLayout );
		HP_RELEASE( _pEffect );
	}
	bool Material::init( Renderer* pRenderer )
	{
		if ( loadEffectFromFile( pRenderer ) )
		{
			_pTechnique = _pEffect->GetTechniqueByName( _techniqueName.c_str( ) );
			_pTechnique->GetDesc( &_techniqueDesc );
			if ( createVertexLayout( pRenderer ) )
			{
				// retrieve all variables using semantic
				_pWorldMatrixVariable = _pEffect->GetVariableBySemantic( "WORLD" )->AsMatrix( );
				_pViewMatrixVariable = _pEffect->GetVariableBySemantic( "VIEW" )->AsMatrix( );
				_pProjectionMatrixVariable = _pEffect->GetVariableBySemantic( "PROJECTION" )->AsMatrix( );
				_pDiffuseTextureVariable = _pEffect->GetVariableByName( "diffuseMap" )->AsShaderResource( );
				_pSpecularTextureVariable = _pEffect->GetVariableByName( "specularMap" )->AsShaderResource( );
				_pBumpTextureVariable = _pEffect->GetVariableByName( "bumpMap" )->AsShaderResource( );
				_pParallaxTextureVariable = _pEffect->GetVariableByName( "heightMap" )->AsShaderResource( );
				_pEnvMapVariable = _pEffect->GetVariableByName( "envMap" )->AsShaderResource( );
				// lights
				_pAmbientLightColourVariable = _pEffect->GetVariableByName( "ambientLightColour" )->AsVector( );
				_pDiffuseLightColourVariable = _pEffect->GetVariableByName( "diffuseLightColour" )->AsVector( );
				_pSpecularLightColourVariable = _pEffect->GetVariableByName( "specularLightColour" )->AsVector( );
				_pLightDirectionVariable = _pEffect->GetVariableByName( "lightDirection" )->AsVector( );
				// materials
				_pAmbientMaterialVariable = _pEffect->GetVariableByName( "ambientMaterialColour" )->AsVector( );
				_pDiffuseMaterialVariable = _pEffect->GetVariableByName( "diffuseMaterialColour" )->AsVector( );
				_pSpecularMaterialVariable = _pEffect->GetVariableByName( "specularMaterialColour" )->AsVector( );
				_pSpecularPowerVariable = _pEffect->GetVariableByName( "specularPower" )->AsScalar( );
				_pTextureRepeatVariable = _pEffect->GetVariableByName( "textureRepeat" )->AsVector( );
				// camera
				_pCameraPositionVariable = _pEffect->GetVariableByName( "cameraPosition" )->AsVector( );
				// booleans
				_pUseDiffuseTextureVariable = _pEffect->GetVariableByName( "useDiffuseTexture" )->AsScalar( );
				_pUseSpecularTextureVariable = _pEffect->GetVariableByName( "useSpecularTexture" )->AsScalar( );
				_pUseBumpTextureVariable = _pEffect->GetVariableByName( "useBumpTexture" )->AsScalar( );
				_pUseParallaxTextureVariable = _pEffect->GetVariableByName( "useHeightTexture" )->AsScalar( );
				return true;
			}
		}
		return false;
	}
	bool Material::createVertexLayout( Renderer* pRenderer )
	{
		UInt32 elementsCount = ARRAYSIZE( D3D11_LAYOUT );
		D3DX11_PASS_DESC passDesc;
		_pTechnique->GetPassByIndex( 0 )->GetDesc( &passDesc );
		if ( FAILED( pRenderer->device( )->CreateInputLayout( D3D11_LAYOUT, elementsCount,
			passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &_pInputLayout ) ) )
		{
			return false;
		}
		return true;
	}
	bool Material::loadDiffuseTexture( Renderer* pRenderer, const String& filename )
	{
		return loadTexture( &_pDiffuseTexture, pRenderer, filename );
	}
	bool Material::loadSpecularTexture( Renderer* pRenderer, const String& filename )
	{
		return loadTexture( &_pSpecularTexture, pRenderer, filename );
	}
	bool Material::loadBumpTexture( Renderer* pRenderer, const String& filename )
	{
		return loadTexture( &_pBumpTexture, pRenderer, filename );
	}
	bool Material::loadParallaxTexture( Renderer* pRenderer, const String& filename )
	{
		return loadTexture( &_pParallaxTexture, pRenderer, filename );
	}
	bool Material::loadEnvMapTexture( Renderer* pRenderer, const String& filename )
	{
		return loadTexture( &_pEnvMapTexture, pRenderer, filename );
	}
	void Material::bindInputLayout( Renderer* pRenderer )
	{
		pRenderer->deviceContext( )->IASetInputLayout( _pInputLayout );
	}
	void Material::applyPass( Renderer* pRenderer, UInt32 i )
	{
		_pPass = _pTechnique->GetPassByIndex( i );
		_pPass->Apply( 0, pRenderer->deviceContext( ) );
	}
	bool Material::loadEffectFromFile( Renderer* pRenderer )
	{
		ID3DBlob* pBuffer = nullptr;
		if ( !compileD3DShader( _filename, "fx_5_0", &pBuffer ) )
		{
			return false;
		}
		if ( FAILED( D3DX11CreateEffectFromMemory( pBuffer->GetBufferPointer( ),
			pBuffer->GetBufferSize( ), 0, pRenderer->device( ), &_pEffect ) ) )
		{
			HP_RELEASE( pBuffer );
			return false;
		}
		return true;
	}
	bool Material::compileD3DShader( const String& filePath, const String& shaderModel,
		ID3DBlob** ppBuffer )
	{
		std::wstring wFilePath;
		wFilePath.assign( filePath.begin( ), filePath.end( ) );
		DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#       ifdef HP_DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG;
#       endif
		ID3DBlob* errorBuffer = 0;
		HRESULT result;
		result = D3DCompileFromFile( wFilePath.c_str( ), 0, 0, 0, shaderModel.c_str( ),
			shaderFlags, 0, ppBuffer, &errorBuffer );
		if ( FAILED( result ) )
		{
			HP_RELEASE( errorBuffer );
			ERR( "Error while compiling " + filePath + " shader." );
			return false;
		}
		HP_RELEASE( errorBuffer );
		return true;
	}
	bool Material::loadTexture( ID3D11ShaderResourceView** texture, Renderer* pRenderer,
		const String& filename )
	{
		String fileExt = filename.substr( filename.find( '.' ) + 1 );
		std::transform( fileExt.begin( ), fileExt.end( ), fileExt.begin( ), ::tolower );
		std::wstring wFilename = s2ws( filename );
		if ( fileExt.compare( "dds" ) == 0 )
		{
			if ( FAILED( DirectX::CreateDDSTextureFromFile( pRenderer->device( ),
				wFilename.c_str( ), nullptr, texture ) ) )
			{
				ERR( "Failed to load \"" + filename + "\" texture." );
				return false;
			}
		}
		else
		{
			if ( FAILED( DirectX::CreateWICTextureFromFile( pRenderer->device( ),
				wFilename.c_str( ), nullptr, texture ) ) )
			{
				ERR( "Failed to load \"" + filename + "\" texture." );
				return false;
			}
		}
		return true;
	}
}

