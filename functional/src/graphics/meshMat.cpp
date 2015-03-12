#include <pch.hpp>
#include <algorithm>
#include <graphics/meshMat.hpp>
#include <graphics/rendererMut.hpp>
#include <graphics/vertex.hpp>
#include <utils/stringUtils.hpp>
namespace hp_fp
{
	MeshMatMut defaultMat( )
	{
		return MeshMatMut
		{
			"assets/shaders/specular.fx",
			"Render",
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ },
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ 1.0f, 1.0f },
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			{ 0.5f, 0.5f, 0.5f },
			{ 0.8f, 0.8f, 0.8f },
			{ 1.0f, 1.0f, 1.0f },
			25.0f
		};
	}
	bool init_IO( MeshMatMut& meshMat, RendererMut& renderer )
	{
		if ( loadShader_IO( meshMat, renderer ) )
		{
			meshMat.technique = meshMat.effect->GetTechniqueByName( meshMat.techniqueName.c_str( ) );
			meshMat.technique->GetDesc( &meshMat.techniqueDesc );
			if ( createVertexLayout_IO( meshMat, renderer ) )
			{
				// retrieve all variables using semantic
				meshMat.worldMatrixVariable = meshMat.effect->GetVariableBySemantic( "WORLD" )->AsMatrix( );
				meshMat.viewMatrixVariable = meshMat.effect->GetVariableBySemantic( "VIEW" )->AsMatrix( );
				meshMat.projectionMatrixVariable = meshMat.effect->GetVariableBySemantic( "PROJECTION" )->AsMatrix( );
				meshMat.diffuseTextureVariable = meshMat.effect->GetVariableByName( "diffuseMap" )->AsShaderResource( );
				meshMat.specularTextureVariable = meshMat.effect->GetVariableByName( "specularMap" )->AsShaderResource( );
				meshMat.bumpTextureVariable = meshMat.effect->GetVariableByName( "bumpMap" )->AsShaderResource( );
				meshMat.parallaxTextureVariable = meshMat.effect->GetVariableByName( "heightMap" )->AsShaderResource( );
				meshMat.envMapVariable = meshMat.effect->GetVariableByName( "envMap" )->AsShaderResource( );
				// lights
				meshMat.ambientLightColourVariable = meshMat.effect->GetVariableByName( "ambientLightColour" )->AsVector( );
				meshMat.diffuseLightColourVariable = meshMat.effect->GetVariableByName( "diffuseLightColour" )->AsVector( );
				meshMat.specularLightColourVariable = meshMat.effect->GetVariableByName( "specularLightColour" )->AsVector( );
				meshMat.lightDirectionVariable = meshMat.effect->GetVariableByName( "lightDirection" )->AsVector( );
				// materials
				meshMat.ambientMaterialVariable = meshMat.effect->GetVariableByName( "ambientMaterialColour" )->AsVector( );
				meshMat.diffuseMaterialVariable = meshMat.effect->GetVariableByName( "diffuseMaterialColour" )->AsVector( );
				meshMat.specularMaterialVariable = meshMat.effect->GetVariableByName( "specularMaterialColour" )->AsVector( );
				meshMat.specularPowerVariable = meshMat.effect->GetVariableByName( "specularPower" )->AsScalar( );
				meshMat.textureRepeatVariable = meshMat.effect->GetVariableByName( "textureRepeat" )->AsVector( );
				// camera
				meshMat.cameraPositionVariable = meshMat.effect->GetVariableByName( "cameraPosition" )->AsVector( );
				// booleans
				meshMat.useDiffuseTextureVariable = meshMat.effect->GetVariableByName( "useDiffuseTexture" )->AsScalar( );
				meshMat.useSpecularTextureVariable = meshMat.effect->GetVariableByName( "useSpecularTexture" )->AsScalar( );
				meshMat.useBumpTextureVariable = meshMat.effect->GetVariableByName( "useBumpTexture" )->AsScalar( );
				meshMat.useParallaxTextureVariable = meshMat.effect->GetVariableByName( "useHeightTexture" )->AsScalar( );
				return true;
			}
		}
		return false;
	}
	bool loadTexture_IO( ID3D11ShaderResourceView* texture, RendererMut& renderer,
		const String& filename )
	{
		String fileExt = filename.substr( filename.find( '.' ) + 1 );
		std::transform( fileExt.begin( ), fileExt.end( ), fileExt.begin( ), ::tolower );
		std::wstring wFilename = s2ws( filename );
		if ( fileExt.compare( "dds" ) == 0 )
		{
			if ( FAILED( DirectX::CreateDDSTextureFromFile( renderer.device,
				wFilename.c_str( ), nullptr, &texture ) ) )
			{
				return false;
			}
		}
		else
		{
			if ( FAILED( DirectX::CreateWICTextureFromFile( renderer.device,
				wFilename.c_str( ), nullptr, &texture ) ) )
			{
				return false;
			}
		}
		return true;
	}
	namespace
	{
		bool loadShader_IO( MeshMatMut& meshMat, RendererMut& renderer )
		{
			ID3DBlob* buffer = NULL;
			if ( !loadAndCompile_IO( meshMat, renderer, "fx_5_0", &buffer ) )
			{
				return false;
			}
			if ( FAILED( D3DX11CreateEffectFromMemory( buffer->GetBufferPointer( ),
				buffer->GetBufferSize( ), 0, renderer.device, &meshMat.effect ) ) )
			{
				HP_RELEASE( buffer );
				return false;
			}
			return true;
		}
		bool loadAndCompile_IO( MeshMatMut& meshMat, RendererMut& renderer, const String& shaderModel,
			ID3DBlob** buffer )
		{
			std::wstring wFilePath = s2ws( meshMat.filename );
			DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#       ifdef HP_DEBUG
			shaderFlags |= D3DCOMPILE_DEBUG;
#       endif
			ID3DBlob* errorBuffer = 0;
			HRESULT result = D3DCompileFromFile( wFilePath.c_str( ), 0, 0, 0, shaderModel.c_str( ),
				shaderFlags, 0, buffer, &errorBuffer );
			if ( FAILED( result ) )
			{
				HP_RELEASE( errorBuffer );
				return false;
			}
			HP_RELEASE( errorBuffer );
			return true;
		}
		bool createVertexLayout_IO( MeshMatMut& meshMat, RendererMut& renderer )
		{
			UInt32 elementsCount = ARRAYSIZE( D3D11_LAYOUT );
			D3DX11_PASS_DESC passDesc;
			meshMat.technique->GetPassByIndex( 0 )->GetDesc( &passDesc );
			if ( FAILED( renderer.device->CreateInputLayout( D3D11_LAYOUT, elementsCount,
				passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &meshMat.inputLayout ) ) )
			{
				return false;
			}
			return true;
		}
	}
}